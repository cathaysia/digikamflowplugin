/* ============================================================
 *
 * This file is a part of digiKam flow plugin project
 * https://github.com/cathaysia/digikamflowplugin
 *
 * Date        : 2021-05-22
 * Description : Flow View generic plugin for digiKam.
 *
 * Copyright (C) 2021-2022 by DragonBillow <DragonBillow at outlook dot com>
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
    int     heightForWidth(int w) const override;
    int     widthForHeight(int h) const;
    void    mouseDoubleClickEvent(QMouseEvent *event) override;

    void adjust();

private:
    qreal  scaleFactor_ = 0;
    QImage pix_;
};
