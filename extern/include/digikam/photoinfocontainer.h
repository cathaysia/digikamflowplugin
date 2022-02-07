/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2006-04-21
 * Description : photo information container
 *
 * Copyright (C) 2006-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_PHOTO_INFO_CONTAINER_H
#define DIGIKAM_PHOTO_INFO_CONTAINER_H

// Qt includes

#include <QString>
#include <QDateTime>
#include <QDebug>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT PhotoInfoContainer
{

public:

    PhotoInfoContainer()                          = default;
    PhotoInfoContainer(const PhotoInfoContainer&) = default;
    ~PhotoInfoContainer()                         = default;

    PhotoInfoContainer& operator=(const PhotoInfoContainer&) = default;
    PhotoInfoContainer& operator=(PhotoInfoContainer&&)      = default;

    bool isEmpty()                               const;
    bool isNull()                                const;

    bool operator==(const PhotoInfoContainer& t) const;

public:

    QString   make;
    QString   model;
    QString   lens;
    QString   exposureTime;
    QString   exposureMode;
    QString   exposureProgram;
    QString   aperture;
    QString   focalLength;
    QString   focalLength35mm;
    QString   sensitivity;
    QString   flash;
    QString   whiteBalance;

    QDateTime dateTime;

    bool      hasCoordinates = false;  ///< true if GPS info are present
};

DIGIKAM_EXPORT QDataStream& operator<<(QDataStream& ds, const PhotoInfoContainer& info);
DIGIKAM_EXPORT QDataStream& operator>>(QDataStream& ds, PhotoInfoContainer& info);

//! qDebug() stream operator. Writes property @a t to the debug output in a nicely formatted way.
DIGIKAM_EXPORT QDebug operator<<(QDebug dbg, const PhotoInfoContainer& t);

} // namespace Digikam

#endif // DIGIKAM_PHOTO_INFO_CONTAINER_H
