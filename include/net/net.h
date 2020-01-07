#ifndef NET_NET
#define NET_NET

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// A socket type.
///
/// This contains any of the necessary information to run a socket. It is
/// implemented depending on the platform to contain any variables for
/// that specific platform.
struct net_socket_t;
typedef struct net_socket_t * const NetSocket;

// Defined in request.h
struct net_req_t;

// Defined in response.h
struct net_res_t;

/// A net implementation.
///
/// This is a platform specific implementation of the necessary methods to
/// perform networking. You create this object to automatically choose an
/// implementation based on the current platform using the `NetSetup` function.
struct net_impl_t {
    NetSocket net;
    void (*setAddress)(NetSocket, char const *);
    void (*setPort)(NetSocket, unsigned);
    void (*open)(NetSocket);
    bool (*listen)(NetSocket);
    struct net_req_t const * (*getRequest)(NetSocket);
    void (*setResponse)(NetSocket, struct net_res_t * const);
    void (*close)(NetSocket);
};
typedef struct net_impl_t * NetImpl;

/// Setup a networking implementation.
///
/// This will check the current platform and create a `net_impl_t` struct that
/// implements the necessary methods based on the proper platform.
NetImpl NetSetup();

void NetSetAddress(NetImpl, char const *);

void NetSetPort(NetImpl, unsigned);

void NetOpen(NetImpl);

bool NetListen(NetImpl);

struct net_req_t const * NetGetRequest(NetImpl);

void NetSetResponse(NetImpl, struct net_res_t * const);

void NetClose(NetImpl);

void NetDispose(NetImpl);

#ifdef __cplusplus
}
#endif

#endif
