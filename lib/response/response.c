#include "net/response.h"

void NetResponseSetBody(NetResponse res, char const * body) {
    res->body = body;
}
