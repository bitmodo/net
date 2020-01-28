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
    AddressParams *params = cr_malloc(sizeof(AddressParams) * size);                                        \
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

// Address type get and set tests

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
    AddressTypeParams *params = cr_malloc(sizeof(AddressTypeParams) * size);                                \
                                                                                                            \
    params[0] = (AddressTypeParams) {.sock = NULL, .addressType = UNSPEC};                                  \
    params[1] = (AddressTypeParams) {.sock = NULL, .addressType = IPv4};                                    \
    params[2] = (AddressTypeParams) {.sock = NULL, .addressType = IPv6};                                    \
    params[3] = (AddressTypeParams) {.sock = createSocket(), .addressType = UNSPEC};                        \
    params[4] = (AddressTypeParams) {.sock = createSocket(), .addressType = IPv4};                          \
    params[5] = (AddressTypeParams) {.sock = createSocket(), .addressType = IPv6};                          \
                                                                                                            \
    return cr_make_param_array(AddressTypeParams, params, size, cleanupAddressParam)

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

// Port get and set tests

typedef struct PortParams {
    Socket * sock;
    int port;
} PortParams;

void cleanupPortParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        PortParams * tup = (PortParams *) ctp->params + i;

        if (tup->sock) {
            cr_free(tup->sock);
        }
    }

    cr_free(ctp->params);
}

#define PORT_PARAMS                                                                                         \
    const size_t size = 6;                                                                                  \
    PortParams *params = cr_malloc(sizeof(PortParams) * size);                                              \
                                                                                                            \
    params[0] = (PortParams) {.sock = NULL, .port = 0};                                                     \
    params[1] = (PortParams) {.sock = NULL, .port = DEFAULT_PORT};                                          \
    params[2] = (PortParams) {.sock = NULL, .port = 80};                                                    \
    params[3] = (PortParams) {.sock = createSocket(), .port = 0};                                           \
    params[4] = (PortParams) {.sock = createSocket(), .port = DEFAULT_PORT};                                \
    params[5] = (PortParams) {.sock = createSocket(), .port = 80};                                          \
                                                                                                            \
    return cr_make_param_array(PortParams, params, size, cleanupAddressParam)

ParameterizedTestParameters(port, set) {
    PORT_PARAMS;
}

ParameterizedTest(PortParams * param, port, set) {
    cr_log_info("Port set test with {sock: %p, port: %d}\n", param->sock, param->port);
    netSetPort(param->sock, param->port);

    if (param->sock) {
        cr_assert_eq(param->sock->port, param->port, "Set port did not set the correct port");
    }
}

ParameterizedTestParameters(port, get) {
    PORT_PARAMS;
}

ParameterizedTest(PortParams * param, port, get) {
    cr_log_info("Port get test with {sock: %p, port: %d}\n", param->sock, param->port);

    if (param->sock) {
        param->sock->port = param->port;
        cr_assert_eq(netGetPort(param->sock), param->port, "Returned port is not equal to the expected port");
    } else {
        cr_assert_eq(netGetPort(param->sock), DEFAULT_PORT, "Null socket did not return the default port");
    }
}
