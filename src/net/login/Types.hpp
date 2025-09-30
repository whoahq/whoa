#ifndef NET_LOGIN_TYPES_HPP
#define NET_LOGIN_TYPES_HPP

#define LOGIN_RECONNECT_CHALLENGE_LEN 16
#define LOGIN_VERSION_CHALLENGE_LEN 16
#define LOGIN_VERSION_CHECKSUM_LEN  20

enum LOGIN_RESULT {
    LOGIN_OK                        = 0,
    LOGIN_INVALID_CHALLENGE_MESSAGE = 1,
    LOGIN_SRP_ERROR                 = 2,
    LOGIN_INVALID_PROOF_MESSAGE     = 3,
    LOGIN_BAD_SERVER_PROOF          = 4,
    LOGIN_INVALID_RECODE_MESSAGE    = 5,
    LOGIN_BAD_SERVER_RECODE_PROOF   = 6,
    LOGIN_UNKNOWN_ACCOUNT           = 7,
    LOGIN_UNKNOWN_ACCOUNT_PIN       = 8,
    LOGIN_UNKNOWN_ACCOUNT_CALL      = 9,
    LOGIN_INCORRECT_PASSWORD        = 10,
    LOGIN_FAILED                    = 11,
    LOGIN_SERVER_DOWN               = 12,
    LOGIN_BANNED                    = 13,
    LOGIN_BADVERSION                = 14,
    LOGIN_ALREADYONLINE             = 15,
    LOGIN_NOTIME                    = 16,
    LOGIN_DBBUSY                    = 17,
    LOGIN_SUSPENDED                 = 18,
    LOGIN_PARENTALCONTROL           = 19,
    LOGIN_LOCKED_ENFORCED           = 20,
    LOGIN_RESULT_21                 = 21, // DISCONNECTED
    LOGIN_ACCOUNT_CONVERTED         = 22,
    LOGIN_ANTI_INDULGENCE           = 23,
    LOGIN_EXPIRED                   = 24,
    LOGIN_TRIAL_EXPIRED             = 25,
    LOGIN_NO_GAME_ACCOUNT           = 26,
    LOGIN_AUTH_OUTAGE               = 27,
    LOGIN_GAME_ACCOUNT_LOCKED       = 28,
    LOGIN_NO_BATTLENET_MANAGER      = 29,
    LOGIN_NO_BATTLENET_APPLICATION  = 30,
    LOGIN_MALFORMED_ACCOUNT_NAME    = 31,
    LOGIN_USE_GRUNT                 = 32,
    LOGIN_TOO_FAST                  = 33,
    LOGIN_CHARGEBACK                = 34,
    LOGIN_IGR_WITHOUT_BNET          = 35,
    LOGIN_UNLOCKABLE_LOCK           = 36,
    LOGIN_CONVERSION_REQUIRED       = 37,
    LOGIN_UNABLE_TO_DOWNLOAD_MODULE = 38,
    LOGIN_NO_GAME_ACCOUNTS_IN_REGION = 39,
    LOGIN_ACCOUNT_LOCKED            = 40,
    LOGIN_RESULT_MAX                = 41,
};

enum LOGIN_STATE {
    LOGIN_STATE_INITIALIZED         = 0,
    LOGIN_STATE_CONNECTING          = 1,
    LOGIN_STATE_HANDSHAKING         = 2,
    LOGIN_STATE_AUTHENTICATING      = 3,
    LOGIN_STATE_AUTHENTICATED       = 4,
    LOGIN_STATE_FAILED              = 5,
    LOGIN_STATE_DOWNLOADFILE        = 6,
    LOGIN_STATE_FIRST_SECURITY      = 7,
    LOGIN_STATE_PIN                 = 8,
    LOGIN_STATE_PIN_WAIT            = 9,
    LOGIN_STATE_MATRIX              = 10,
    LOGIN_STATE_MATRIX_WAIT         = 11,
    LOGIN_STATE_TOKEN               = 12,
    LOGIN_STATE_TOKEN_WAIT          = 13,
    LOGIN_STATE_CHECKINGVERSIONS    = 14,
    LOGIN_STATE_CONNECTED           = 15,
    LOGIN_STATE_DISCONNECTED        = 16,
    LOGIN_STATE_SURVEY              = 17,
    LOGIN_STATE_MAX                 = 18,
};

#endif
