/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2009-11-13
 * Description : a template to create wizard dialog.
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

#ifndef DIGIKAM_DWIZARD_DLG_H
#define DIGIKAM_DWIZARD_DLG_H

// Qt includes

#include <QWizard>
#include <QString>

// Local includes

#include "dplugin.h"
#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT DWizardDlg : public QWizard
{
    Q_OBJECT

public:

    explicit DWizardDlg(QWidget* const parent, const QString& objName);
    ~DWizardDlg() override;

    void setPlugin(DPlugin* const tool);

protected:

    void restoreDialogSize();
    void saveDialogSize();

private Q_SLOTS:

    void slotAboutPlugin();

private:

    DPlugin* m_tool;

    Q_DISABLE_COPY(DWizardDlg)
};

} // namespace Digikam

#endif // DIGIKAM_DWIZARD_DLG_H
