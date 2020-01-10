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

    EERRORS_END /* Utility for the end of this enum */
};

/** Network related errors */
enum ENetError {
    EIN_USE = EERRORS_END, /* An address is already in use */
    EINVALID_IP, /* The specified address is invalid */
    ECLOSED /* Not actually an error. The connection closed */
};

#ifdef __cplusplus
}
#endif

#endif
