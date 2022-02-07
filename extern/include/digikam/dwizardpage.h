/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2009-11-13
 * Description : a template to create wizard page.
 *
 * Copyright (C) 2009-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_DWIZARD_PAGE_H
#define DIGIKAM_DWIZARD_PAGE_H

// Qt includes

#include <QWizardPage>
#include <QString>
#include <QPixmap>

// Local includes

#include "digikam_export.h"

class QWizard;

namespace Digikam
{

class DIGIKAM_EXPORT DWizardPage : public QWizardPage
{
    Q_OBJECT

public:

    explicit DWizardPage(QWizard* const dlg, const QString& title);
    ~DWizardPage()                override;

public:

    void setPageWidget(QWidget* const w);
    void removePageWidget(QWidget* const w);
    void setLeftBottomPix(const QPixmap& pix);
    void setLeftBottomPix(const QIcon& icon);
    void setShowLeftView(bool v);

    void setComplete(bool b);
    bool isComplete()       const override;

    int  id()               const;

    QWizard* assistant()    const;

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_DWIZARD_PAGE_H
