#ifndef NET_GRUNT_COMMAND_HPP
#define NET_GRUNT_COMMAND_HPP

#include "net/Grunt.hpp"
#include "net/grunt/Util.hpp"

template <class T>
class Grunt::Command {
    public:
    // Static members
    static int32_t Process(CDataStore& msg, Command<T>* commands, uint32_t commandCount, T& a4, uint32_t& pos);

    // Member variables
    int32_t cmd;
    const char* name;
    int32_t (T::*callback)(CDataStore& msg);
    uint32_t unk;
};

template <class T>
int32_t Grunt::Command<T>::Process(CDataStore& msg, Command<T>* commands, uint32_t commandCount, T& a4, uint32_t& pos) {
    while (true) {
        if (!CanRead(msg, 1)) {
            return 1;
        }

        uint8_t cmd;
        msg.Get(cmd);

        Command<T>* command = nullptr;
        for (uint32_t i = 0; i < commandCount; i++) {
            if (commands[i].cmd == cmd) {
                command = &commands[i];
            }
        }

        if (!command) {
            // Bad command received [0x%02X] (unrecognized command)
            break;
        }

        auto result = (a4.*command->callback)(msg);

        if (result == 0) {
            return 1;
        }

        if (result == 1) {
            // Bad command received [0x%02X] (Corrupt)
            break;
        }

        if (result == 2) {
            pos = msg.Tell();
        }
    }

    // TODO

    return 0;
}

#endif
