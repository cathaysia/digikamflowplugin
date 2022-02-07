/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2010-02-20
 * Description : color selector widget
 *
 * Copyright (C) 2010-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_DCOLOR_SELECTOR_H
#define DIGIKAM_DCOLOR_SELECTOR_H

// Qt includes

#include <QColor>
#include <QPushButton>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

/**
 * A widget to chosse a color from a palette.
 */
class DIGIKAM_EXPORT DColorSelector : public QPushButton
{
    Q_OBJECT

public:

    explicit DColorSelector(QWidget* const parent = nullptr);
    ~DColorSelector() override;

    void setColor(const QColor& color);
    QColor color() const;

    void setAlphaChannelEnabled(bool);

Q_SIGNALS:

    void signalColorSelected(const QColor&);

private Q_SLOTS:

    void slotBtnClicked();

private:

    void paintEvent(QPaintEvent*) override;

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_DCOLOR_SELECTOR_H
