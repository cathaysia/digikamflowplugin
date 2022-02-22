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

#include "aspectratiopixmaplabel.hpp"

#include <QApplication>
#include <QDebug>
#include <cmath>

inline double sizeFactor(QSize const &a) {
    if(a.height() == 0) return 0;
    return a.width() * 100.f / a.height() / 100;
}

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget *parent) : QLabel(parent) {
    setScaledContents(true);
}

void AspectRatioPixmapLabel::setPixmap(const QPixmap &p) {
    pix_         = p.toImage();
    scaleFactor_ = sizeFactor(size());
    QLabel::setPixmap(scaledPixmap());
}

QSize AspectRatioPixmapLabel::sizeHint() const {
    return pix_.size();
}

QPixmap AspectRatioPixmapLabel::scaledPixmap() const {
    /**
     * first scaled image using KeepAspectRatioByExpanding,
     * at this time, image is bigger than QSize()
     * then clip image to this->size()
     */
    if(pix_.height() < heightForWidth(pix_.width())) {
        QImage const &res = pix_.copy(0, 0, widthForHeight(pix_.height()), pix_.height());
        return QPixmap::fromImage(res);
    }
    QImage const &res = pix_.copy(0, 0, pix_.width(), heightForWidth(pix_.width()));
    // QImage const &res = pix_.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)
    //                         .copy(0, 0, size().width(), size().height());
    return QPixmap::fromImage(res);
}

void AspectRatioPixmapLabel::adjust() {
    // setScaledContents(true);
    // if((qreal)width() / height() == scaleFactor_) return;
    // scaleFactor_ = (qreal)width() / height();
    // if(this->hasScaledContents()) return;
    // if(!pix_.isNull()) QLabel::setPixmap(scaledPixmap());
    if(sizeFactor(size()) == scaleFactor_) {
        qDebug() << "sizeFactor has not changed: " << sizeFactor(size());
        return;
    }
    qDebug() << "scaleFactor Changed from " << scaleFactor_ << " to " << sizeFactor(size());
    scaleFactor_ = sizeFactor(size());
    setPixmap(scaledPixmap());
}
int AspectRatioPixmapLabel::heightForWidth(int w) const {
    // height = width*(y/x)
    // ry/rx=y/x ==> ry=rx*y/x
    // height = width * sizeHint().rheight() / sizeHint().rwidth();
    return w * sizeHint().rheight() / sizeHint().rwidth();
}

int AspectRatioPixmapLabel::widthForHeight(int h) const {
    // rw/rh = w/h ==> w = rw/rh * h
    return h * sizeHint().width() / sizeHint().rheight();
}
