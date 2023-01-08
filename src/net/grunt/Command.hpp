#ifndef NET_GRUNT_COMMAND_HPP
#define NET_GRUNT_COMMAND_HPP

#include "net/Grunt.hpp"

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
    uint8_t cmd;
    msg.Get(cmd);

    for (uint32_t i = 0; i < commandCount; i++) {
        auto& command = commands[i];

        if (command.cmd == cmd) {
            auto callback = command.callback;
            auto result = (a4.*callback)(msg);

            if (result == 0) {
                return 1;
            } else {
                // TODO
                return 0;
            }
        }
    }

    // TODO

    return 0;
}

#endif
