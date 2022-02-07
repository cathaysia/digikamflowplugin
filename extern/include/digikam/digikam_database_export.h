
#ifndef DIGIKAM_DATABASE_EXPORT_H
#define DIGIKAM_DATABASE_EXPORT_H

#ifdef DIGIKAM_DATABASE_STATIC_DEFINE
#  define DIGIKAM_DATABASE_EXPORT
#  define DIGIKAM_DATABASE_NO_EXPORT
#else
#  ifndef DIGIKAM_DATABASE_EXPORT
#    ifdef digikamdatabase_EXPORTS
        /* We are building this library */
#      define DIGIKAM_DATABASE_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DIGIKAM_DATABASE_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DIGIKAM_DATABASE_NO_EXPORT
#    define DIGIKAM_DATABASE_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DIGIKAM_DATABASE_DEPRECATED
#  define DIGIKAM_DATABASE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DIGIKAM_DATABASE_DEPRECATED_EXPORT
#  define DIGIKAM_DATABASE_DEPRECATED_EXPORT DIGIKAM_DATABASE_EXPORT DIGIKAM_DATABASE_DEPRECATED
#endif

#ifndef DIGIKAM_DATABASE_DEPRECATED_NO_EXPORT
#  define DIGIKAM_DATABASE_DEPRECATED_NO_EXPORT DIGIKAM_DATABASE_NO_EXPORT DIGIKAM_DATABASE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DIGIKAM_DATABASE_NO_DEPRECATED
#    define DIGIKAM_DATABASE_NO_DEPRECATED
#  endif
#endif

#endif /* DIGIKAM_DATABASE_EXPORT_H */
