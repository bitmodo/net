#ifndef NET_REQUEST
#define NET_REQUEST

#include <stddef.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

// Defined in net.h
struct net_socket_t;

struct net_req_t;
typedef struct net_req_t const * NetRequest;

struct net_req_handler_t {
    ssize_t (*read)(NetRequest, void *, size_t);
};

ssize_t NetRequestRead(void *, size_t);

ssize_t NetRequestReadSocket(struct net_socket_t *, void *, size_t);

#ifdef __cplusplus
}
#endif

#endif
