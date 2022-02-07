/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2014-11-14
 * Description : Settings container for preview settings
 *
 * Copyright (C) 2014 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#ifndef DIGIKAM_PREVIEW_SETTINGS_H
#define DIGIKAM_PREVIEW_SETTINGS_H

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT PreviewSettings
{
public:

    enum Quality
    {
        /** A preview were loading time is most important.
         *  Preview can be reduced in size.
         *  Additionally specifying the size of the preview area may be appropriate
         */
        FastPreview,
        /**
         * Load a preview that is as large as possible without sacrificing speed
         * for performance. Especially, raw previews are taken if larger than the given size.
         * Raw decoding and color management settings will be adjusted.
         */
        FastButLargePreview,
        /**
         * Load a high quality additional image. For normal images, loads the full data.
         * For RAW, the additional settings below are taken into account
         */
        HighQualityPreview
    };

    enum RawLoading
    {
        RawPreviewAutomatic,
        RawPreviewFromEmbeddedPreview,
        RawPreviewFromRawHalfSize
    };

public:

    explicit PreviewSettings(Quality quality = HighQualityPreview,
                             RawLoading rawLoading = RawPreviewAutomatic);
    ~PreviewSettings();

    bool operator==(const PreviewSettings& other) const;

public:

    static PreviewSettings fastPreview();
    static PreviewSettings highQualityPreview();

public:

    Quality    quality;
    RawLoading rawLoading;
    bool       convertToEightBit;
};

} // namespace Digikam

#endif // DIGIKAM_PREVIEW_SETTINGS_H
