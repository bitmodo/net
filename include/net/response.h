#ifndef NET_RESPONSE
#define NET_RESPONSE

#ifdef __cplusplus
extern "C" {
#endif

struct net_res_t {
    char const * body;
};
typedef struct net_res_t * NetResponse;

void NetResponseSetBody(NetResponse, char const *);

#ifdef __cplusplus
}
#endif

#endif