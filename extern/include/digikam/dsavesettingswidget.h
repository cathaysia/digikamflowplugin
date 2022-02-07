/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2006-09-13
 * Description : a widget to provide options to save image.
 *
 * Copyright (C) 2006-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef DIGIKAM_DSAVE_SETTINGS_WIDGET_H
#define DIGIKAM_DSAVE_SETTINGS_WIDGET_H

// Qt includes

#include <QWidget>

// Local includes

#include "digikam_export.h"
#include "filesaveconflictbox.h"

class KConfigGroup;

namespace Digikam
{

class DIGIKAM_EXPORT DSaveSettingsWidget : public QWidget
{
    Q_OBJECT

public:

    enum OutputFormat
    {
        OUTPUT_PNG = 0,
        OUTPUT_TIFF,
        OUTPUT_JPEG,
        OUTPUT_PPM
    };

public:

    explicit DSaveSettingsWidget(QWidget* const parent);
    ~DSaveSettingsWidget() override;

public:

    void setCustomSettingsWidget(QWidget* const custom);

    OutputFormat fileFormat()                           const;
    void setFileFormat(OutputFormat f);

    FileSaveConflictBox::ConflictRule conflictRule()    const;
    void setConflictRule(FileSaveConflictBox::ConflictRule r);

    QString extension()                                 const;
    QString typeMime()                                  const;

    void resetToDefault();

    void readSettings(KConfigGroup& group);
    void writeSettings(KConfigGroup& group);

    static QString extensionForFormat(OutputFormat format);

public Q_SLOTS:

    void slotPopulateImageFormat(bool sixteenBits);

Q_SIGNALS:

    void signalSaveFormatChanged();
    void signalConflictButtonChanged(int);

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_DSAVE_SETTINGS_WIDGET_H
