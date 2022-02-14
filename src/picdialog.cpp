#include "picdialog.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QUrl>

PicDialog::PicDialog(QWidget* parent)
    : QDialog(parent), box_(new QWidget(this)), layout_(new Z::FlowLayout(box_)), pool_(nullptr), t_(nullptr) {

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->installEventFilter(this);
    this->setLayout(new QHBoxLayout);

    auto scrollWidget = new QScrollArea(this);
    layout()->addWidget(scrollWidget);
    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollWidget->setWidget(box_);

    box_->setLayout(layout_);

    // Set ReferenceWidth
    layout_->setRefWidth(300);
    // clang-format off
    connect(this, &PicDialog::signalPixLoaded, this
            , QOverload<QPixmap const&>::of(&PicDialog::add)
            , Qt::QueuedConnection);
    connect(t_, &PreviewLoadThread::signalImageLoaded, this
            , QOverload<LoadingDescription const&, DImg const&>::of(&PicDialog::add)
            , Qt::QueuedConnection);
    // clang-format on
}

PicDialog::~PicDialog() {
    if(pool_) { pool_->waitForDone(1000); }

    if(t_) {
        t_->stop();
        t_->wait();
    }
}

void PicDialog::setReferenceWidth(qreal width) {
    layout_->setRefWidth(width);
}

qreal PicDialog::referenceWidth() {
    return layout_->refWidth();
}

void PicDialog::setSpacing(int spacing) {
    layout_->setSpacing(spacing);
}

int PicDialog::spacing() {
    return layout_->spacing();
}

void PicDialog::setStyle(Z::FlowLayout::Style sty) {
    layout_->setStyle(sty);
}

void PicDialog::add(LoadingDescription const& desc, DImg const& img) {
    if(img.isNull()) {
        qDebug() << "DImg " << desc.filePath << " load failed";
        return;
    }
    this->add(img.convertToPixmap());
}

void PicDialog::add(const QPixmap& pix) {
    if(pix.isNull()) return;
    auto* lbl = new QLabel;
    lbl->installEventFilter(this);
    lbl->setPixmap(pix);
    lbl->setScaledContents(true);
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
    return false;
}

void PicDialog::load(const QUrl& url, bool loadByPool) {
    if(loadByPool) {
        if(!pool_) {
            static QThreadPool pool;
            pool_ = &pool;
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
        return;
    }
    if(!t_) {
        static PreviewLoadThread t;
        t_ = &t;
    }
    const DImg& img = t_->loadHighQualitySynchronously(url.toLocalFile());
    if(img.isNull()) {
        qDebug() << "DImg " << url.toLocalFile() << " load failed";
        return;
    }
    this->add(img.convertToPixmap());
}
