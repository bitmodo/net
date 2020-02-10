/* Net util header
 * This header file contains some macros that are used throughout the net
 * project. Most, if not all of the headers defined in this file are taken
 * from Hedley (https://nemequ.github.io/hedley/). Note that the "HEDLEY"
 * prefix has been removed from the macros, and replaced with "NET".
 */

#if !defined(NET_UTIL_VERSION) || (NET_UTIL_VERSION < 1)
#if defined(NET_UTIL_VERSION)
#  undef NET_UTIL_VERSION
#endif
#define NET_UTIL_VERSION 1

// Version macros
// These macros help with determining what version compiler we are using.
// That information can be used to determine what features are available
// for us to use.

// Version encoding utility
// This macro allows us to specify a major, minor, and revision number,
// which are encoded into a single number and can be compared to other
// encoded versions.
#if defined(NET_VERSION_ENCODE)
#  undef NET_VERSION_ENCODE
#endif
#define NET_VERSION_ENCODE(major,minor,revision) (((major) * 1000000) + ((minor) * 1000) + (revision))

// Gnu version checks

#if defined(NET_GNUC_VERSION)
#  undef NET_GNUC_VERSION
#endif
#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#  define NET_GNUC_VERSION NET_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#  define NET_GNUC_VERSION NET_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(NET_GNUC_VERSION_CHECK)
#  undef NET_GNUC_VERSION_CHECK
#endif
#if defined(NET_GNUC_VERSION)
#  define NET_GNUC_VERSION_CHECK(major,minor,patch) (NET_GNUC_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_GNUC_VERSION_CHECK(major,minor,patch) (0)
#endif

// MSVC version checks

#if defined(NET_MSVC_VERSION)
#  undef NET_MSVC_VERSION
#endif
#if defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140000000)
#  define NET_MSVC_VERSION NET_VERSION_ENCODE(_MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000, (_MSC_FULL_VER % 100000) / 100)
#elif defined(_MSC_FULL_VER)
#  define NET_MSVC_VERSION NET_VERSION_ENCODE(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#elif defined(_MSC_VER)
#  define NET_MSVC_VERSION NET_VERSION_ENCODE(_MSC_VER / 100, _MSC_VER % 100, 0)
#endif

#if defined(NET_MSVC_VERSION_CHECK)
#  undef NET_MSVC_VERSION_CHECK
#endif
#if !defined(_MSC_VER)
#  define NET_MSVC_VERSION_CHECK(major,minor,patch) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define NET_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_FULL_VER >= ((major * 10000000) + (minor * 100000) + (patch)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define NET_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000) + (patch)))
#else
#  define NET_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_VER >= ((major * 100) + (minor)))
#endif

// Intel version checks

#if defined(NET_INTEL_VERSION)
#  undef NET_INTEL_VERSION
#endif
#if defined(__INTEL_COMPILER) && defined(__INTEL_COMPILER_UPDATE)
#  define NET_INTEL_VERSION NET_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE)
#elif defined(__INTEL_COMPILER)
#  define NET_INTEL_VERSION NET_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, 0)
#endif

#if defined(NET_INTEL_VERSION_CHECK)
#  undef NET_INTEL_VERSION_CHECK
#endif
#if defined(NET_INTEL_VERSION)
#  define NET_INTEL_VERSION_CHECK(major,minor,patch) (NET_INTEL_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_INTEL_VERSION_CHECK(major,minor,patch) (0)
#endif

// PGI version checks

#if defined(NET_PGI_VERSION)
#  undef NET_PGI_VERSION
#endif
#if defined(__PGI) && defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__)
#  define NET_PGI_VERSION NET_VERSION_ENCODE(__PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__)
#endif

#if defined(NET_PGI_VERSION_CHECK)
#  undef NET_PGI_VERSION_CHECK
#endif
#if defined(NET_PGI_VERSION)
#  define NET_PGI_VERSION_CHECK(major,minor,patch) (NET_PGI_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_PGI_VERSION_CHECK(major,minor,patch) (0)
#endif

// Oracle version

#if defined(NET_SUNPRO_VERSION)
#  undef NET_SUNPRO_VERSION
#endif
#if defined(__SUNPRO_C) && (__SUNPRO_C > 0x1000)
#  define NET_SUNPRO_VERSION NET_VERSION_ENCODE((((__SUNPRO_C >> 16) & 0xf) * 10) + ((__SUNPRO_C >> 12) & 0xf), (((__SUNPRO_C >> 8) & 0xf) * 10) + ((__SUNPRO_C >> 4) & 0xf), (__SUNPRO_C & 0xf) * 10)
#elif defined(__SUNPRO_C)
#  define NET_SUNPRO_VERSION NET_VERSION_ENCODE((__SUNPRO_C >> 8) & 0xf, (__SUNPRO_C >> 4) & 0xf, (__SUNPRO_C) & 0xf)
#elif defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x1000)
#  define NET_SUNPRO_VERSION NET_VERSION_ENCODE((((__SUNPRO_CC >> 16) & 0xf) * 10) + ((__SUNPRO_CC >> 12) & 0xf), (((__SUNPRO_CC >> 8) & 0xf) * 10) + ((__SUNPRO_CC >> 4) & 0xf), (__SUNPRO_CC & 0xf) * 10)
#elif defined(__SUNPRO_CC)
#  define NET_SUNPRO_VERSION NET_VERSION_ENCODE((__SUNPRO_CC >> 8) & 0xf, (__SUNPRO_CC >> 4) & 0xf, (__SUNPRO_CC) & 0xf)
#endif

#if defined(NET_SUNPRO_VERSION_CHECK)
#  undef NET_SUNPRO_VERSION_CHECK
#endif
#if defined(NET_SUNPRO_VERSION)
#  define NET_SUNPRO_VERSION_CHECK(major,minor,patch) (NET_SUNPRO_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_SUNPRO_VERSION_CHECK(major,minor,patch) (0)
#endif

// Emscripten version checks

#if defined(NET_EMSCRIPTEN_VERSION)
#  undef NET_EMSCRIPTEN_VERSION
#endif
#if defined(__EMSCRIPTEN__)
#  define NET_EMSCRIPTEN_VERSION NET_VERSION_ENCODE(__EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__)
#endif

#if defined(NET_EMSCRIPTEN_VERSION_CHECK)
#  undef NET_EMSCRIPTEN_VERSION_CHECK
#endif
#if defined(NET_EMSCRIPTEN_VERSION)
#  define NET_EMSCRIPTEN_VERSION_CHECK(major,minor,patch) (NET_EMSCRIPTEN_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_EMSCRIPTEN_VERSION_CHECK(major,minor,patch) (0)
#endif

// ARM version checks

#if defined(NET_ARM_VERSION)
#  undef NET_ARM_VERSION
#endif
#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
#  define NET_ARM_VERSION NET_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000, (__ARMCOMPILER_VERSION % 1000000) / 10000, (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
#  define NET_ARM_VERSION NET_VERSION_ENCODE(__ARMCC_VERSION / 1000000, (__ARMCC_VERSION % 1000000) / 10000, (__ARMCC_VERSION % 10000) / 100)
#endif

#if defined(NET_ARM_VERSION_CHECK)
#  undef NET_ARM_VERSION_CHECK
#endif
#if defined(NET_ARM_VERSION)
#  define NET_ARM_VERSION_CHECK(major,minor,patch) (NET_ARM_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_ARM_VERSION_CHECK(major,minor,patch) (0)
#endif

// IBM XL version checks

#if defined(NET_IBM_VERSION)
#  undef NET_IBM_VERSION
#endif
#if defined(__ibmxl__)
#  define NET_IBM_VERSION NET_VERSION_ENCODE(__ibmxl_version__, __ibmxl_release__, __ibmxl_modification__)
#elif defined(__xlC__) && defined(__xlC_ver__)
#  define NET_IBM_VERSION NET_VERSION_ENCODE(__xlC__ >> 8, __xlC__ & 0xff, (__xlC_ver__ >> 8) & 0xff)
#elif defined(__xlC__)
#  define NET_IBM_VERSION NET_VERSION_ENCODE(__xlC__ >> 8, __xlC__ & 0xff, 0)
#endif

