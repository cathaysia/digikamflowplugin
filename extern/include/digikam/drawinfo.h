/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2007-05-02
 * Description : RAW file identification information container
 * Note:         this container permit to not expose the rest of
 *               digiKam code to libraw API and use Qt internal
 *               containers when possible.
 *
 * Copyright (C) 2007-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_DRAW_INFO_H
#define DIGIKAM_DRAW_INFO_H

// Qt includes

#include <QByteArray>
#include <QString>
#include <QDateTime>
#include <QSize>
#include <QDebug>
#include <QImage>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT DRawInfo
{

public:

    /**
     * The RAW image orientation values
     */
    enum ImageOrientation
    {
        ORIENTATION_NONE        = 0,
        ORIENTATION_180         = 3,
        ORIENTATION_Mirror90CCW = 4,
        ORIENTATION_90CCW       = 5,
        ORIENTATION_90CW        = 6
    };

public:

    /**
     * Standard constructor
     */
    explicit DRawInfo();

    /**
     * Standard destructor
     */
    ~DRawInfo();

public:

    /**
     * True if RAW file include an ICC color profile.
     */
    bool             hasIccProfile;

    /**
     * True is RAW file is decodable by dcraw.
     */
    bool             isDecodable;

    /**
     * The number of RAW colors.
     */
    int              rawColors;

    /**
     * The number of RAW images.
     */
    int              rawImages;

    /**
     * Black level from Raw histogram.
     */
    unsigned int     blackPoint;

    /**
     * Channel black levels from Raw histogram.
     */
    unsigned int     blackPointCh[4];

    /**
     * White level from Raw histogram.
     */
    unsigned int     whitePoint;

    /**
     * Top margin of raw image.
     */
    unsigned int     topMargin;

    /**
     * Left margin of raw image.
     */
    unsigned int     leftMargin;

    /**
     * The raw image orientation
     */
    ImageOrientation orientation;

    /**
     * The sensitivity in ISO used by camera to take the picture.
     */
    float            sensitivity;

    /**
     * 1/exposureTime = exposure time in seconds.
     */
    float            exposureTime;

    /**
     * Aperture value in APEX.
     */
    float            aperture;

    /**
     * Focal Length value in mm.
     */
    float            focalLength;

    /**
     * The pixel Aspect Ratio if != 1.0. NOTE: if == 1.0, dcraw do not show this value.
     */
    float            pixelAspectRatio;

    /**
     * Exposure compensation to be applied during raw conversion.
     */
    float            baselineExposure;

    /**
     * Ambient temperature in Celcius degrees.
     */
    float            ambientTemperature;

    /**
     * Ambient relative humidity in percent.
     */
    float            ambientHumidity;

    /**
     * Ambient air pressure in hPa or mbar.
     */
    float            ambientPressure;

    /**
     * Depth under water in metres, negative for above water.
     */
    float            ambientWaterDepth;

    /**
     * Directionless camera acceleration in units of mGal, or 10-5 m/s2.
     */
    float            ambientAcceleration;

    /**
     * Camera elevation angle in degrees.
     */
    float            ambientElevationAngle;

    /**
     * Describe how flash has been used by camera.
     */
    int              flashUsed;

    /**
     * The metering mode used by camera.
     */
    int              meteringMode;

    /**
     * The exposure program used by camera.
     */
    int              exposureProgram;

    /**
     * Exposure Index from the camera.
     */
    float            exposureIndex;

    /**
     * White color balance settings.
     */
    double           daylightMult[3];

    /**
     * Camera multipliers used for White Balance adjustments
     */
    double           cameraMult[4];

    /**
     * Camera Color Matrix
     */
    float            cameraColorMatrix1[3][4];
    float            cameraColorMatrix2[3][4];
    float            cameraXYZMatrix[4][3];

    /**
     * GPS information
     */
    double           latitude;
    double           longitude;
    double           altitude;
    bool             hasGpsInfo;          ///< true if GPS info are parsed from RAW file.

    /**
     * The used Color Keys
     */
    QString          colorKeys;

    /**
     * The camera maker.
     */
    QString          make;

    /**
     * The camera model.
     */
    QString          model;

    /**
     * The artist name who have picture owner.
     */
    QString          owner;

    /**
     * The software name which process raw image.
     */
    QString          software;

    /**
     * The Firmware name or version which create raw image.
     */
    QString          firmware;

    /**
     * The image description of raw image.
     */
    QString          description;

    /**
     * Serial number of raw image.
     */
    unsigned int     serialNumber;

    /**
     * The demosaising filter pattern.
     */
    QString          filterPattern;

    /**
     * The DNG version. NOTE: it is only shown with DNG RAW files.
     */
    QString          DNGVersion;

    /**
     * Non-localized name for the camera model that created the raw file
     */
    QString          uniqueCameraModel;

    /**
     * Localized name for the camera model that created the raw file
     */
    QString          localizedCameraModel;

    /**
     * An unique image ID generated by camera.
     */
    QString          imageID;

    /**
     * An unique RAW data ID.
     */
    QString          rawDataUniqueID;

    /**
     * The original RAW file name.
     */
    QString          originalRawFileName;

    /**
     * Date & time when the picture has been taken.
     */
    QDateTime        dateTime;

    /**
     * The image dimensions in pixels.
     */
    QSize            imageSize;

    /**
     * The thumb dimensions in pixels.
     */
    QSize            thumbSize;

    /**
     * The full RAW image dimensions in pixels.
     */
    QSize            fullSize;

    /**
     * The output dimensions in pixels.
     */
    QSize            outputSize;

    /**
     * Xmp metadata container extracted from RAW file, if present.
     */
    QByteArray       xmpData;

    /**
     * ICC color profilr container extracted from RAW file, if present.
     */
    QByteArray       iccData;

    /**
     * Thumbnail image data extracted from raw file.
     */
    QByteArray       thumbnail;

    /**
     * Description of lens properties.
     */
    QString          lensModel;
    QString          lensMake;
    QString          lensSerial;
    int              focalLengthIn35mmFilm;
    float            maxAperture;
};

//! qDebug() stream operator. Writes container @a c to the debug output in a nicely formatted way.
QDebug operator<<(QDebug dbg, const DRawInfo& c);

} // namespace Digikam

#endif // DIGIKAM_DRAW_INFO_H
