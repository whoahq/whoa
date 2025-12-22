#include "util/Mpq.hpp"
#include <bzlib.h>
#include <zlib.h>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>

namespace {
    constexpr uint32_t ID_MPQ = 0x1A51504D;
    constexpr uint32_t ID_MPQ_USERDATA = 0x1B51504D;

    constexpr uint32_t MPQ_FILE_IMPLODE = 0x00000100;
    constexpr uint32_t MPQ_FILE_COMPRESSED = 0x00000200;
    constexpr uint32_t MPQ_FILE_ENCRYPTED = 0x00010000;
    constexpr uint32_t MPQ_FILE_SINGLE_UNIT = 0x01000000;

    constexpr uint8_t MPQ_COMP_ZLIB = 0x02;
    constexpr uint8_t MPQ_COMP_BZIP2 = 0x08;

#pragma pack(push, 1)
    struct MpqUserDataHeader {
        uint32_t id;
        uint32_t userDataSize;
        uint32_t headerOffset;
        uint32_t userDataHeaderSize;
    };

    struct MpqHeaderV2 {
        uint32_t id;
        uint32_t headerSize;
        uint32_t archiveSize;
        uint16_t formatVersion;
        uint16_t sectorSizeShift;
        uint32_t hashTablePos;
        uint32_t blockTablePos;
        uint32_t hashTableSize;
        uint32_t blockTableSize;
        uint64_t hiBlockTablePos64;
        uint16_t hashTablePosHi;
        uint16_t blockTablePosHi;
    };

    struct MpqHashEntry {
        uint32_t hashA;
        uint32_t hashB;
        uint16_t locale;
        uint16_t platform;
        uint32_t blockIndex;
    };

    struct MpqBlockEntry {
        uint32_t filePos;
        uint32_t compressedSize;
        uint32_t fileSize;
        uint32_t flags;
    };
#pragma pack(pop)

    uint32_t g_cryptTable[0x500];
    bool g_cryptReady = false;
    uint16_t g_locale = 0;
    void PrepareCryptTable() {
        uint32_t seed = 0x00100001;
        for (uint32_t i = 0; i < 0x100; ++i) {
            for (uint32_t j = 0; j < 5; ++j) {
                seed = (seed * 125 + 3) % 0x2AAAAB;
                uint32_t temp1 = (seed & 0xFFFF) << 16;
                seed = (seed * 125 + 3) % 0x2AAAAB;
                uint32_t temp2 = (seed & 0xFFFF);
                g_cryptTable[i + j * 0x100] = temp1 | temp2;
            }
        }
        g_cryptReady = true;
    }

    uint32_t HashString(const std::string& input, uint32_t type) {
        if (!g_cryptReady) {
            PrepareCryptTable();
        }

        uint32_t seed1 = 0x7FED7FED;
        uint32_t seed2 = 0xEEEEEEEE;

        for (char ch : input) {
            uint8_t c = static_cast<uint8_t>(std::toupper(static_cast<unsigned char>(ch)));
            seed1 = g_cryptTable[(type << 8) + c] ^ (seed1 + seed2);
            seed2 = c + seed1 + seed2 + (seed2 << 5) + 3;
        }

        return seed1;
    }

    void DecryptBlock(uint32_t* data, size_t length, uint32_t key) {
        if (!g_cryptReady) {
            PrepareCryptTable();
        }

        uint32_t seed1 = key;
        uint32_t seed2 = 0xEEEEEEEE;

        for (size_t i = 0; i < length; ++i) {
            seed2 += g_cryptTable[0x400 + (seed1 & 0xFF)];
            uint32_t value = data[i];
            uint32_t decoded = value ^ (seed1 + seed2);
            data[i] = decoded;
            seed1 = ((~seed1 << 21) + 0x11111111) | (seed1 >> 11);
            seed2 = decoded + seed2 + (seed2 << 5) + 3;
        }
    }

    std::string NormalizePath(const char* filename) {
        std::string path = filename ? filename : "";
        for (char& ch : path) {
            if (ch == '/') {
                ch = '\\';
            }
        }
        return path;
    }

    bool FileExists(const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        return file.is_open();
    }

    bool DirectoryExists(const std::string& path) {
        struct stat st = {};
        return stat(path.c_str(), &st) == 0 && (st.st_mode & S_IFDIR);
    }

