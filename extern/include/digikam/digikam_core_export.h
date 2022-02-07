
#ifndef DIGIKAM_EXPORT_H
#define DIGIKAM_EXPORT_H

#ifdef DIGIKAM_STATIC_DEFINE
#  define DIGIKAM_EXPORT
#  define DIGIKAM_NO_EXPORT
#else
#  ifndef DIGIKAM_EXPORT
#    ifdef digikamcore_EXPORTS
        /* We are building this library */
#      define DIGIKAM_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DIGIKAM_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DIGIKAM_NO_EXPORT
#    define DIGIKAM_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DIGIKAM_DEPRECATED
#  define DIGIKAM_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DIGIKAM_DEPRECATED_EXPORT
#  define DIGIKAM_DEPRECATED_EXPORT DIGIKAM_EXPORT DIGIKAM_DEPRECATED
#endif

#ifndef DIGIKAM_DEPRECATED_NO_EXPORT
#  define DIGIKAM_DEPRECATED_NO_EXPORT DIGIKAM_NO_EXPORT DIGIKAM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DIGIKAM_NO_DEPRECATED
#    define DIGIKAM_NO_DEPRECATED
#  endif
#endif

#endif /* DIGIKAM_EXPORT_H */
