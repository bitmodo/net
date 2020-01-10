#ifndef NET_UTIL
#define NET_UTIL

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
    // Microsoft
    #define NET_EXPORT __declspec(dllexport)
    #define NET_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    // GCC
    #define NET_EXPORT __attribute__((visibility("default")))
    #define NET_IMPORT
#else
    // do nothing and hope for the best?
    #define NET_EXPORT
    #define NET_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef __cplusplus
}
#endif

#endif