    uint16_t LocaleFromString(const std::string& locale) {
        if (locale == "enUS") return 0x0409;
        if (locale == "enGB") return 0x0809;
        if (locale == "frFR") return 0x040C;
        if (locale == "deDE") return 0x0407;
        if (locale == "esES") return 0x0C0A;
        if (locale == "esMX") return 0x080A;
        if (locale == "ruRU") return 0x0419;
        if (locale == "koKR") return 0x0412;
        if (locale == "zhCN") return 0x0804;
        if (locale == "zhTW") return 0x0404;
        return 0;
    }

    bool DecompressZlib(const uint8_t* in, size_t inSize, uint8_t* out, size_t outSize) {
        uLongf destLen = static_cast<uLongf>(outSize);
        int result = uncompress(out, &destLen, in, static_cast<uLongf>(inSize));
        return result == Z_OK && destLen == outSize;
    }

    bool DecompressBzip2(const uint8_t* in, size_t inSize, uint8_t* out, size_t outSize) {
        unsigned int destLen = static_cast<unsigned int>(outSize);
        int result = BZ2_bzBuffToBuffDecompress(
            reinterpret_cast<char*>(out),
            &destLen,
            const_cast<char*>(reinterpret_cast<const char*>(in)),
            static_cast<unsigned int>(inSize),
            0,
            0
        );
        return result == BZ_OK && destLen == outSize;
    }

    bool DecompressBlock(const uint8_t* in, size_t inSize, uint8_t* out, size_t outSize) {
        if (inSize == outSize) {
            std::memcpy(out, in, outSize);
            return true;
        }

        if (inSize == 0) {
            return false;
        }

        uint8_t type = in[0];
        const uint8_t* payload = in + 1;
        size_t payloadSize = inSize - 1;

        if (type == 0) {
            if (payloadSize != outSize) {
                return false;
            }
            std::memcpy(out, payload, outSize);
            return true;
        }

        if ((type & (type - 1)) != 0) {
            return false;
        }

        if (type == MPQ_COMP_ZLIB) {
            return DecompressZlib(payload, payloadSize, out, outSize);
        }

        if (type == MPQ_COMP_BZIP2) {
            return DecompressBzip2(payload, payloadSize, out, outSize);
        }

        return false;
    }

    struct MpqArchive {
        std::ifstream stream;
        uint64_t baseOffset = 0;
        uint32_t sectorSize = 0;
        uint32_t hashTableSize = 0;
        uint32_t blockTableSize = 0;
        std::vector<MpqHashEntry> hashTable;
        std::vector<MpqBlockEntry> blockTable;

        bool Open(const std::string& archivePath) {
            stream.open(archivePath, std::ios::binary);
            if (!stream.is_open()) {
                return false;
            }

            MpqUserDataHeader userHeader = {};
            MpqHeaderV2 header = {};

            stream.read(reinterpret_cast<char*>(&userHeader), sizeof(userHeader));
            if (!stream.good()) {
                return false;
            }

            if (userHeader.id == ID_MPQ_USERDATA) {
                baseOffset = userHeader.headerOffset;
            } else {
                baseOffset = 0;
            }

            stream.seekg(static_cast<std::streamoff>(baseOffset), std::ios::beg);
            stream.read(reinterpret_cast<char*>(&header), sizeof(header));
            if (!stream.good()) {
                return false;
            }

            if (header.id != ID_MPQ || header.headerSize < 32) {
                return false;
            }

            if (header.formatVersion > 1) {
                return false;
            }

            sectorSize = 512u << header.sectorSizeShift;
            hashTableSize = header.hashTableSize;
            blockTableSize = header.blockTableSize;

            uint64_t hashTablePos = baseOffset + header.hashTablePos;
            uint64_t blockTablePos = baseOffset + header.blockTablePos;

            if (header.headerSize >= 44) {
                hashTablePos = baseOffset + (static_cast<uint64_t>(header.hashTablePosHi) << 32) + header.hashTablePos;
                blockTablePos = baseOffset + (static_cast<uint64_t>(header.blockTablePosHi) << 32) + header.blockTablePos;
            }

            hashTable.resize(hashTableSize);
            blockTable.resize(blockTableSize);

            stream.seekg(static_cast<std::streamoff>(hashTablePos), std::ios::beg);
            stream.read(reinterpret_cast<char*>(hashTable.data()), hashTableSize * sizeof(MpqHashEntry));

            stream.seekg(static_cast<std::streamoff>(blockTablePos), std::ios::beg);
            stream.read(reinterpret_cast<char*>(blockTable.data()), blockTableSize * sizeof(MpqBlockEntry));

            if (!stream.good()) {
                return false;
            }

            DecryptBlock(reinterpret_cast<uint32_t*>(hashTable.data()), hashTableSize * sizeof(MpqHashEntry) / 4, HashString("(hash table)", 3));
            DecryptBlock(reinterpret_cast<uint32_t*>(blockTable.data()), blockTableSize * sizeof(MpqBlockEntry) / 4, HashString("(block table)", 3));

            return true;
        }

