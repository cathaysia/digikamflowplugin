
#ifndef DIGIKAM_GUI_EXPORT_H
#define DIGIKAM_GUI_EXPORT_H

#ifdef DIGIKAM_GUI_STATIC_DEFINE
#  define DIGIKAM_GUI_EXPORT
#  define DIGIKAM_GUI_NO_EXPORT
#else
#  ifndef DIGIKAM_GUI_EXPORT
#    ifdef digikamgui_EXPORTS
        /* We are building this library */
#      define DIGIKAM_GUI_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DIGIKAM_GUI_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DIGIKAM_GUI_NO_EXPORT
#    define DIGIKAM_GUI_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DIGIKAM_GUI_DEPRECATED
#  define DIGIKAM_GUI_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DIGIKAM_GUI_DEPRECATED_EXPORT
#  define DIGIKAM_GUI_DEPRECATED_EXPORT DIGIKAM_GUI_EXPORT DIGIKAM_GUI_DEPRECATED
#endif

#ifndef DIGIKAM_GUI_DEPRECATED_NO_EXPORT
#  define DIGIKAM_GUI_DEPRECATED_NO_EXPORT DIGIKAM_GUI_NO_EXPORT DIGIKAM_GUI_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DIGIKAM_GUI_NO_DEPRECATED
#    define DIGIKAM_GUI_NO_DEPRECATED
#  endif
#endif

#endif /* DIGIKAM_GUI_EXPORT_H */
