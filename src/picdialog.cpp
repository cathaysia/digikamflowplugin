#include "picdialog.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QUrl>

#include "aspectratiopixmaplabel.hpp"

#define INSERT_CANCEL_POINT                   \
    do {                                      \
        if(stop_) {                           \
            qDebug() << "Cancel load images"; \
            return;                           \
        }                                     \
    } while(0)

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
    this->stop_ = true;
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
    qDebug() << "set style";
    qApp->postEvent(this, new QResizeEvent(this->size(), this->size()));
    QResizeEvent e(this->size(), this->size());
}

void PicDialog::add(LoadingDescription const& desc, DImg const& dimg) {
    qApp->processEvents();
    if(dimg.isNull()) {
        qDebug() << "DImg " << desc.filePath << " load failed";
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
    this->adjust();
}

void PicDialog::adjust() {
    for(int i = 0; i < layout_->count(); ++i) {
        // adjust images's size
        auto lbl = qobject_cast<AspectRatioPixmapLabel*>(layout_->itemAt(i)->widget());
        if(!lbl) continue;
        lbl->adjust();
    }
}

// Update layout after the size of dialog has changed
bool PicDialog::eventFilter(QObject* watched, QEvent* event) {
    // eventFilter for this self
    auto dialog = qobject_cast<PicDialog*>(watched);
    if(dialog) {
        if(event->type() == QEvent::Resize) {
            box_->resize(dialog->width(), layout_->innerHeight());
            this->adjust();
            return false;
        }
        if(event->type() == QEvent::Close) {
            emit this->signalOnClose();
            return false;
        }
    }
    return false;
}

void PicDialog::load(const QUrl& url, bool loadByPool) {
    INSERT_CANCEL_POINT;
    if(!loadByPool) {
        if(!t_) {
            static PreviewLoadThread t;
            t_ = &t;
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
        DImg dimg(url.toLocalFile());
        INSERT_CANCEL_POINT;
        QPixmap pix = dimg.convertToPixmap();
        if(pix.isNull()) {
            qDebug() << "Image " << url.toLocalFile() << " load failed";
            return;
        }
        INSERT_CANCEL_POINT;
        if((pix.width() * pix.height() > 1920 * 1080))
            pix = pix.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        emit this->signalPixLoaded(pix);
    };
    pool_->start(std::bind(task, url));
}
