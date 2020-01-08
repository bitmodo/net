#ifndef NET_NET
#define NET_NET

#include <stdbool.h>
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

/// A socket type.
///
/// This contains any of the necessary information to run a socket. It is
/// implemented depending on the platform to contain any variables for
/// that specific platform.
struct net_data_t;

// Defined in request.h
struct net_req_handler_t;
struct net_req_t;

// Defined in response.h
struct net_res_t;

/// A net implementation.
///
/// This is a platform specific implementation of the necessary methods to
/// perform networking. You create this object to automatically choose an
/// implementation based on the current platform using the `NetSetup` function.
struct net_socket_t {
    struct net_data_t * data;

    struct net_req_handler_t * req_handler;

    void (* setAddress)(struct net_data_t * const, char const *);

    void (* setPort)(struct net_data_t * const, unsigned);

    char const * (* getAddress)(struct net_data_t * const);

    unsigned (* getPort)(struct net_data_t * const);

    int (* open)(struct net_data_t * const);

    bool (* listen)(struct net_data_t * const);

    struct net_req_t const * (* getRequest)(struct net_data_t * const);

    struct net_res_t * (* getResponse)(struct net_data_t * const);

    void (* finalizeResponse)(struct net_data_t * const);

    void (* close)(struct net_data_t * const);

    void (* disposeData)(struct net_data_t * const);

    void (* disposeRequest)(struct net_req_t * const);
};

typedef struct net_socket_t * NetSocket;

static NetSocket net_global_socket;

NET_EXPORT int NetSetup();

/// Setup a networking implementation.
///
/// This will check the current platform and create a `net_impl_t` struct that
/// implements the necessary methods based on the proper platform.
NET_EXPORT NetSocket NetSetupSocket();

NET_EXPORT void NetSetAddress(char const *);

NET_EXPORT void NetSetAddressSocket(NetSocket, char const *);

NET_EXPORT void NetSetPort(unsigned);

NET_EXPORT void NetSetPortSocket(NetSocket, unsigned);

NET_EXPORT char const * NetGetAddress();

NET_EXPORT char const * NetGetAddressSocket(NetSocket);

NET_EXPORT unsigned NetGetPort();

NET_EXPORT unsigned NetGetPortSocket(NetSocket);

NET_EXPORT int NetOpen();

NET_EXPORT int NetOpenSocket(NetSocket);

NET_EXPORT bool NetListen();

NET_EXPORT bool NetListenSocket(NetSocket);

NET_EXPORT struct net_req_t const * NetGetRequest();

NET_EXPORT struct net_req_t const * NetGetRequestSocket(NetSocket);

NET_EXPORT struct net_res_t * NetGetResponse();

NET_EXPORT struct net_res_t * NetGetResponseSocket(NetSocket);

NET_EXPORT void NetFinalizeResponse();

NET_EXPORT void NetFinalizeResponseSocket(NetSocket);

NET_EXPORT void NetClose();

NET_EXPORT void NetCloseSocket(NetSocket);

NET_EXPORT void NetDispose();

NET_EXPORT void NetDisposeSocket(NetSocket);

NET_EXPORT void NetDisposeRequest(struct net_req_t *);

NET_EXPORT void NetDisposeRequestSocket(NetSocket, struct net_req_t *);

#ifdef __cplusplus
}
#endif

#endif