#if defined(NET_IBM_VERSION_CHECK)
#  undef NET_IBM_VERSION_CHECK
#endif
#if defined(NET_IBM_VERSION)
#  define NET_IBM_VERSION_CHECK(major,minor,patch) (NET_IBM_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_IBM_VERSION_CHECK(major,minor,patch) (0)
#endif

// TI version checks

#if defined(NET_TI_VERSION)
#  undef NET_TI_VERSION
#endif
#if \
    defined(__TI_COMPILER_VERSION__) && \
    ( \
      defined(__TMS470__) || defined(__TI_ARM__) || \
      defined(__MSP430__) || \
      defined(__TMS320C2000__) \
    )
#  if (__TI_COMPILER_VERSION__ >= 16000000)
#    define NET_TI_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#  endif
#endif

#if defined(NET_TI_VERSION_CHECK)
#  undef NET_TI_VERSION_CHECK
#endif
#if defined(NET_TI_VERSION)
#  define NET_TI_VERSION_CHECK(major,minor,patch) (NET_TI_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(NET_TI_CL2000_VERSION)
#  undef NET_TI_CL2000_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C2000__)
#  define NET_TI_CL2000_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(NET_TI_CL2000_VERSION_CHECK)
#  undef NET_TI_CL2000_VERSION_CHECK
#endif
#if defined(NET_TI_CL2000_VERSION)
#  define NET_TI_CL2000_VERSION_CHECK(major,minor,patch) (NET_TI_CL2000_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_CL2000_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(NET_TI_CL430_VERSION)
#  undef NET_TI_CL430_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__MSP430__)
#  define NET_TI_CL430_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(NET_TI_CL430_VERSION_CHECK)
#  undef NET_TI_CL430_VERSION_CHECK
#endif
#if defined(NET_TI_CL430_VERSION)
#  define NET_TI_CL430_VERSION_CHECK(major,minor,patch) (NET_TI_CL430_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_CL430_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(NET_TI_ARMCL_VERSION)
#  undef NET_TI_ARMCL_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && (defined(__TMS470__) || defined(__TI_ARM__))
#  define NET_TI_ARMCL_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(NET_TI_ARMCL_VERSION_CHECK)
#  undef NET_TI_ARMCL_VERSION_CHECK
#endif
#if defined(NET_TI_ARMCL_VERSION)
#  define NET_TI_ARMCL_VERSION_CHECK(major,minor,patch) (NET_TI_ARMCL_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_ARMCL_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(NET_TI_CL6X_VERSION)
#  undef NET_TI_CL6X_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C6X__)
#  define NET_TI_CL6X_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(NET_TI_CL6X_VERSION_CHECK)
#  undef NET_TI_CL6X_VERSION_CHECK
#endif
#if defined(NET_TI_CL6X_VERSION)
#  define NET_TI_CL6X_VERSION_CHECK(major,minor,patch) (NET_TI_CL6X_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_CL6X_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(NET_TI_CL7X_VERSION)
#  undef NET_TI_CL7X_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__C7000__)
#  define NET_TI_CL7X_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(NET_TI_CL7X_VERSION_CHECK)
#  undef NET_TI_CL7X_VERSION_CHECK
#endif
#if defined(NET_TI_CL7X_VERSION)
#  define NET_TI_CL7X_VERSION_CHECK(major,minor,patch) (NET_TI_CL7X_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_CL7X_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(NET_TI_CLPRU_VERSION)
#  undef NET_TI_CLPRU_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__PRU__)
#  define NET_TI_CLPRU_VERSION NET_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(NET_TI_CLPRU_VERSION_CHECK)
#  undef NET_TI_CLPRU_VERSION_CHECK
#endif
#if defined(NET_TI_CLPRU_VERSION)
#  define NET_TI_CLPRU_VERSION_CHECK(major,minor,patch) (NET_TI_CLPRU_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TI_CLPRU_VERSION_CHECK(major,minor,patch) (0)
#endif

// Cray version checks

#if defined(NET_CRAY_VERSION)
#  undef NET_CRAY_VERSION
#endif
#if defined(_CRAYC)
#  if defined(_RELEASE_PATCHLEVEL)
#    define NET_CRAY_VERSION NET_VERSION_ENCODE(_RELEASE_MAJOR, _RELEASE_MINOR, _RELEASE_PATCHLEVEL)
#  else
#    define NET_CRAY_VERSION NET_VERSION_ENCODE(_RELEASE_MAJOR, _RELEASE_MINOR, 0)
#  endif
#endif

#if defined(NET_CRAY_VERSION_CHECK)
#  undef NET_CRAY_VERSION_CHECK
#endif
#if defined(NET_CRAY_VERSION)
#  define NET_CRAY_VERSION_CHECK(major,minor,patch) (NET_CRAY_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_CRAY_VERSION_CHECK(major,minor,patch) (0)
#endif

// IAR version checks

#if defined(NET_IAR_VERSION)
#  undef NET_IAR_VERSION
#endif
#if defined(__IAR_SYSTEMS_ICC__)
#  if __VER__ > 1000
#    define NET_IAR_VERSION NET_VERSION_ENCODE((__VER__ / 1000000), ((__VER__ / 1000) % 1000), (__VER__ % 1000))
#  else
#    define NET_IAR_VERSION NET_VERSION_ENCODE(VER / 100, __VER__ % 100, 0)
#  endif
#endif

#if defined(NET_IAR_VERSION_CHECK)
#  undef NET_IAR_VERSION_CHECK
#endif
#if defined(NET_IAR_VERSION)
#  define NET_IAR_VERSION_CHECK(major,minor,patch) (NET_IAR_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_IAR_VERSION_CHECK(major,minor,patch) (0)
#endif

// TinyC version checks

#if defined(NET_TINYC_VERSION)
#  undef NET_TINYC_VERSION
#endif
#if defined(__TINYC__)
#  define NET_TINYC_VERSION NET_VERSION_ENCODE(__TINYC__ / 1000, (__TINYC__ / 100) % 10, __TINYC__ % 100)
#endif

#if defined(NET_TINYC_VERSION_CHECK)
#  undef NET_TINYC_VERSION_CHECK
#endif
#if defined(NET_TINYC_VERSION)
#  define NET_TINYC_VERSION_CHECK(major,minor,patch) (NET_TINYC_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_TINYC_VERSION_CHECK(major,minor,patch) (0)
#endif

// DMC version checks

#if defined(NET_DMC_VERSION)
#  undef NET_DMC_VERSION
#endif
#if defined(__DMC__)
#  define NET_DMC_VERSION NET_VERSION_ENCODE(__DMC__ >> 8, (__DMC__ >> 4) & 0xf, __DMC__ & 0xf)
#endif

#if defined(NET_DMC_VERSION_CHECK)
#  undef NET_DMC_VERSION_CHECK
#endif
#if defined(NET_DMC_VERSION)
#  define NET_DMC_VERSION_CHECK(major,minor,patch) (NET_DMC_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_DMC_VERSION_CHECK(major,minor,patch) (0)
#endif

// Compcert version checks

#if defined(NET_COMPCERT_VERSION)
#  undef NET_COMPCERT_VERSION
#endif
#if defined(__COMPCERT_VERSION__)
#  define NET_COMPCERT_VERSION NET_VERSION_ENCODE(__COMPCERT_VERSION__ / 10000, (__COMPCERT_VERSION__ / 100) % 100, __COMPCERT_VERSION__ % 100)
#endif

#if defined(NET_COMPCERT_VERSION_CHECK)
#  undef NET_COMPCERT_VERSION_CHECK
#endif
#if defined(NET_COMPCERT_VERSION)
#  define NET_COMPCERT_VERSION_CHECK(major,minor,patch) (NET_COMPCERT_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_COMPCERT_VERSION_CHECK(major,minor,patch) (0)
#endif

// Pelles version checks

