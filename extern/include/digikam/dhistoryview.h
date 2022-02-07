/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2009-05-26
 * Description : History view.
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

#ifndef DIGIKAM_DHISTORY_VIEW_H
#define DIGIKAM_DHISTORY_VIEW_H

// Qt includes

#include <QTreeWidget>
#include <QWidget>
#include <QString>
#include <QVariant>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT DHistoryView : public QTreeWidget
{
    Q_OBJECT

public:

    enum EntryType
    {
        StartingEntry = 0,
        SuccessEntry,
        WarningEntry,
        ErrorEntry,
        ProgressEntry,
        CancelEntry
    };

public:

    explicit DHistoryView(QWidget* const parent);
    ~DHistoryView()                     override;

    void addEntry(const QString& msg,
                  EntryType type,
                  const QVariant& metadata = QVariant());

Q_SIGNALS:

    void signalEntryClicked(const QVariant& metadata);

private Q_SLOTS:

    void slotItemDoubleClicked(QTreeWidgetItem*);
    void slotContextMenu();
    void slotCopy2ClipBoard();

private:

    void mouseMoveEvent(QMouseEvent*)   override;
};

} // namespace Digikam

#endif // DIGIKAM_DHISTORY_VIEW_H