        const MpqBlockEntry* FindBlock(const std::string& filename) const {
            if (hashTable.empty()) {
                return nullptr;
            }

            uint32_t hashA = HashString(filename, 1);
            uint32_t hashB = HashString(filename, 2);
            uint32_t hashStart = HashString(filename, 0) & (hashTableSize - 1);

            const MpqHashEntry* neutral = nullptr;

            for (uint32_t i = 0; i < hashTableSize; ++i) {
                uint32_t index = (hashStart + i) & (hashTableSize - 1);
                const MpqHashEntry& entry = hashTable[index];
                if (entry.blockIndex == 0xFFFFFFFF) {
                    break;
                }
                if (entry.hashA == hashA && entry.hashB == hashB) {
                    if (g_locale == 0) {
                        if (entry.blockIndex < blockTable.size()) {
                            return &blockTable[entry.blockIndex];
                        }
                    }
                    if (entry.locale == g_locale) {
                        if (entry.blockIndex < blockTable.size()) {
                            return &blockTable[entry.blockIndex];
                        }
                    }
                    if (entry.locale == 0 && !neutral) {
                        neutral = &entry;
                    }
                }
            }

            if (neutral && neutral->blockIndex < blockTable.size()) {
                return &blockTable[neutral->blockIndex];
            }

            return nullptr;
        }

        bool ReadFile(const std::string& filename, uint8_t** data, size_t* size) {
            const MpqBlockEntry* block = FindBlock(filename);
            if (!block) {
                return false;
            }

            if (block->flags & MPQ_FILE_ENCRYPTED) {
                return false;
            }

            if (block->flags & MPQ_FILE_IMPLODE) {
                return false;
            }

            uint64_t filePos = baseOffset + block->filePos;
            uint32_t fileSize = block->fileSize;

            std::vector<uint8_t> buffer(fileSize);

            if (block->flags & MPQ_FILE_SINGLE_UNIT) {
                std::vector<uint8_t> raw(block->compressedSize);
                stream.seekg(static_cast<std::streamoff>(filePos), std::ios::beg);
                stream.read(reinterpret_cast<char*>(raw.data()), raw.size());
                if (!stream.good()) {
                    return false;
                }

                if (block->flags & MPQ_FILE_COMPRESSED) {
                    if (!DecompressBlock(raw.data(), raw.size(), buffer.data(), buffer.size())) {
                        return false;
                    }
                } else {
                    if (raw.size() != buffer.size()) {
                        return false;
                    }
                    std::memcpy(buffer.data(), raw.data(), buffer.size());
                }
            } else if ((block->flags & MPQ_FILE_COMPRESSED) == 0) {
                stream.seekg(static_cast<std::streamoff>(filePos), std::ios::beg);
                stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
                if (!stream.good()) {
                    return false;
                }
            } else {
                uint32_t sectorCount = (fileSize + sectorSize - 1) / sectorSize;
                uint32_t tableSize = (sectorCount + 1) * sizeof(uint32_t);
                std::vector<uint32_t> offsets(sectorCount + 1);

                stream.seekg(static_cast<std::streamoff>(filePos), std::ios::beg);
                stream.read(reinterpret_cast<char*>(offsets.data()), tableSize);
                if (!stream.good()) {
                    return false;
                }

                for (uint32_t i = 0; i < sectorCount; ++i) {
                    uint32_t start = offsets[i];
                    uint32_t end = offsets[i + 1];
                    if (end < start) {
                        return false;
                    }

                    uint32_t compSize = end - start;
                    uint32_t remaining = fileSize - i * sectorSize;
                    uint32_t outSize = std::min(sectorSize, remaining);

                    std::vector<uint8_t> raw(compSize);
                    stream.seekg(static_cast<std::streamoff>(filePos + start), std::ios::beg);
                    stream.read(reinterpret_cast<char*>(raw.data()), raw.size());
                    if (!stream.good()) {
                        return false;
                    }

                    uint8_t* outPtr = buffer.data() + i * sectorSize;
                    if (!DecompressBlock(raw.data(), raw.size(), outPtr, outSize)) {
                        if (raw.size() == outSize) {
                            std::memcpy(outPtr, raw.data(), outSize);
                        } else {
                            return false;
                        }
                    }
                }
            }

            *size = buffer.size();
            auto output = new uint8_t[buffer.size()];
            std::memcpy(output, buffer.data(), buffer.size());
            *data = output;

            return true;
        }
    };