#if defined(NET_PELLES_VERSION)
#  undef NET_PELLES_VERSION
#endif
#if defined(__POCC__)
#  define NET_PELLES_VERSION NET_VERSION_ENCODE(__POCC__ / 100, __POCC__ % 100, 0)
#endif

#if defined(NET_PELLES_VERSION_CHECK)
#  undef NET_PELLES_VERSION_CHECK
#endif
#if defined(NET_PELLES_VERSION)
#  define NET_PELLES_VERSION_CHECK(major,minor,patch) (NET_PELLES_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_PELLES_VERSION_CHECK(major,minor,patch) (0)
#endif

// Gcc version checks

#if defined(NET_GCC_VERSION)
#  undef NET_GCC_VERSION
#endif
#if \
  defined(NET_GNUC_VERSION) && \
  !defined(__clang__) && \
  !defined(NET_INTEL_VERSION) && \
  !defined(NET_PGI_VERSION) && \
  !defined(NET_ARM_VERSION) && \
  !defined(NET_TI_VERSION) && \
  !defined(NET_TI_ARMCL_VERSION) && \
  !defined(NET_TI_CL430_VERSION) && \
  !defined(NET_TI_CL2000_VERSION) && \
  !defined(NET_TI_CL6X_VERSION) && \
  !defined(NET_TI_CL7X_VERSION) && \
  !defined(NET_TI_CLPRU_VERSION) && \
  !defined(__COMPCERT__)
#  define NET_GCC_VERSION NET_GNUC_VERSION
#endif

#if defined(NET_GCC_VERSION_CHECK)
#  undef NET_GCC_VERSION_CHECK
#endif
#if defined(NET_GCC_VERSION)
#  define NET_GCC_VERSION_CHECK(major,minor,patch) (NET_GCC_VERSION >= NET_VERSION_ENCODE(major, minor, patch))
#else
#  define NET_GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

// Compiler feature testing macros
// These are some macros to test the actual features of the current compiler.
// This is not to be mistaken for compiler version checking macros. Instead
// of simply getting the version of the compiler, these macros actually check
// what features are available on the current compiler.

// Check if the compiler has an attribute
// This macro can be used to check if a specific attribute is supported by the
// compiler. At its base level, this macro doesn't have specific attribute
// checking ability depending on the compiler. Compiler-specific attribute
// checking macros can be used to check for attributes on specific compilers.
#if defined(NET_HAS_ATTRIBUTE)
#  undef NET_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#  define NET_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
#  define NET_HAS_ATTRIBUTE(attribute) (0)
#endif

// Gnu attribute check

#if defined(NET_GNUC_HAS_ATTRIBUTE)
#  undef NET_GNUC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#  define NET_GNUC_HAS_ATTRIBUTE(attribute,major,minor,patch) __has_attribute(attribute)
#else
#  define NET_GNUC_HAS_ATTRIBUTE(attribute,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc attribute check

#if defined(NET_GCC_HAS_ATTRIBUTE)
#  undef NET_GCC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#  define NET_GCC_HAS_ATTRIBUTE(attribute,major,minor,patch) __has_attribute(attribute)
#else
#  define NET_GCC_HAS_ATTRIBUTE(attribute,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// C++ attribute check

#if defined(NET_HAS_CPP_ATTRIBUTE)
#  undef NET_HAS_CPP_ATTRIBUTE
#endif
#if \
  defined(__has_cpp_attribute) && \
  defined(__cplusplus) && \
  (!defined(NET_SUNPRO_VERSION) || NET_SUNPRO_VERSION_CHECK(5,15,0))
#  define NET_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
#  define NET_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif

// C++ attribute w/ a namespace check

#if defined(NET_HAS_CPP_ATTRIBUTE_NS)
#  undef NET_HAS_CPP_ATTRIBUTE_NS
#endif
#if !defined(__cplusplus) || !defined(__has_cpp_attribute)
#  define NET_HAS_CPP_ATTRIBUTE_NS(ns,attribute) (0)
#elif \
  !defined(NET_PGI_VERSION) && \
  !defined(NET_IAR_VERSION) && \
  (!defined(NET_SUNPRO_VERSION) || NET_SUNPRO_VERSION_CHECK(5,15,0)) && \
  (!defined(NET_MSVC_VERSION) || NET_MSVC_VERSION_CHECK(19,20,0))
#  define NET_HAS_CPP_ATTRIBUTE_NS(ns,attribute) NET_HAS_CPP_ATTRIBUTE(ns::attribute)
#else
#  define NET_HAS_CPP_ATTRIBUTE_NS(ns,attribute) (0)
#endif

// Gnu C++ attribute check

#if defined(NET_GNUC_HAS_CPP_ATTRIBUTE)
#  undef NET_GNUC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#  define NET_GNUC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) __has_cpp_attribute(attribute)
#else
#  define NET_GNUC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc C++ attribute check

#if defined(NET_GCC_HAS_CPP_ATTRIBUTE)
#  undef NET_GCC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#  define NET_GCC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) __has_cpp_attribute(attribute)
#else
#  define NET_GCC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// Check if a builtin function is specified
// This will check if a builtin function is available to be used.
// Similar to the attribute check, this macro doesn't actually have
// a compiler-specific implementation. Use compiler specific macros
// to check for specific compilers.
#if defined(NET_HAS_BUILTIN)
#  undef NET_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#  define NET_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#  define NET_HAS_BUILTIN(builtin) (0)
#endif

// Gnu builtin check

#if defined(NET_GNUC_HAS_BUILTIN)
#  undef NET_GNUC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#  define NET_GNUC_HAS_BUILTIN(builtin,major,minor,patch) __has_builtin(builtin)
#else
#  define NET_GNUC_HAS_BUILTIN(builtin,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc builtin check

#if defined(NET_GCC_HAS_BUILTIN)
#  undef NET_GCC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#  define NET_GCC_HAS_BUILTIN(builtin,major,minor,patch) __has_builtin(builtin)
#else
#  define NET_GCC_HAS_BUILTIN(builtin,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// Check if a feature is supported
// This will check if a feature is supported and standardized. Like
// other feature checks, this is not compiler dependant.
#if defined(NET_HAS_FEATURE)
#  undef NET_HAS_FEATURE
#endif
#if defined(__has_feature)
#  define NET_HAS_FEATURE(feature) __has_feature(feature)
#else
#  define NET_HAS_FEATURE(feature) (0)
#endif

// Gnu feature check

#if defined(NET_GNUC_HAS_FEATURE)
#  undef NET_GNUC_HAS_FEATURE
#endif
#if defined(__has_feature)
#  define NET_GNUC_HAS_FEATURE(feature,major,minor,patch) __has_feature(feature)
#else
#  define NET_GNUC_HAS_FEATURE(feature,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc feature check

#if defined(NET_GCC_HAS_FEATURE)
#  undef NET_GCC_HAS_FEATURE
#endif
#if defined(__has_feature)
#  define NET_GCC_HAS_FEATURE(feature,major,minor,patch) __has_feature(feature)
#else
#  define NET_GCC_HAS_FEATURE(feature,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// Check if an extension is present
// This will check if the feature is supported by the compiler, either
// as an extension or language feature. Like other feature checks, this
// is not compiler dependant.
#if defined(NET_HAS_EXTENSION)
#  undef NET_HAS_EXTENSION
#endif
#if defined(__has_extension)
#  define NET_HAS_EXTENSION(extension) __has_extension(extension)
#else
#  define NET_HAS_EXTENSION(extension) (0)
#endif

// Gnu extension check

#if defined(NET_GNUC_HAS_EXTENSION)
#  undef NET_GNUC_HAS_EXTENSION
#endif
#if defined(__has_extension)
#  define NET_GNUC_HAS_EXTENSION(extension,major,minor,patch) __has_extension(extension)
#else
#  define NET_GNUC_HAS_EXTENSION(extension,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc extension check

