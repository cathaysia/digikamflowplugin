/* ============================================================
 *
 * This file is a part of digiKam flow plugin project
 * https://github.com/cathaysia/digikamflowplugin
 *
 * Date        : 2021-05-22
 * Description : Flow View generic plugin for digiKam.
 *
 * Copyright (C) 2021-2022 by Cathaysia <DragonBillow at outlook dot com>
 *
 * Redistribution and use is allowed according to the terms of the GPL3 license.
 * For details see the accompanying LICENSE file.
 *
 * ============================================================ */

#pragma once

#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class AspectRatioPixmapLabel : public QLabel {
    Q_OBJECT;
public:
    explicit AspectRatioPixmapLabel(QWidget *parent = 0);
    QSize   sizeHint() const override;
    QPixmap scaledPixmap() const;
    void    setPixmap(const QPixmap &pix);
    // overwrite these function
    void setScaledContents(bool);
    bool hasScaledContents() const;

    void adjust();

private:
    QImage pix_;
};
