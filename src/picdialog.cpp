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

#include "picdialog.hpp"

#include <QApplication>
#include <QImageReader>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QUrl>

#include "aspectratiopixmaplabel.hpp"
#include "digikam_debug.h"

#define INSERT_CANCEL_POINT                                               \
    do {                                                                  \
        if(stop_) {                                                       \
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Cancel load images"; \
            return;                                                       \
        }                                                                 \
    } while(0)

#define POST_RESIZE_EVENT(obj) (qApp->postEvent(obj, new QResizeEvent(this->size(), this->size())))

PicDialog::PicDialog(QWidget* parent)
    : QDialog(parent)
    , stop_(false)
    , box_(new QWidget(this))
    , layout_(new Z::FlowLayout(box_))
    , pool_(nullptr)
    , t_(nullptr) {

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->installEventFilter(this);
    this->setLayout(new QHBoxLayout);

    auto scrollWidget = new QScrollArea(this);
    layout()->addWidget(scrollWidget);
    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollWidget->setWidget(box_);

    box_->setLayout(layout_);

    // clang-format off
    connect(this, &PicDialog::signalPixLoaded, this
            , QOverload<QPixmap const&>::of(&PicDialog::add)
            , Qt::BlockingQueuedConnection);
    // clang-format on
}

PicDialog::~PicDialog() {
    this->stop_ = true;
    if(t_) t_->stop();
    if(pool_) pool_->waitForDone(1000);

    if(t_) t_->wait();
}

void PicDialog::setReferenceWidth(qreal width) {
    layout_->setRefWidth(width);
    POST_RESIZE_EVENT(this);
}

qreal PicDialog::referenceWidth() {
    return layout_->refWidth();
}

void PicDialog::setSpacing(int spacing) {
    layout_->setSpacing(spacing);
    POST_RESIZE_EVENT(this);
}

int PicDialog::spacing() {
    return layout_->spacing();
}

void PicDialog::setStyle(Z::Style sty) {
    layout_->setStyle(sty);
    POST_RESIZE_EVENT(this);
}

void PicDialog::add(LoadingDescription const& desc, DImg const& dimg) {
    qApp->processEvents();
    if(dimg.isNull()) {
        qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "DImg " << desc.filePath << " load failed";
        return;
    }
    INSERT_CANCEL_POINT;
    QPixmap img = dimg.convertToPixmap();
    INSERT_CANCEL_POINT;
    if((img.width() * img.height() > 1920 * 1080))
        img = img.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->add(img);
}

void PicDialog::add(const QPixmap& pix) {
    qApp->processEvents();
    if(pix.isNull()) return;
    INSERT_CANCEL_POINT;
    auto* lbl = new AspectRatioPixmapLabel;
    lbl->setPixmap(pix);
    layout_->addWidget(lbl);
    POST_RESIZE_EVENT(this);
}

// Update layout after the size of dialog has changed
bool PicDialog::eventFilter(QObject* watched, QEvent* event) {
    if(event->type() != QEvent::Resize) return false;
    auto dialog = qobject_cast<PicDialog*>(watched);
    if(!dialog) return false;

    for(int i = 0; i < layout_->count(); ++i) {
        auto lbl = qobject_cast<AspectRatioPixmapLabel*>(layout_->itemAt(i)->widget());
        if(!lbl) continue;
        lbl->adjust();
    }
    box_->resize(dialog->width(), layout_->innerHeight());
    return true;
}

void PicDialog::load(const QUrl& url, bool loadByPool) {
    INSERT_CANCEL_POINT;
    qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Load image: " << url.toLocalFile();
    if(!loadByPool) {
        if(!t_) {
            static PreviewLoadThread t;
            t_ = &t;
            // clang-format off
            connect(t_, &PreviewLoadThread::signalImageLoaded, this
                    , QOverload<LoadingDescription const&
                    , DImg const&>::of(&PicDialog::add)
                    , Qt::QueuedConnection);
            // clang-format on
        }
        // t_->load(url.toLocalFile(), PreviewSettings::fastPreview(), 1920);
        const DImg& dimg = t_->loadFastSynchronously(url.toLocalFile(), 1920);
        this->add(LoadingDescription(url.toLocalFile()), dimg);
        return;
    }
    // load by QThreadPool
    if(!pool_) {
        static QThreadPool pool;
        pool_ = &pool;
    }
    auto task = [this](QUrl const& url) {
        INSERT_CANCEL_POINT;
        QImageReader imgReader(url.toLocalFile());
        imgReader.setAutoTransform(true);
        QPixmap pix = QPixmap::fromImage(imgReader.read());
        if(pix.isNull()) {
            qCDebug(DIGIKAM_DPLUGIN_GENERIC_LOG) << "Image " << url.toLocalFile() << " load failed";
            return;
        }
        INSERT_CANCEL_POINT;
        if((pix.width() * pix.height() > 1920 * 1080))
            pix = pix.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        emit this->signalPixLoaded(pix);
    };
    pool_->start(std::bind(task, url));
}
