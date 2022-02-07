#include "picdialog.hpp"

#include <QApplication>
#include <QLabel>
#include <QScrollArea>
#include <QUrl>

PicDialog::PicDialog(QWidget* parent)
    : QDialog(parent)
    , spacing_(-1)
    , referenceWidth_(300)
    , box_(new QWidget(this))
    , layout_(new Z::FlowLayout(box_))
    , t(new PreviewLoadThread(this))
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
    // clang-format off
    connect(t, &PreviewLoadThread::signalImageLoaded,
            this, QOverload<LoadingDescription const&, DImg const&>::of(&PicDialog::add),
            Qt::QueuedConnection);
    connect(this, &PicDialog::signalPixLoaded,
            this, QOverload<QPixmap const&>::of(&PicDialog::add),
            Qt::QueuedConnection);
    // clang-format on
}
PicDialog::~PicDialog() {
    pool_->waitForDone(1000);
    delete pool_;
    t->stop();
    t->wait();
}

void PicDialog::setWidgetWidth(qreal width) {
    layout_->setRefWidth(referenceWidth_);
    referenceWidth_ = width;
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

void PicDialog::add(const LoadingDescription& desc, const DImg& dimg) {
    if(dimg.isNull()) {
        qDebug() << "file " << desc.filePath << " load be null in DImg";
        return;
    }
    this->add(dimg.convertToPixmap());
}

void PicDialog::add(const QPixmap& pix) {
    if(pix.isNull()) return;
    auto* lbl = new QLabel;
    lbl->setPixmap(pix);
    layout_->addWidget(lbl);
}

// Update layout after the size of dialog has changed
bool PicDialog::eventFilter(QObject* watched, QEvent* event) {
    auto dialog = qobject_cast<PicDialog*>(watched);
    if(!dialog) return false;
    if(event->type() == QEvent::Resize) {
        box_->resize(dialog->width(), layout_->innerHeight());
        return true;
    }
    if(event->type() == QEvent::Close) {
        emit this->signalOnClose();
        return true;
    }
    return false;
}

void PicDialog::load(const QUrl& url, bool loadByPool) {
    if(!loadByPool) {
        // t->load(this->createLoadingDescription(url.toLocalFile()));
        // BUG: close dialog before image not loaded fully will cause digikam core
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
        if((pix.width() * pix.height() > 1920 * 1080)) {
            //
            pix = pix.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        emit this->signalPixLoaded(pix);
    };
    pool_->start(std::bind(task, url));
}
LoadingDescription PicDialog::createLoadingDescription(const QString& filePath) {

    LoadingDescription description(filePath, PreviewSettings::fastPreview(), 1920);

    if(DImg::fileFormat(filePath) == DImg::RAW) {
        description.rawDecodingSettings.optimizeTimeLoading();
        description.rawDecodingSettings.rawPrm.sixteenBitsImage   = false;
        description.rawDecodingSettings.rawPrm.halfSizeColorImage = true;
        description.rawDecodingHint                               = LoadingDescription::RawDecodingTimeOptimized;
    }

    description.postProcessingParameters.colorManagement = LoadingDescription::ConvertForDisplay;

    return description;
}
