#include "net/net.h"

#include <assert.h>
#include <stdlib.h>

extern NetHandler * gHandler;

int main() {
    gHandler = NULL;
    netCleanup();
    assert(gHandler == NULL && "Handler was not null after cleanup");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    netCleanup();
    assert(gHandler == NULL && "Handler was not null after setting it then cleanup");

    exit(EXIT_SUCCESS);
}
