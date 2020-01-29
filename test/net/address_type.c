#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

#include "socket.h"

typedef struct AddressTypeParams {
    Socket * sock;
    int addressType;
} AddressTypeParams;

void cleanupAddressTypeParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        AddressTypeParams * tup = (AddressTypeParams *) ctp->params + i;

        if (tup->sock) {
            cr_free(tup->sock);
        }
    }

    cr_free(ctp->params);
}

#define ADDRESS_TYPE_PARAMS                                                                                 \
    const size_t size = 6;                                                                                  \
    AddressTypeParams * params = cr_malloc(sizeof(AddressTypeParams) * size);                               \
                                                                                                            \
    params[0] = (AddressTypeParams) {.sock = NULL, .addressType = UNSPEC};                                  \
    params[1] = (AddressTypeParams) {.sock = NULL, .addressType = IPv4};                                    \
    params[2] = (AddressTypeParams) {.sock = NULL, .addressType = IPv6};                                    \
    params[3] = (AddressTypeParams) {.sock = createSocket(), .addressType = UNSPEC};                        \
    params[4] = (AddressTypeParams) {.sock = createSocket(), .addressType = IPv4};                          \
    params[5] = (AddressTypeParams) {.sock = createSocket(), .addressType = IPv6};                          \
                                                                                                            \
    return cr_make_param_array(AddressTypeParams, params, size, cleanupAddressTypeParam)

ParameterizedTestParameters(address_type, set) {
    ADDRESS_TYPE_PARAMS;
}

ParameterizedTest(AddressTypeParams * param, address_type, set) {
    cr_log_info("Address type set test with {sock: %p, addressType: %d}\n", param->sock, param->addressType);
    netSetAddressType(param->sock, param->addressType);

    if (param->sock) {
        cr_assert_eq(param->sock->addressType, param->addressType, "Set address type did not set the correct type");
    }
}

ParameterizedTestParameters(address_type, get) {
    ADDRESS_TYPE_PARAMS;
}

ParameterizedTest(AddressTypeParams * param, address_type, get) {
    cr_log_info("Address type get test with {sock: %p, addressType: %d}\n", param->sock, param->addressType);

    if (param->sock) {
        param->sock->addressType = param->addressType;
        cr_assert_eq(netGetAddressType(param->sock), param->addressType, "Returned address type is not equal to the expected type");
    } else {
        cr_assert_eq(netGetAddressType(param->sock), UNSPEC, "Null socket did not return unspecified address type");
    }
}
