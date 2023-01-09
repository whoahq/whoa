#ifndef NET_GRUNT_GRUNT_HPP
#define NET_GRUNT_GRUNT_HPP

namespace Grunt {
    class ClientLink;
    class ClientResponse;
    template <class>
    class Command;
    class Pending;
    class Timer;

    extern Command<ClientLink> s_clientCommands[];

    enum Result {
        GRUNT_RESULT_0  = 0,
        GRUNT_RESULT_1  = 1,
        GRUNT_RESULT_2  = 2,
        GRUNT_RESULT_3  = 3,
        GRUNT_RESULT_4  = 4,
        GRUNT_RESULT_5  = 5,
        GRUNT_RESULT_6  = 6,
        GRUNT_RESULT_7  = 7,
        GRUNT_RESULT_8  = 8,
        GRUNT_RESULT_9  = 9,
        GRUNT_RESULT_10 = 10,
        GRUNT_RESULT_11 = 11,
        GRUNT_RESULT_12 = 12,
        GRUNT_RESULT_13 = 13,
        GRUNT_RESULT_14 = 14,
        GRUNT_RESULT_15 = 15,
        GRUNT_RESULT_16 = 16,
        GRUNT_RESULT_17 = 17,
        GRUNT_RESULT_18 = 18,
        GRUNT_RESULT_19 = 19,
        GRUNT_RESULT_20 = 20,
        GRUNT_RESULT_21 = 21,
        GRUNT_RESULT_22 = 22,
        GRUNT_RESULT_23 = 23,
        GRUNT_RESULT_24 = 24,
        GRUNT_RESULT_25 = 25,
        GRUNT_RESULT_26 = 26,
        GRUNT_RESULT_27 = 27,
        GRUNT_RESULT_28 = 28,
        GRUNT_RESULT_29 = 29,
        GRUNT_RESULT_30 = 30,
        GRUNT_RESULT_31 = 31,
        GRUNT_RESULT_32 = 32,
        GRUNT_RESULT_LAST,
    };
}

#endif
