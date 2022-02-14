#include "aspectratiopixmaplabel.hpp"

#include <QDebug>

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget *parent) : QLabel(parent) {
    setScaledContents(false);
}

void AspectRatioPixmapLabel::setPixmap(const QPixmap &p) {
    pix_ = p.toImage();
    QLabel::setPixmap(scaledPixmap());
}

QSize AspectRatioPixmapLabel::sizeHint() const {
    return pix_.size();
}

void AspectRatioPixmapLabel::setScaledContents(bool) {
    // do nothing
}

bool AspectRatioPixmapLabel::hasScaledContents() const {
    return false;
}

QPixmap AspectRatioPixmapLabel::scaledPixmap() const {
    /**
     * first scaled image using KeepAspectRatioByExpanding,
     * at this time, image is bigger than QSize()
     * then clip image to this->size()
     */
    QImage const &res = pix_.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)
                            .copy(0, 0, size().width(), size().height());
    return QPixmap::fromImage(res);
}

void AspectRatioPixmapLabel::adjust() {
    if(!pix_.isNull()) QLabel::setPixmap(scaledPixmap());
}