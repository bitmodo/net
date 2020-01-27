#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <stdbool.h>

#include "handler.h"

extern NetHandler * gHandler;

// Socket functions

#define emptySocket ((Socket) {0, 0, NULL, 0, 0, NULL})

Socket * fillEmptySocket(Socket * sock) {
    *sock = emptySocket;
    return sock;
}

Socket * createSocket() {
    return cr_malloc(sizeof(Socket));
}

Socket * createEmptySocket() {
    return fillEmptySocket(createSocket());
}

// Address get and set tests

typedef struct AddressParams {
    Socket * sock;
    char * address;
} AddressParams;

void cleanupAddressParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        struct AddressParams *tup = (struct AddressParams *) ctp->params + i;

        if (tup->sock) {
            cr_free(tup->sock);
        }

        if (tup->address) {
            cr_free(tup->address);
        }
    }

    cr_free(ctp->params);
}

#define ADDRESS_PARAMS                                                                                      \
    const size_t size = 5;                                                                                  \
    AddressParams *params = cr_malloc(sizeof(AddressParams) * size);                                        \
                                                                                                            \
    int pos = 0;                                                                                            \
    for (int i = 0; i < 2; i++) {                                                                           \
        for (int j = 0; j < 3; j++) {                                                                       \
            char * address = NULL;                                                                          \
            if (j != 0) {                                                                                   \
                char * addrTmp = j == 1 ? "test" : "";                                                      \
                address = cr_malloc(sizeof(char) * strlen(addrTmp) + 1);                                    \
                address = strcpy(address, addrTmp);                                                         \
            }                                                                                               \
                                                                                                            \
            params[pos++] = (AddressParams) {.sock = (i == 0 ? NULL : createSocket()), .address = address}; \
        }                                                                                                   \
    }                                                                                                       \
                                                                                                            \
    return cr_make_param_array(AddressParams, params, size, cleanupAddressParam)

ParameterizedTestParameters(address, set) {
    ADDRESS_PARAMS;
}

ParameterizedTest(AddressParams * param, address, set) {
    cr_log_info("Address set test with {sock: %p, address: %s}\n", param->sock, param->address);
    netSetAddress(param->sock, param->address);

    if (param->sock) {
        if (param->address) {
            cr_assert_str_eq(param->sock->address, param->address, "Set address did not set the correct address");
        } else {
            cr_assert_null(param->sock->address, "Set address did not set the address to null");
        }
    }
}

ParameterizedTestParameters(address, get) {
    ADDRESS_PARAMS;
}

ParameterizedTest(AddressParams * param, address, get) {
    cr_log_info("Address get test with {sock: %p, address: %s}\n", param->sock, param->address);

    if (param->sock) {
        param->sock->address = param->address;

        if (param->address) {
            cr_assert_str_eq(netGetAddress(param->sock), param->address, "Get address did not return correct address");
        } else {
            cr_assert_null(netGetAddress(param->sock), "Null address did not return null address");
        }
    } else {
        cr_assert_null(netGetAddress(param->sock), "Null socket did not return null address");
    }
}
