#ifndef NET_ERROR_H
#define NET_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/** Generic errors */
enum EErrors {
    ESUCCESS = 0, /* Not actually an error. Represents success */

    EUNKNOWN, /* An unknown error */
    EUNSUPPORTED, /* Something was requested that is not supported */
    ENULL_POINTER, /* A pointer was null */
    EINVALID_STATE, /* The current state is invalid */

    EERRORS_END /* Utility for the end of this enum */
};

/** Network related errors */
enum ENetError {
    ENETERROR_START = EERRORS_END, /* Utility for the beginning of this enum */

    EINCORRECT_SIDE, /* A method was called on the wrong side */
    EIN_USE, /* An address or connection is already in use */
    EINVALID_IP, /* The specified address is invalid */
    ECLOSED, /* Not actually an error. The connection closed */

    ENETERROR_END /* Utility for the end of this enum */
};

#ifdef __cplusplus
}
#endif

#endif