#if defined(NET_GCC_HAS_EXTENSION)
#  undef NET_GCC_HAS_EXTENSION
#endif
#if defined(__has_extension)
#  define NET_GCC_HAS_EXTENSION(extension,major,minor,patch) __has_extension(extension)
#else
#  define NET_GCC_HAS_EXTENSION(extension,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// Check if a declspec is supported
// This will check to see an attribute is implemented as a
// Microsoft-style attribute. As with other feature checks,
// this is not compiler dependant.
#if defined(NET_HAS_DECLSPEC_ATTRIBUTE)
#  undef NET_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#  define NET_HAS_DECLSPEC_ATTRIBUTE(attribute) __has_declspec_attribute(attribute)
#else
#  define NET_HAS_DECLSPEC_ATTRIBUTE(attribute) (0)
#endif

// Gnu declspec check

#if defined(NET_GNUC_HAS_DECLSPEC_ATTRIBUTE)
#  undef NET_GNUC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#  define NET_GNUC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) __has_declspec_attribute(attribute)
#else
#  define NET_GNUC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc declspec check

#if defined(NET_GCC_HAS_DECLSPEC_ATTRIBUTE)
#  undef NET_GCC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#  define NET_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) __has_declspec_attribute(attribute)
#else
#  define NET_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// Check if the warning is valid
// This takes a command line option argument and determines
// if it is a valid warning option. As with other feature checks,
// this is not compiler dependent.
#if defined(NET_HAS_WARNING)
#  undef NET_HAS_WARNING
#endif
#if defined(__has_warning)
#  define NET_HAS_WARNING(warning) __has_warning(warning)
#else
#  define NET_HAS_WARNING(warning) (0)
#endif

// Gnu warning check

#if defined(NET_GNUC_HAS_WARNING)
#  undef NET_GNUC_HAS_WARNING
#endif
#if defined(__has_warning)
#  define NET_GNUC_HAS_WARNING(warning,major,minor,patch) __has_warning(warning)
#else
#  define NET_GNUC_HAS_WARNING(warning,major,minor,patch) NET_GNUC_VERSION_CHECK(major,minor,patch)
#endif

// Gcc warning check

#if defined(NET_GCC_HAS_WARNING)
#  undef NET_GCC_HAS_WARNING
#endif
#if defined(__has_warning)
#  define NET_GCC_HAS_WARNING(warning,major,minor,patch) __has_warning(warning)
#else
#  define NET_GCC_HAS_WARNING(warning,major,minor,patch) NET_GCC_VERSION_CHECK(major,minor,patch)
#endif

// Disable C++ compat
// This is an internal macro that is not intended to be used externally.
// It helps us do things which would emit warnings/errors easier.
#if defined(NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_)
#  undef NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_
#endif
#if defined(__cplusplus)
#  if NET_HAS_WARNING("-Wc++98-compat")
#    if NET_HAS_WARNING("-Wc++17-extensions")
#      define NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
         NET_DIAGNOSTIC_PUSH \
         _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
         _Pragma("clang diagnostic ignored \"-Wc++17-extensions\"") \
         xpr \
         NET_DIAGNOSTIC_POP
#    else
#      define NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
         NET_DIAGNOSTIC_PUSH \
         _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
         xpr \
         NET_DIAGNOSTIC_POP
#    endif
#  endif
#endif
#if !defined(NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_)
#  define NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(x) x
#endif

// Cast macros

// Perform a static cast
// This will perform a static cast using the specified
// type and expression. The only distinction is that
// on C++, a static_cast is used, whereas in C, a
// regular cast is used.
#if defined(NET_STATIC_CAST)
#  undef NET_STATIC_CAST
#endif
#if defined(__cplusplus)
#  define NET_STATIC_CAST(T, expr) (static_cast<T>(expr))
#else
#  define NET_STATIC_CAST(T, expr) ((T) (expr))
#endif

// Diagnostic macros

