#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

#include "socket.h"

extern NetHandler * gHandler;

typedef struct AddressParams {
    Socket * sock;
    char * address;
} AddressParams;

char * cr_strdup(char * str) {
    char * result = cr_malloc(sizeof(char) * strlen(str) + 1);
    result = strcpy(result, str);

    return result;
}

void cleanupAddressParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        AddressParams * tup = (AddressParams *) ctp->params + i;

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
    const size_t size = 6;                                                                                  \
    AddressParams * params = cr_malloc(sizeof(AddressParams) * size);                                       \
                                                                                                            \
    params[0] = (AddressParams) {.sock = NULL, .address = NULL};                                            \
    params[1] = (AddressParams) {.sock = NULL, .address = cr_strdup("")};                                   \
    params[2] = (AddressParams) {.sock = NULL, .address = cr_strdup("test")};                               \
    params[3] = (AddressParams) {.sock = createSocket(), .address = NULL};                                  \
    params[4] = (AddressParams) {.sock = createSocket(), .address = cr_strdup("")};                         \
    params[5] = (AddressParams) {.sock = createSocket(), .address = cr_strdup("test")};                     \
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
