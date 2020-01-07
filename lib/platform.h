#ifndef NET_PLATFORM
#define NET_PLATFORM

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#error "Windows is not supported yet"
#elif defined(__APPLE__)
#error "Apple is not supported yet"
#elif defined(__linux__)

#else
#warning "Not compiling with any platform code, because no supported platform is being compiled for"
#endif

#ifdef __cplusplus
}
#endif

#endif