// Create a pragma
// This is a simple macro that will create a pragma using the specified
// value. It is useful for when a pragma is to be used in another macro,
// but a macro directive cannot be used.
#if \
  (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
  defined(__clang__) || \
  NET_GCC_VERSION_CHECK(3,0,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_IAR_VERSION_CHECK(8,0,0) || \
  NET_PGI_VERSION_CHECK(18,4,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  NET_TI_ARMCL_VERSION_CHECK(4,7,0) || \
  NET_TI_CL430_VERSION_CHECK(2,0,1) || \
  NET_TI_CL2000_VERSION_CHECK(6,1,0) || \
  NET_TI_CL6X_VERSION_CHECK(7,0,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  NET_CRAY_VERSION_CHECK(5,0,0) || \
  NET_TINYC_VERSION_CHECK(0,9,17) || \
  NET_SUNPRO_VERSION_CHECK(8,0,0) || \
  (NET_IBM_VERSION_CHECK(10,1,0) && defined(__C99_PRAGMA_OPERATOR))
#  define NET_PRAGMA(value) _Pragma(#value)
#elif NET_MSVC_VERSION_CHECK(15,0,0)
#  define NET_PRAGMA(value) __pragma(value)
#else
#  define NET_PRAGMA(value)
#endif

// Push and pop diagnostics
// Save and restore the current diagnostic state so that
// diagnostics can be modified without messing up subsequent
// diagnostics.
#if defined(NET_DIAGNOSTIC_PUSH)
#  undef NET_DIAGNOSTIC_PUSH
#endif
#if defined(NET_DIAGNOSTIC_POP)
#  undef NET_DIAGNOSTIC_POP
#endif
#if defined(__clang__)
#  define NET_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#  define NET_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#elif NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_DIAGNOSTIC_PUSH _Pragma("warning(push)")
#  define NET_DIAGNOSTIC_POP _Pragma("warning(pop)")
#elif NET_GCC_VERSION_CHECK(4,6,0)
#  define NET_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#  define NET_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#elif NET_MSVC_VERSION_CHECK(15,0,0)
#  define NET_DIAGNOSTIC_PUSH __pragma(warning(push))
#  define NET_DIAGNOSTIC_POP __pragma(warning(pop))
#elif NET_ARM_VERSION_CHECK(5,6,0)
#  define NET_DIAGNOSTIC_PUSH _Pragma("push")
#  define NET_DIAGNOSTIC_POP _Pragma("pop")
#elif \
    NET_TI_VERSION_CHECK(15,12,0) || \
    NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    NET_TI_CL430_VERSION_CHECK(4,4,0) || \
    NET_TI_CL6X_VERSION_CHECK(8,1,0) || \
    NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
    NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_DIAGNOSTIC_PUSH _Pragma("diag_push")
#  define NET_DIAGNOSTIC_POP _Pragma("diag_pop")
#elif NET_PELLES_VERSION_CHECK(2,90,0)
#  define NET_DIAGNOSTIC_PUSH _Pragma("warning(push)")
#  define NET_DIAGNOSTIC_POP _Pragma("warning(pop)")
#else
#  define NET_DIAGNOSTIC_PUSH
#  define NET_DIAGNOSTIC_POP
#endif

// Ignore deprecated warnings
// This will turn off the diagnostic which flags the usage
// of deprecated declarations as warnings.
#if defined(NET_DIAGNOSTIC_DISABLE_DEPRECATED)
#  undef NET_DIAGNOSTIC_DISABLE_DEPRECATED
#endif
#if NET_HAS_WARNING("-Wdeprecated-declarations")
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#elif NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("warning(disable:1478 1786)")
#elif NET_PGI_VERSION_CHECK(17,10,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1215,1444")
#elif NET_GCC_VERSION_CHECK(4,3,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif NET_MSVC_VERSION_CHECK(15,0,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED __pragma(warning(disable:4996))
#elif \
    NET_TI_VERSION_CHECK(15,12,0) || \
    (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    NET_TI_CL430_VERSION_CHECK(4,3,0) || \
    (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
    NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
    NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1291,1718")
#elif NET_SUNPRO_VERSION_CHECK(5,13,0) && !defined(__cplusplus)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("error_messages(off,E_DEPRECATED_ATT,E_DEPRECATED_ATT_MESS)")
#elif NET_SUNPRO_VERSION_CHECK(5,13,0) && defined(__cplusplus)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("error_messages(off,symdeprecated,symdeprecated2)")
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress=Pe1444,Pe1215")
#elif NET_PELLES_VERSION_CHECK(2,90,0)
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("warn(disable:2241)")
#else
#  define NET_DIAGNOSTIC_DISABLE_DEPRECATED
#endif

// Ignore unknown pragma warnings
// This will turn off diagnostics which flag the use of unknown
// pragmas as a warning.
#if defined(NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS)
#  undef NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif
#if NET_HAS_WARNING("-Wunknown-pragmas")
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#elif NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("warning(disable:161)")
#elif NET_PGI_VERSION_CHECK(17,10,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 1675")
#elif NET_GCC_VERSION_CHECK(4,3,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#elif NET_MSVC_VERSION_CHECK(15,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS __pragma(warning(disable:4068))
#elif \
    NET_TI_VERSION_CHECK(16,9,0) || \
    NET_TI_CL6X_VERSION_CHECK(8,0,0) || \
    NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
    NET_TI_CLPRU_VERSION_CHECK(2,3,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 163")
#elif NET_TI_CL6X_VERSION_CHECK(8,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 163")
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress=Pe161")
#else
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif

// Ignore unknown attribute warnings
// This will turn off diagnostics which flag the use of
// unkown attributes as a warning.
#if defined(NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES)
#  undef NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES
#endif
#if NET_HAS_WARNING("-Wunknown-attributes")
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("clang diagnostic ignored \"-Wunknown-attributes\"")
#elif NET_GCC_VERSION_CHECK(4,6,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif NET_INTEL_VERSION_CHECK(17,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("warning(disable:1292)")
#elif NET_MSVC_VERSION_CHECK(19,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES __pragma(warning(disable:5030))
#elif NET_PGI_VERSION_CHECK(17,10,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1097")
#elif NET_SUNPRO_VERSION_CHECK(5,14,0) && defined(__cplusplus)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("error_messages(off,attrskipunsup)")
#elif \
    NET_TI_VERSION_CHECK(18,1,0) || \
    NET_TI_CL6X_VERSION_CHECK(8,3,0) || \
    NET_TI_CL7X_VERSION_CHECK(1,2,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1173")
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress=Pe1097")
#else
#  define NET_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES
#endif

// Function macros

// Flag a function as deprecated
// This will flag the function it is annotated on as deprecated.
// A "deprecated for" macro is also defined. It allows a replacement
// function to be specified as the newer version of the deprecated function,
// so that users know what to switch to.
#if defined(NET_DEPRECATED)
#  undef NET_DEPRECATED
#endif
#if defined(NET_DEPRECATED_FOR)
#  undef NET_DEPRECATED_FOR
#endif
#if defined(__cplusplus) && (__cplusplus >= 201402L)
#  define NET_DEPRECATED(since) NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[deprecated("Since " #since)]])
#  define NET_DEPRECATED_FOR(since, replacement) NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[deprecated("Since " #since "; use " #replacement)]])
#elif \
  NET_HAS_EXTENSION(attribute_deprecated_with_message) || \
  NET_GCC_VERSION_CHECK(4,5,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_ARM_VERSION_CHECK(5,6,0) || \
  NET_SUNPRO_VERSION_CHECK(5,13,0) || \
  NET_PGI_VERSION_CHECK(17,10,0) || \
  NET_TI_VERSION_CHECK(18,1,0) || \
  NET_TI_ARMCL_VERSION_CHECK(18,1,0) || \
  NET_TI_CL6X_VERSION_CHECK(8,3,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,3,0)
#  define NET_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
#  define NET_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__("Since " #since "; use " #replacement)))
#elif \
  NET_HAS_ATTRIBUTE(deprecated) || \
  NET_GCC_VERSION_CHECK(3,1,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_DEPRECATED(since) __attribute__((__deprecated__))
#  define NET_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__))
#elif NET_MSVC_VERSION_CHECK(14,0,0)
#  define NET_DEPRECATED(since) __declspec(deprecated("Since " # since))
#  define NET_DEPRECATED_FOR(since, replacement) __declspec(deprecated("Since " #since "; use " #replacement))
#elif \
  NET_MSVC_VERSION_CHECK(13,10,0) || \
  NET_PELLES_VERSION_CHECK(6,50,0)
#  define NET_DEPRECATED(since) __declspec(deprecated)
#  define NET_DEPRECATED_FOR(since, replacement) __declspec(deprecated)
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_DEPRECATED(since) _Pragma("deprecated")
#  define NET_DEPRECATED_FOR(since, replacement) _Pragma("deprecated")
#else
#  define NET_DEPRECATED(since)
#  define NET_DEPRECATED_FOR(since, replacement)
#endif

// Mark a function as unavailable
// Mark a function as unavailable in the API. This means that the function
// is not actually included anymore, and that the declaration is only there
// for historic reasons.
#if defined(NET_UNAVAILABLE)
#  undef NET_UNAVAILABLE
#endif
#if \
  NET_HAS_ATTRIBUTE(warning) || \
  NET_GCC_VERSION_CHECK(4,3,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_UNAVAILABLE(available_since) __attribute__((__warning__("Not available until " #available_since)))
#else
#  define NET_UNAVAILABLE(available_since)
#endif

// Emit a diagnostic if the return value is not checked
// This will signal the compiler to emit a diagnostic if the return value
// is discarded without being checked.
#if defined(NET_WARN_UNUSED_RESULT)
#  undef NET_WARN_UNUSED_RESULT
#endif
#if defined(NET_WARN_UNUSED_RESULT_MSG)
#  undef NET_WARN_UNUSED_RESULT_MSG
#endif
#if (NET_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L)
#  define NET_WARN_UNUSED_RESULT NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#  define NET_WARN_UNUSED_RESULT_MSG(msg) NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard(msg)]])
#elif NET_HAS_CPP_ATTRIBUTE(nodiscard)
#  define NET_WARN_UNUSED_RESULT NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#  define NET_WARN_UNUSED_RESULT_MSG(msg) NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#elif \
  NET_HAS_ATTRIBUTE(warn_unused_result) || \
  NET_GCC_VERSION_CHECK(3,4,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  (NET_SUNPRO_VERSION_CHECK(5,15,0) && defined(__cplusplus)) || \
  NET_PGI_VERSION_CHECK(17,10,0)
#  define NET_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#  define NET_WARN_UNUSED_RESULT_MSG(msg) __attribute__((__warn_unused_result__))
#elif defined(_Check_return_) /* SAL */
#  define NET_WARN_UNUSED_RESULT _Check_return_
#  define NET_WARN_UNUSED_RESULT_MSG(msg) _Check_return_
#else
#  define NET_WARN_UNUSED_RESULT
#  define NET_WARN_UNUSED_RESULT_MSG(msg)
#endif

// Mark a pointer as not escaping
// This is an attribute for pointer parameters to specify that the
// pointer does not escape the function. This means that there will
// not be any new aliases of the pointer after running the function.
// For example, setting a global variable to the pointer parameter would
// escape the pointer.
#if defined(NET_NO_ESCAPE)
#  undef NET_NO_ESCAPE
#endif
#if NET_HAS_ATTRIBUTE(noescape)
#  define NET_NO_ESCAPE __attribute__((__noescape__))
#else
#  define NET_NO_ESCAPE
#endif

// Mark the segment as unreachable or make an assumption
// This will mark some part of code as unreachable for the compiler
// which can allow for specific optimizations. For example, using a
// switch statement with an enumeration could use an unreachable
// statement to signify that a route should never be possible.
// The return variant also returns if no actual failure is supported.
// The assume macro is similar, but instead of specifying that a
// statement is unreachable, it tells the compiler to make an assumption
// that a specific expression should always evaluate to a non-false value.
#if defined(NET_UNREACHABLE)
#  undef NET_UNREACHABLE
#endif
#if defined(NET_UNREACHABLE_RETURN)
#  undef NET_UNREACHABLE_RETURN
#endif
#if defined(NET_ASSUME)
#  undef NET_ASSUME
#endif
#if \
  NET_MSVC_VERSION_CHECK(13,10,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_ASSUME(expr) __assume(expr)
#elif NET_HAS_BUILTIN(__builtin_assume)
#  define NET_ASSUME(expr) __builtin_assume(expr)
#elif \
    NET_TI_CL2000_VERSION_CHECK(6,2,0) || \
    NET_TI_CL6X_VERSION_CHECK(4,0,0)
#  if defined(__cplusplus)
#    define NET_ASSUME(expr) std::_nassert(expr)
#  else
#    define NET_ASSUME(expr) _nassert(expr)
#  endif
#endif
#if \
  (NET_HAS_BUILTIN(__builtin_unreachable) && (!defined(NET_ARM_VERSION))) || \
  NET_GCC_VERSION_CHECK(4,5,0) || \
  NET_PGI_VERSION_CHECK(18,10,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_IBM_VERSION_CHECK(13,1,5)
#  define NET_UNREACHABLE() __builtin_unreachable()
#elif defined(NET_ASSUME)
#  define NET_UNREACHABLE() NET_ASSUME(0)
#endif
#if !defined(NET_ASSUME)
#  if defined(NET_UNREACHABLE)
#    define NET_ASSUME(expr) NET_STATIC_CAST(void, ((expr) ? 1 : (NET_UNREACHABLE(), 1)))
#  else
#    define NET_ASSUME(expr) NET_STATIC_CAST(void, expr)
#  endif
#endif
#if defined(NET_UNREACHABLE)
#  if  \
      NET_TI_CL2000_VERSION_CHECK(6,2,0) || \
      NET_TI_CL6X_VERSION_CHECK(4,0,0)
#    define NET_UNREACHABLE_RETURN(value) return (NET_STATIC_CAST(void, NET_ASSUME(0)), (value))
#  else
#    define NET_UNREACHABLE_RETURN(value) NET_UNREACHABLE()
#  endif
#else
#  define NET_UNREACHABLE_RETURN(value) return (value)
#endif
#if !defined(NET_UNREACHABLE)
#  define NET_UNREACHABLE() NET_ASSUME(0)
#endif

// Mark parameters as non-null
// This will specify to the compiler that specific
// parameters should never be null. If they are, a diagnostic
// should be outputted.
NET_DIAGNOSTIC_PUSH
#if NET_HAS_WARNING("-Wpedantic")
#  pragma clang diagnostic ignored "-Wpedantic"
#endif
#if NET_HAS_WARNING("-Wc++98-compat-pedantic") && defined(__cplusplus)
#  pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#if NET_GCC_HAS_WARNING("-Wvariadic-macros",4,0,0)
#  if defined(__clang__)
#    pragma clang diagnostic ignored "-Wvariadic-macros"
#  elif defined(NET_GCC_VERSION)
#    pragma GCC diagnostic ignored "-Wvariadic-macros"
#  endif
#endif
#if defined(NET_NON_NULL)
#  undef NET_NON_NULL
#endif
#if \
  NET_HAS_ATTRIBUTE(nonnull) || \
  NET_GCC_VERSION_CHECK(3,3,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_ARM_VERSION_CHECK(4,1,0)
#  define NET_NON_NULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
#  define NET_NON_NULL(...)
#endif
NET_DIAGNOSTIC_POP

// Make some sort of assumption
// This will make some sort of assumption about the likelihood of
// the output of an expression to allow the compiler to make
// optimizations.
#if defined(NET_PREDICT)
#  undef NET_PREDICT
#endif
#if defined(NET_LIKELY)
#  undef NET_LIKELY
#endif
#if defined(NET_UNLIKELY)
#  undef NET_UNLIKELY
#endif
#if defined(NET_UNPREDICTABLE)
#  undef NET_UNPREDICTABLE
#endif
#if NET_HAS_BUILTIN(__builtin_unpredictable)
#  define NET_UNPREDICTABLE(expr) __builtin_unpredictable((expr))
#endif
#if \
  NET_HAS_BUILTIN(__builtin_expect_with_probability) || \
  NET_GCC_VERSION_CHECK(9,0,0)
#  define NET_PREDICT(expr, value, probability) __builtin_expect_with_probability(  (expr), (value), (probability))
#  define NET_PREDICT_TRUE(expr, probability)   __builtin_expect_with_probability(!!(expr),    1   , (probability))
#  define NET_PREDICT_FALSE(expr, probability)  __builtin_expect_with_probability(!!(expr),    0   , (probability))
#  define NET_LIKELY(expr)                      __builtin_expect                 (!!(expr),    1                  )
#  define NET_UNLIKELY(expr)                    __builtin_expect                 (!!(expr),    0                  )
#elif \
  NET_HAS_BUILTIN(__builtin_expect) || \
  NET_GCC_VERSION_CHECK(3,0,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  (NET_SUNPRO_VERSION_CHECK(5,15,0) && defined(__cplusplus)) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(10,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  NET_TI_ARMCL_VERSION_CHECK(4,7,0) || \
  NET_TI_CL430_VERSION_CHECK(3,1,0) || \
  NET_TI_CL2000_VERSION_CHECK(6,1,0) || \
  NET_TI_CL6X_VERSION_CHECK(6,1,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  NET_TINYC_VERSION_CHECK(0,9,27) || \
  NET_CRAY_VERSION_CHECK(8,1,0)
#  define NET_PREDICT(expr, expected, probability) \
     (((probability) >= 0.9) ? __builtin_expect((expr), (expected)) : (NET_STATIC_CAST(void, expected), (expr)))
#  define NET_PREDICT_TRUE(expr, probability) \
     (__extension__ ({ \
       double hedley_probability_ = (probability); \
       ((hedley_probability_ >= 0.9) ? __builtin_expect(!!(expr), 1) : ((hedley_probability_ <= 0.1) ? __builtin_expect(!!(expr), 0) : !!(expr))); \
     }))
#  define NET_PREDICT_FALSE(expr, probability) \
     (__extension__ ({ \
       double hedley_probability_ = (probability); \
       ((hedley_probability_ >= 0.9) ? __builtin_expect(!!(expr), 0) : ((hedley_probability_ <= 0.1) ? __builtin_expect(!!(expr), 1) : !!(expr))); \
     }))
#  define NET_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#  define NET_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#  define NET_PREDICT(expr, expected, probability) (NET_STATIC_CAST(void, expected), (expr))
#  define NET_PREDICT_TRUE(expr, probability) (!!(expr))
#  define NET_PREDICT_FALSE(expr, probability) (!!(expr))
#  define NET_LIKELY(expr) (!!(expr))
#  define NET_UNLIKELY(expr) (!!(expr))
#endif
#if !defined(NET_UNPREDICTABLE)
#  define NET_UNPREDICTABLE(expr) NET_PREDICT(expr, 1, 0.5)
#endif

// Mark a function as being malloc-like
// This will mark a function as being malloc-like. This means
// that the pointer returned is not aliased anywhere else, as
// well as that there are no pointers to valid objects in the
// returned pointer. For example, malloc and calloc use this,
// because their output is zero-ed out, but realloc does not,
// because its output could point to storage containing pointers.
#if defined(NET_MALLOC)
#  undef NET_MALLOC
#endif
#if \
  NET_HAS_ATTRIBUTE(malloc) || \
  NET_GCC_VERSION_CHECK(3,1,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_SUNPRO_VERSION_CHECK(5,11,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(12,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_MALLOC __attribute__((__malloc__))
#elif NET_SUNPRO_VERSION_CHECK(5,10,0)
#  define NET_MALLOC _Pragma("returns_new_memory")
#elif NET_MSVC_VERSION_CHECK(14, 0, 0)
#  define NET_MALLOC __declspec(restrict)
#else
#  define NET_MALLOC
#endif

// Mark the function as pure
// Mark the function as a pure function. This means that
// it has no side-effects, as well as that the return value
// only depends on the parameters and/or global variables.
#if defined(NET_PURE)
#  undef NET_PURE
#endif
#if \
  NET_HAS_ATTRIBUTE(pure) || \
  NET_GCC_VERSION_CHECK(2,96,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_SUNPRO_VERSION_CHECK(5,11,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(10,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  NET_PGI_VERSION_CHECK(17,10,0)
#  define NET_PURE __attribute__((__pure__))
#elif NET_SUNPRO_VERSION_CHECK(5,10,0)
#  define NET_PURE _Pragma("does_not_write_global_data")
#elif defined(__cplusplus) && \
    ( \
      NET_TI_CL430_VERSION_CHECK(2,0,1) || \
      NET_TI_CL6X_VERSION_CHECK(4,0,0) || \
      NET_TI_CL7X_VERSION_CHECK(1,2,0) \
    )
#  define NET_PURE _Pragma("FUNC_IS_PURE;")
#else
#  define NET_PURE
#endif

// Mark the function as constant
// Mark the function as a constant function. This means that
// the function has no side-effects, as well as that the return
// value only depends on the parameters. Note that pointer
// parameters cannot be used.
#if defined(NET_CONST)
#  undef NET_CONST
#endif
#if \
  NET_HAS_ATTRIBUTE(const) || \
  NET_GCC_VERSION_CHECK(2,5,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_SUNPRO_VERSION_CHECK(5,11,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(10,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  NET_PGI_VERSION_CHECK(17,10,0)
#  define NET_CONST __attribute__((__const__))
#elif \
  NET_SUNPRO_VERSION_CHECK(5,10,0)
#  define NET_CONST _Pragma("no_side_effect")
#else
#  define NET_CONST NET_PURE
#endif

// Mark the pointer as being restricted
// Mark a pointer as being a restricted pointer. This means that it
// is not aliased anywhere else. For example, the object is only referenced
// using the direct pointer itself, or by a value directly derived
// from it (i.e. "pointer + 1").
#if defined(NET_RESTRICT)
#  undef NET_RESTRICT
#endif
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && !defined(__cplusplus)
#  define NET_RESTRICT restrict
#elif \
  NET_GCC_VERSION_CHECK(3,1,0) || \
  NET_MSVC_VERSION_CHECK(14,0,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(10,1,0) || \
  NET_PGI_VERSION_CHECK(17,10,0) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  NET_TI_CL2000_VERSION_CHECK(6,2,4) || \
  NET_TI_CL6X_VERSION_CHECK(8,1,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  (NET_SUNPRO_VERSION_CHECK(5,14,0) && defined(__cplusplus)) || \
  NET_IAR_VERSION_CHECK(8,0,0) || \
  defined(__clang__)
#  define NET_RESTRICT __restrict
#elif NET_SUNPRO_VERSION_CHECK(5,3,0) && !defined(__cplusplus)
#  define NET_RESTRICT _Restrict
#else
#  define NET_RESTRICT
#endif

// Hint that the function should be inlined
// This will hint to the compiler that the function should be inlined
// wherever possible. This is only a hint, and is not actually enforced.
#if defined(NET_INLINE)
#  undef NET_INLINE
#endif
#if \
  (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
  (defined(__cplusplus) && (__cplusplus >= 199711L))
#  define NET_INLINE inline
#elif \
  defined(NET_GCC_VERSION) || \
  NET_ARM_VERSION_CHECK(6,2,0)
#  define NET_INLINE __inline__
#elif \
  NET_MSVC_VERSION_CHECK(12,0,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_TI_ARMCL_VERSION_CHECK(5,1,0) || \
  NET_TI_CL430_VERSION_CHECK(3,1,0) || \
  NET_TI_CL2000_VERSION_CHECK(6,2,0) || \
  NET_TI_CL6X_VERSION_CHECK(8,0,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_INLINE __inline
#else
#  define NET_INLINE
#endif

// Tell the compiler to inline the function
// This will tell the compiler to inline the function. This is
// a command rather than a hint, and will enforce inlining no
// matter what.
#if defined(NET_ALWAYS_INLINE)
#  undef NET_ALWAYS_INLINE
#endif
#if \
  NET_HAS_ATTRIBUTE(always_inline) || \
  NET_GCC_VERSION_CHECK(4,0,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_SUNPRO_VERSION_CHECK(5,11,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(10,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_ALWAYS_INLINE __attribute__((__always_inline__)) NET_INLINE
#elif NET_MSVC_VERSION_CHECK(12,0,0)
#  define NET_ALWAYS_INLINE __forceinline
#elif defined(__cplusplus) && \
    ( \
      NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
      NET_TI_CL430_VERSION_CHECK(4,3,0) || \
      NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
      NET_TI_CL6X_VERSION_CHECK(6,1,0) || \
      NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
      NET_TI_CLPRU_VERSION_CHECK(2,1,0) \
    )
#  define NET_ALWAYS_INLINE _Pragma("FUNC_ALWAYS_INLINE;")
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_ALWAYS_INLINE _Pragma("inline=forced")
#else
#  define NET_ALWAYS_INLINE NET_INLINE
#endif

// Tell the compiler to never inline the function
// This will tell the compiler to never inline the function. This will
// ensure that the function is never inlined, and is a command rather
// than a hint.
#if defined(NET_NEVER_INLINE)
#  undef NET_NEVER_INLINE
#endif
#if \
  NET_HAS_ATTRIBUTE(noinline) || \
  NET_GCC_VERSION_CHECK(4,0,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0) || \
  NET_SUNPRO_VERSION_CHECK(5,11,0) || \
  NET_ARM_VERSION_CHECK(4,1,0) || \
  NET_IBM_VERSION_CHECK(10,1,0) || \
  NET_TI_VERSION_CHECK(15,12,0) || \
  (NET_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (NET_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (NET_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL430_VERSION_CHECK(4,3,0) || \
  (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  NET_TI_CL6X_VERSION_CHECK(7,5,0) || \
  NET_TI_CL7X_VERSION_CHECK(1,2,0) || \
  NET_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define NET_NEVER_INLINE __attribute__((__noinline__))
#elif NET_MSVC_VERSION_CHECK(13,10,0)
#  define NET_NEVER_INLINE __declspec(noinline)
#elif NET_PGI_VERSION_CHECK(10,2,0)
#  define NET_NEVER_INLINE _Pragma("noinline")
#elif NET_TI_CL6X_VERSION_CHECK(6,0,0) && defined(__cplusplus)
#  define NET_NEVER_INLINE _Pragma("FUNC_CANNOT_INLINE;")
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_NEVER_INLINE _Pragma("inline=never")
#elif NET_COMPCERT_VERSION_CHECK(3,2,0)
#  define NET_NEVER_INLINE __attribute((noinline))
#elif NET_PELLES_VERSION_CHECK(9,0,0)
#  define NET_NEVER_INLINE __declspec(noinline)
#else
#  define NET_NEVER_INLINE
#endif

// Mark symbol visibility
// These can be used to mark the visibility of a symbol. A private
// symbol is not exposed publicly, and should never be used in a
// header. A public symbol is exposed publicly, and an import symbol
// is a symbol that is defined in another library.
#if defined(NET_PRIVATE)
#  undef NET_PRIVATE
#endif
#if defined(NET_PUBLIC)
#  undef NET_PUBLIC
#endif
#if defined(NET_IMPORT)
#  undef NET_IMPORT
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
#  define NET_PRIVATE
#  define NET_PUBLIC   __declspec(dllexport)
#  define NET_IMPORT   __declspec(dllimport)
#else
#  if \
    NET_HAS_ATTRIBUTE(visibility) || \
    NET_GCC_VERSION_CHECK(3,3,0) || \
    NET_SUNPRO_VERSION_CHECK(5,11,0) || \
    NET_INTEL_VERSION_CHECK(13,0,0) || \
    NET_ARM_VERSION_CHECK(4,1,0) || \
    NET_IBM_VERSION_CHECK(13,1,0) || \
    ( \
      defined(__TI_EABI__) && \
      ( \
        (NET_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
        NET_TI_CL6X_VERSION_CHECK(7,5,0) \
      ) \
    )
#    define NET_PRIVATE __attribute__((__visibility__("hidden")))
#    define NET_PUBLIC  __attribute__((__visibility__("default")))
#  else
#    define NET_PRIVATE
#    define NET_PUBLIC
#  endif
#  define NET_IMPORT    extern
#endif

// Mark a function as never throwing a C++ exception
// Mark the function as never throwing a C++ exception. Even though
// this is targeted for C++, it can also improve performance in C
// code. Note: Make sure that no function calls (even indirect ones)
// throw an exception.
#if defined(NET_NO_THROW)
#  undef NET_NO_THROW
#endif
#if \
  NET_HAS_ATTRIBUTE(nothrow) || \
  NET_GCC_VERSION_CHECK(3,3,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_NO_THROW __attribute__((__nothrow__))
#elif \
  NET_MSVC_VERSION_CHECK(13,1,0) || \
  NET_ARM_VERSION_CHECK(4,1,0)
#  define NET_NO_THROW __declspec(nothrow)
#else
#  define NET_NO_THROW
#endif

// Mark the function as not returning null
// This will mark the function as not returning null. This can
// be useful, because the compiler can make assumptions based on
// the result not being null.
#if defined(NET_RETURNS_NON_NULL)
#  undef NET_RETURNS_NON_NULL
#endif
#if \
  NET_HAS_ATTRIBUTE(returns_nonnull) || \
  NET_GCC_VERSION_CHECK(4,9,0)
#  define NET_RETURNS_NON_NULL __attribute__((__returns_nonnull__))
#elif defined(_Ret_notnull_) /* SAL */
#  define NET_RETURNS_NON_NULL _Ret_notnull_
#else
#  define NET_RETURNS_NON_NULL
#endif

// Utility macros for C declarations
// These are just a couple of usefull utility macros that
// help with making C declarations.
#if defined(NET_BEGIN_C_DECLS)
#  undef NET_BEGIN_C_DECLS
#endif
#if defined(NET_END_C_DECLS)
#  undef NET_END_C_DECLS
#endif
#if defined(NET_C_DECL)
#  undef NET_C_DECL
#endif
#if defined(__cplusplus)
#  define NET_BEGIN_C_DECLS extern "C" {
#  define NET_END_C_DECLS }
#  define NET_C_DECL extern "C"
#else
#  define NET_BEGIN_C_DECLS
#  define NET_END_C_DECLS
#  define NET_C_DECL
#endif

// Null value
// This is a compatability macro that evaluates to null.
// The actual value depends on the language and supported
// features.
#if defined(NET_NULL)
#  undef NET_NULL
#endif
#if defined(__cplusplus)
#  if __cplusplus >= 201103L
#    define NET_NULL NET_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(nullptr)
#  elif defined(NULL)
#    define NET_NULL NULL
#  else
#    define NET_NULL NET_STATIC_CAST(void*, 0)
#  endif
#elif defined(NULL)
#  define NET_NULL NULL
#else
#  define NET_NULL ((void*) 0)
#endif

// Emit a diagnostic mesage
// This will create a pragma to emit a diagnostic message.
#if defined(NET_MESSAGE)
#  undef NET_MESSAGE
#endif
#if NET_HAS_WARNING("-Wunknown-pragmas")
#  define NET_MESSAGE(msg) \
  NET_DIAGNOSTIC_PUSH \
  NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS \
  NET_PRAGMA(message msg) \
  NET_DIAGNOSTIC_POP
#elif \
  NET_GCC_VERSION_CHECK(4,4,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_MESSAGE(msg) NET_PRAGMA(message msg)
#elif NET_CRAY_VERSION_CHECK(5,0,0)
#  define NET_MESSAGE(msg) NET_PRAGMA(_CRI message msg)
#elif NET_IAR_VERSION_CHECK(8,0,0)
#  define NET_MESSAGE(msg) NET_PRAGMA(message(msg))
#elif NET_PELLES_VERSION_CHECK(2,0,0)
#  define NET_MESSAGE(msg) NET_PRAGMA(message(msg))
#else
#  define NET_MESSAGE(msg)
#endif

// Emit a warning diagnostic
// This will create a pragma to emit a warning diagnostic
#if defined(NET_WARNING)
#  undef NET_WARNING
#endif
#if NET_HAS_WARNING("-Wunknown-pragmas")
#  define NET_WARNING(msg) \
  NET_DIAGNOSTIC_PUSH \
  NET_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS \
  NET_PRAGMA(clang warning msg) \
  NET_DIAGNOSTIC_POP
#elif \
  NET_GCC_VERSION_CHECK(4,8,0) || \
  NET_PGI_VERSION_CHECK(18,4,0) || \
  NET_INTEL_VERSION_CHECK(13,0,0)
#  define NET_WARNING(msg) NET_PRAGMA(GCC warning msg)
#elif NET_MSVC_VERSION_CHECK(15,0,0)
#  define NET_WARNING(msg) NET_PRAGMA(message(msg))
#else
#  define NET_WARNING(msg) NET_MESSAGE(msg)
#endif

// Create a parameter precondition
// This will create a precondition for the function parameters,
// which if not satisfied will emit a warning diagnostic.
#if defined(NET_REQUIRE)
#  undef NET_REQUIRE
#endif
#if defined(NET_REQUIRE_MSG)
#  undef NET_REQUIRE_MSG
#endif
#if NET_HAS_ATTRIBUTE(diagnose_if)
#  if NET_HAS_WARNING("-Wgcc-compat")
#    define NET_REQUIRE(expr) \
       NET_DIAGNOSTIC_PUSH \
       _Pragma("clang diagnostic ignored \"-Wgcc-compat\"") \
       __attribute__((diagnose_if(!(expr), #expr, "error"))) \
       NET_DIAGNOSTIC_POP
#    define NET_REQUIRE_MSG(expr,msg) \
       NET_DIAGNOSTIC_PUSH \
       _Pragma("clang diagnostic ignored \"-Wgcc-compat\"") \
       __attribute__((diagnose_if(!(expr), msg, "error"))) \
       NET_DIAGNOSTIC_POP
#  else
#    define NET_REQUIRE(expr) __attribute__((diagnose_if(!(expr), #expr, "error")))
#    define NET_REQUIRE_MSG(expr,msg) __attribute__((diagnose_if(!(expr), msg, "error")))
#  endif
#else
#  define NET_REQUIRE(expr)
#  define NET_REQUIRE_MSG(expr,msg)
#endif

#if defined(NET_API)
#  undef NET_API
#endif
#if defined(NET_COMPILATION)
#  define NET_API NET_PUBLIC
#else
#  define NET_API NET_IMPORT
# endif

#define NET_VERSION_MAJOR 0
#define NET_VERSION_MINOR 1
#define NET_VERSION_REVISION 0

#define NET_VERSION NET_VERSION_ENCODE(NET_VERSION_MAJOR, NET_VERSION_MINOR, NET_VERSION_REVISION)
#if !defined(NET_TARGET_VERSION)
#  define NET_TARGET_VERSION NET_VERSION
#endif
#if !defined(NET_VERSION_MIN_REQUIRED)
#  define NET_VERSION_MIN_REQUIRED NET_TARGET_VERSION
#endif
#if !defined(NET_VERSION_MAX_ALLOWED)
#  define NET_VERSION_MAX_ALLOWED NET_TARGET_VERSION
#endif

#define NET_VERSION_0_1 NET_VERSION_ENCODE(0,1,0)

#if NET_VERSION_MIN_REQUIRED > NET_VERSION_0_1
#  define NET_DEPRECATED_SINCE_1_1 NET_DEPRECATED(0.1)
#  define NET_DEPRECATED_SINCE_1_1_FOR(replacement) NET_DEPRECATED_FOR(0.1, replacement)
#else
#  define NET_DEPRECATED_SINCE_1_1
#  define NET_DEPRECATED_SINCE_1_1_FOR(replacement)
#endif

#if NET_VERSION_MAX_ALLOWED < NET_VERSION_0_1
#  define NET_AVAILABLE_SINCE_1_1 NET_UNAVAILABLE(0.1)
#else
#  define NET_AVAILABLE_SINCE_1_1
#endif

#endif /* !defined(NET_UTIL_VERSION) || (NET_UTIL_VERSION < 1) */
