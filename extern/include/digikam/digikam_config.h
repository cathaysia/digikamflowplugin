/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2009-08-03
 * Description : digiKam config header
 *
 * Copyright (C) 2009-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef DIGIKAM_CONFIG_H
#define DIGIKAM_CONFIG_H

/* Define to 1 if you have KDEPIM shared library installed */
#define HAVE_AKONADICONTACT 1

/* Define to 1 if an external liblqr-1 shared library have been found */
#define HAVE_LIBLQR_1 1

/* Define to 1 if an external lensfun shared library have been found */
#define HAVE_LENSFUN 1

/* Define to 1 if an external KIO shared library have been found */
#define HAVE_KIO 1

/* Define to 1 if an external Ksane shared library have been found */
#define HAVE_KSANE 1

/* Define to 1 if an external Marble shared library have been found */
#define HAVE_MARBLE 1

/* Define to 1 if an external Eigen3 shared library have been found */
#define HAVE_EIGEN3 1

/* Define to 1 if an external x265 shared library have been found */
/* #undef HAVE_X265 */

/* The Eigen3 version string, if found, or an empty string otherwise */
#define EIGEN3_VERSION_STRING "3.3.9"

/* Define to 1 if GPhoto2 shared library is installed */
#define HAVE_GPHOTO2 1

/* Define to 1 if Jasper shared library is installed */
#define HAVE_JASPER 1

/* Define to 1 if libgphoto2 2.5 shared library is installed */
#define HAVE_GPHOTO25 1

/* Define to 1 if you have Baloo shared libraries installed */
#define HAVE_KFILEMETADATA 1

/* Define to 1 if you have KIconThemes shared libraries installed */
#define HAVE_KICONTHEMES 1

/* Define to 1 if you have KNotifications shared libraries installed */
#define HAVE_KNOTIFICATIONS 1

/* Define to 1 if you have KNotifyConfig shared libraries installed */
#define HAVE_KNOTIFYCONFIG 1

/* Define to 1 if you have KCalendar core shared libraries installed */
/* #undef HAVE_KCALENDAR */

/* Define to 1 if you have KCalendar core shared libraries with QDateTime support installed */
/* #undef HAVE_KCALENDAR_QDATETIME */

/* Define to 1 if Panorama tool is supported */
#define HAVE_PANORAMA 1

/* Define to 1 if VKontakte tool is supported */
/* #undef HAVE_VKONTAKTE */

/* Define to 1 if HTMLGallery tool is supported */
#define HAVE_HTMLGALLERY 1

/* Define to 1 if we support mysql database */
#define HAVE_MYSQLSUPPORT 1

/* Define to 1 if we support mysql internal database server */
#define HAVE_INTERNALMYSQL 1

/* Define to 1 if preview of video files is supported */
/* #undef HAVE_MEDIAPLAYER */

/* Define to 1 if DBUS is supported */
#define HAVE_DBUS 1

/* Define to 1 if system use OpenGL */
#define HAVE_OPENGL 1

/* Define to 1 if system use X11 */
#define HAVE_X11 1

/* Define to 1 if changing application styles is supported */
#define HAVE_APPSTYLE_SUPPORT 1

/* Define to 1 if system use QtWebEngine instead of QWebKit */
#define HAVE_QWEBENGINE 1

/* Define to 1 if system use QtXmlPatterns */
#define HAVE_QTXMLPATTERNS 1

/* Define to 1 if ImageMagick codecs are supported */
#define HAVE_IMAGE_MAGICK 1

/* Defines to 1 if the Dr. Mingw crash handler should be used */
/* #undef HAVE_DRMINGW */

#define LIBEXEC_INSTALL_DIR "/usr//usr/libexec"

/*
  Disable indeep warnings from Visual Studio C++ 2008 (9.0)
*/
#if defined(_MSC_VER)
// To disable warnings about no suitable definition provided for explicit template instantiation request.
#pragma warning(disable : 4661)
// To disable warnings about deprecated POSIX methods().
#pragma warning(disable : 4996)
// To disable warnings about qualifier applied to reference type ignored.
#pragma warning(disable : 4181)
#endif

#endif // DIGIKAM_CONFIG_H
