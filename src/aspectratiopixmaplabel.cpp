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
    return QPixmap::fromImage(res);
}

void AspectRatioPixmapLabel::adjust() {
    if(pix_.isNull()) return;
    if(size().height() && (sizeFactor(size()) != scaleFactor_)) return;

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
