#include <stdlib.h>
#include "net/net.h"

void NetSetAddress(NetImpl impl, char const * address) {
    if (impl && impl->net) {
        impl->setAddress(impl->net, address);
    }
}

void NetSetPort(NetImpl impl, unsigned port) {
    if (impl && impl->net) {
        impl->setPort(impl->net, port);
    }
}

void NetOpen(NetImpl impl) {
    if (impl && impl->net) {
        impl->open(impl->net);
    }
}

bool NetListen(NetImpl impl) {
    if (impl && impl->net) {
        return impl->listen(impl->net);
    } else {
        return false;
    }
}

struct net_req_t const * NetGetRequest(NetImpl impl) {
    if (impl && impl->net) {
        return impl->getRequest(impl->net);
    } else {
        return NULL;
    }
}

void NetSetResponse(NetImpl impl, struct net_res_t * const res) {
    if (impl && impl->net) {
        impl->setResponse(impl->net, res);
    }
}

void NetClose(NetImpl impl) {
    if (impl && impl->net) {
        impl->close(impl->net);
    }
}

void NetDispose(NetImpl impl) {
    if (impl) {
        if (impl->net) {
            free(impl->net);
        }

        free(impl);
        impl = NULL;
    }
}