    struct MpqManager {
        bool initialized = false;
        std::string dataRoot;
        std::string locale;
        std::vector<MpqArchive> archives;

        void Initialize() {
            if (initialized) {
                return;
            }

            initialized = true;

            if (DirectoryExists("Data")) {
                dataRoot = "Data";
            } else if (DirectoryExists("../Data")) {
                dataRoot = "../Data";
            } else {
                dataRoot = "Data";
            }

            const char* locales[] = {
                "enUS",
                "enGB",
                "frFR",
                "deDE",
                "esES",
                "esMX",
                "ruRU",
                "koKR",
                "zhCN",
                "zhTW"
            };

            for (const char* loc : locales) {
                std::string probe = dataRoot + "/" + loc + "/locale-" + loc + ".MPQ";
                if (FileExists(probe)) {
                    locale = loc;
                    break;
                }
            }

            if (locale.empty()) {
                locale = "enUS";
            }

            g_locale = LocaleFromString(locale);

            AddArchives();
        }

        bool OpenArchive(const std::string& path) {
            MpqArchive archive;
            if (!archive.Open(path)) {
                return false;
            }
            archives.push_back(std::move(archive));
            return true;
        }

        void AddIfExists(const std::string& path) {
            if (FileExists(path)) {
                OpenArchive(path);
            }
        }

        void AddPatchSeries(const std::string& prefix) {
            for (int i = 9; i >= 1; --i) {
                std::string name = prefix + std::to_string(i) + ".MPQ";
                AddIfExists(name);
            }
        }

        void AddArchives() {
            AddPatchSeries(dataRoot + "/patch-");
            AddIfExists(dataRoot + "/patch.MPQ");

            std::string localeRoot = dataRoot + "/" + locale;
            AddPatchSeries(localeRoot + "/patch-" + locale + "-");
            AddIfExists(localeRoot + "/patch-" + locale + ".MPQ");

            AddIfExists(dataRoot + "/common.MPQ");
            AddIfExists(dataRoot + "/common-2.MPQ");
            AddIfExists(dataRoot + "/expansion.MPQ");
            AddIfExists(dataRoot + "/lichking.MPQ");

            AddIfExists(localeRoot + "/lichking-speech-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/expansion-speech-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/lichking-locale-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/expansion-locale-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/speech-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/locale-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/base-" + locale + ".MPQ");
            AddIfExists(localeRoot + "/backup-" + locale + ".MPQ");
        }

        bool ReadFile(const char* filename, uint8_t** data, size_t* size) {
            Initialize();

            std::string normalized = NormalizePath(filename);
            for (auto& archive : archives) {
                if (archive.ReadFile(normalized, data, size)) {
                    return true;
                }
            }

            return false;
        }

        bool HasFile(const char* filename) {
            Initialize();

            std::string normalized = NormalizePath(filename);
            for (const auto& archive : archives) {
                if (archive.FindBlock(normalized)) {
                    return true;
                }
            }

            return false;
        }
    };

    MpqManager& GetManager() {
        static MpqManager manager;
        return manager;
    }
}

namespace Mpq {
    bool FileExists(const char* filename) {
        return GetManager().HasFile(filename);
    }

    bool ReadFile(const char* filename, uint8_t** data, size_t* size) {
        return GetManager().ReadFile(filename, data, size);
    }
}
