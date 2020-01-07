#ifndef NET_REQUEST
#define NET_REQUEST

#ifdef __cplusplus
extern "C" {
#endif

struct net_req_t {
    char const* body;
};

char const* NetRequestGetBody(net_req_t const*);

#ifdef __cplusplus
}
#endif

#endif