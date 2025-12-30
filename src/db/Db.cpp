#include "db/Db.hpp"
#include "db/WowClientDB_Base.hpp"

WowClientDB<AchievementRec> g_achievementDB;
WowClientDB<AreaTableRec> g_areaTableDB;
WowClientDB<Cfg_CategoriesRec> g_cfg_CategoriesDB;
WowClientDB<Cfg_ConfigsRec> g_cfg_ConfigsDB;
WowClientDB<CharBaseInfoRec> g_charBaseInfoDB;
WowClientDB<CharHairGeosetsRec> g_charHairGeosetsDB;
WowClientDB<CharSectionsRec> g_charSectionsDB;
WowClientDB<CharacterFacialHairStylesRec> g_characterFacialHairStylesDB;
WowClientDB<ChrClassesRec> g_chrClassesDB;
WowClientDB<ChrRacesRec> g_chrRacesDB;
WowClientDB<CreatureDisplayInfoRec> g_creatureDisplayInfoDB;
WowClientDB<CreatureModelDataRec> g_creatureModelDataDB;
WowClientDB<FactionTemplateRec> g_factionTemplateDB;
WowClientDB<ItemDisplayInfoRec> g_itemDisplayInfoDB;
WowClientDB<ItemVisualsRec> g_itemVisualsDB;
WowClientDB<MapRec> g_mapDB;
WowClientDB<SoundEntriesRec> g_soundEntriesDB;
WowClientDB<SoundEntriesAdvancedRec> g_soundEntriesAdvancedDB;

void LoadDB(WowClientDB_Base* db, const char* filename, int32_t linenumber) {
    db->Load(filename, linenumber);
};

void StaticDBLoadAll(void (*loadFn)(WowClientDB_Base*, const char*, int32_t)) {
    loadFn(&g_achievementDB, __FILE__, __LINE__);
    loadFn(&g_areaTableDB, __FILE__, __LINE__);
    loadFn(&g_cfg_CategoriesDB, __FILE__, __LINE__);
    loadFn(&g_cfg_ConfigsDB, __FILE__, __LINE__);
    loadFn(&g_charBaseInfoDB, __FILE__, __LINE__);
    loadFn(&g_charHairGeosetsDB, __FILE__, __LINE__);
    loadFn(&g_charSectionsDB, __FILE__, __LINE__);
    loadFn(&g_characterFacialHairStylesDB, __FILE__, __LINE__);
    loadFn(&g_chrClassesDB, __FILE__, __LINE__);
    loadFn(&g_chrRacesDB, __FILE__, __LINE__);
    loadFn(&g_creatureDisplayInfoDB, __FILE__, __LINE__);
    loadFn(&g_creatureModelDataDB, __FILE__, __LINE__);
    loadFn(&g_factionTemplateDB, __FILE__, __LINE__);
    loadFn(&g_itemDisplayInfoDB, __FILE__, __LINE__);
    loadFn(&g_itemVisualsDB, __FILE__, __LINE__);
    loadFn(&g_mapDB, __FILE__, __LINE__);
    loadFn(&g_soundEntriesDB, __FILE__, __LINE__);
    loadFn(&g_soundEntriesAdvancedDB, __FILE__, __LINE__);
};

void ClientDBInitialize() {
    // TODO

    StaticDBLoadAll(LoadDB);

    // TODO
}
