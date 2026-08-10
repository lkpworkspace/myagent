
#ifndef MYAGENT_EXPORT_H
#define MYAGENT_EXPORT_H

#ifdef MYAGENT_STATIC_DEFINE
#  define MYAGENT_EXPORT
#  define MYAGENT_NO_EXPORT
#else
#  ifndef MYAGENT_EXPORT
#    ifdef myagent_EXPORTS
        /* We are building this library */
#      define MYAGENT_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define MYAGENT_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef MYAGENT_NO_EXPORT
#    define MYAGENT_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef MYAGENT_DEPRECATED
#  define MYAGENT_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MYAGENT_DEPRECATED_EXPORT
#  define MYAGENT_DEPRECATED_EXPORT MYAGENT_EXPORT MYAGENT_DEPRECATED
#endif

#ifndef MYAGENT_DEPRECATED_NO_EXPORT
#  define MYAGENT_DEPRECATED_NO_EXPORT MYAGENT_NO_EXPORT MYAGENT_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MYAGENT_NO_DEPRECATED
#    define MYAGENT_NO_DEPRECATED
#  endif
#endif

#endif /* MYAGENT_EXPORT_H */
