#ifndef NET_RESPONSE
#define NET_RESPONSE

#ifdef __cplusplus
extern "C" {
#endif

struct net_res_t {
    char const* body;
};

void NetResponseSetBody(net_res_t * const, char const*);

#ifdef __cplusplus
}
#endif

#endif