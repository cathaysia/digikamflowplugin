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

#include "aspectratiopixmaplabel.hpp"

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>

// Only three decimal place
inline qreal sizeFactor(QSize const &a) {
    if(!a.height() || !a.width()) return 0;
    return a.width() * 1000 / a.height() / 1000.f;
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
    if(heightForWidth(width()) < height()) {
        // width 相对变小
        auto tmp = pix_.scaled(widthForHeight(height()), height(), Qt::KeepAspectRatioByExpanding,
                               Qt::TransformationMode::FastTransformation);
        auto res = tmp.copy(0, 0, width(), height());
        return QPixmap::fromImage(res);
    }

    auto tmp = pix_.scaled(width(), heightForWidth(width()), Qt::KeepAspectRatioByExpanding,
                           Qt::TransformationMode::FastTransformation);
    auto res = tmp.copy(0, 0, width(), height());
    return QPixmap::fromImage(res);
}

void AspectRatioPixmapLabel::adjust() {
    if(pix_.isNull()) return;
    qreal scaleFactor = sizeFactor(size());
    if(!scaleFactor || (scaleFactor == scaleFactor_)) return;
    scaleFactor_ = scaleFactor;

    QLabel::setPixmap(scaledPixmap());
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
void AspectRatioPixmapLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    auto dialog = new QDialog;
    dialog->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose, true);
    dialog->setLayout(new QHBoxLayout);

    auto lbl = new QLabel(dialog);
    lbl->setPixmap(QPixmap::fromImage(this->pix_));
    lbl->setScaledContents(false);
    dialog->layout()->addWidget(lbl);

    dialog->showFullScreen();
}
