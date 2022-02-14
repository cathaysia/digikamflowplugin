#include "picdialog.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QUrl>

PicDialog::PicDialog(QWidget* parent)
    : QDialog(parent)
    , spacing_(-1)
    , referenceWidth_(300)
    , box_(new QWidget(this))
    , layout_(new Z::FlowLayout(box_))
    , pool_(new QThreadPool) {

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->installEventFilter(this);
    this->setLayout(new QHBoxLayout);

    auto scrollWidget = new QScrollArea(this);
    layout()->addWidget(scrollWidget);
    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollWidget->setWidget(box_);

    box_->setLayout(layout_);

    layout_->setSpacing(spacing_);

    // Set ReferenceWidth
    layout_->setRefWidth(referenceWidth_);
    connect(this, &PicDialog::signalPixLoaded, this, &PicDialog::add, Qt::QueuedConnection);
}

PicDialog::~PicDialog() {
    pool_->waitForDone(1000);
    delete pool_;
}

void PicDialog::setReferenceWidth(qreal width) {
    referenceWidth_ = width;
    layout_->setRefWidth(referenceWidth_);
}

qreal PicDialog::referenceWidth() {
    return this->referenceWidth_;
}

void PicDialog::setSpacing(int spacing) {
    layout_->setSpacing(spacing);
}

int PicDialog::spacing() {
    return this->spacing_;
}

void PicDialog::setStyle(Z::FlowLayout::Style sty) {
    layout_->setStyle(sty);
}

void PicDialog::add(const QPixmap& pix) {
    if(pix.isNull()) return;
    auto* lbl = new QLabel;
    lbl->installEventFilter(this);
    lbl->setPixmap(pix);
    layout_->addWidget(lbl);
}

// Update layout after the size of dialog has changed
bool PicDialog::eventFilter(QObject* watched, QEvent* event) {
    // eventFilter for this self
    auto dialog = qobject_cast<PicDialog*>(watched);
    if(dialog) {
        if(event->type() == QEvent::Resize) {
            box_->resize(dialog->width(), layout_->innerHeight());
            return true;
        }
        if(event->type() == QEvent::Close) {
            emit this->signalOnClose();
            return true;
        }
    }
    // eventFilter for QLabel
    // auto lbl = qobject_cast<QLabel*>(watched);
    // if(lbl) {
    //     if(event->type() == QEvent::Resize) {
    //         // TODO: auto scale pixmap
    //         // lbl->setPixmap(lbl->pixmap(Qt::ReturnByValue).scaled(lbl->size(), Qt::KeepAspectRatio));
    //         return false;
    //     }
    // }
    return false;
}

void PicDialog::load(const QUrl& url, bool loadByPool) {
    if(!loadByPool) {
        // BUG: close dialog before image not loaded fully will cause digikam core
        // the reason is that PreviewLoadThread to slow when it cancel it task
        // So I want use t->load ..., but t->load can load one picture for every album
        DImg const& img = PreviewLoadThread::loadFastButLargeSynchronously(url.toLocalFile(), 1920 * 1080);
        emit this->signalPixLoaded(img.convertToPixmap());
        return;
    }
    auto task = [this](QUrl const& url) {
        QPixmap pix = DImg(url.toLocalFile()).convertToPixmap();
        if(pix.isNull()) {
            qDebug() << "Image " << url.toLocalFile() << " load failed";
            return;
        }
        if((pix.width() * pix.height() > 1920 * 1080))
            pix = pix.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        emit this->signalPixLoaded(pix);
    };
    pool_->start(std::bind(task, url));
}
