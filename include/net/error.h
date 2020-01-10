#ifndef NET_ERROR_H
#define NET_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

enum EErrors {
    ESUCCESS = 0, /* Not actually an error. Represents success */

    EUNKNOWN,
    ENULL_POINTER, /* A pointer was null */

    EERRORS_END /* Utility for the end of this enum */
};

enum ENetError {
    EIN_USE = EERRORS_END, /* An address is already in use */
    EINVALID_IP,
    ECLOSED
};

#ifdef __cplusplus
}
#endif

#endif
