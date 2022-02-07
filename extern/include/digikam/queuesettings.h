/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2009-02-21
 * Description : Queue common settings container.
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

#ifndef DIGIKAM_BQM_QUEUE_SETTINGS_H
#define DIGIKAM_BQM_QUEUE_SETTINGS_H

// Qt includes

#include <QUrl>

// Local includes

#include "iofilesettings.h"
#include "drawdecodersettings.h"
#include "filesaveconflictbox.h"

namespace Digikam
{

/**
 * This container host all common settings used by a queue, not including assigned batch tools
 */
class QueueSettings
{

public:

    enum RenamingRule
    {
        USEORIGINAL = 0,
        CUSTOMIZE
    };

    enum RawLoadingRule
    {
        USEEMBEDEDJPEG = 0,
        DEMOSAICING
    };

public:

    explicit QueueSettings();
    ~QueueSettings();

public:

    bool                              useMultiCoreCPU;

    bool                              saveAsNewVersion;

    /// Setting managed through Metadata control panel.
    bool                              exifSetOrientation;

    /// If true, original file dir will be used to process queue items.
    bool                              useOrgAlbum;

    QString                           renamingParser;

    QUrl                              workingUrl;

    FileSaveConflictBox::ConflictRule conflictRule;
    RenamingRule                      renamingRule;
    RawLoadingRule                    rawLoadingRule;

    DRawDecoderSettings               rawDecodingSettings;

    IOFileSettings                    ioFileSettings;
};

} // namespace Digikam

#endif // DIGIKAM_BQM_QUEUE_SETTINGS_H
