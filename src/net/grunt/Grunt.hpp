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
        SUCCESS             = 0,
        GRUNT_RESULT_1      = 1,
        GRUNT_RESULT_2      = 2,
        BANNED              = 3,
        GRUNT_RESULT_4      = 4,
        GRUNT_RESULT_5      = 5,
        ALREADYONLINE       = 6,
        NOTIME              = 7,
        DBBUSY              = 8,
        BADVERSION          = 9,
        DOWNLOADFILE        = 10,
        GRUNT_RESULT_11     = 11,
        SUSPENDED           = 12,
        GRUNT_RESULT_13     = 13,
        SUCCESS_SURVEY      = 14,
        PARENTALCONTROL     = 15,
        LOCKED_ENFORCED     = 16,
        TRIAL_EXPIRED       = 17,
        ACCOUNT_CONVERTED   = 18,
        GRUNT_RESULT_19     = 19,
        GRUNT_RESULT_20     = 20,
        GRUNT_RESULT_21     = 21,
        CHARGEBACK          = 22,
        IGR_WITHOUT_BNET    = 23,
        GAME_ACCOUNT_LOCKED = 24,
        UNLOCKABLE_LOCK     = 25,
        GRUNT_RESULT_26     = 26,
        GRUNT_RESULT_27     = 27,
        GRUNT_RESULT_28     = 28,
        GRUNT_RESULT_29     = 29,
        GRUNT_RESULT_30     = 30,
        GRUNT_RESULT_31     = 31,
        CONVERSION_REQUIRED = 32,
        GRUNT_RESULT_LAST,
        DISCONNECTED        = 255
    };

    extern const char* g_LoginResultStringNames[];
    extern const char* g_LoginStateStringNames[];
}

#endif
