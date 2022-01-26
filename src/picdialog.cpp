#include "picdialog.hpp"

#include <QLabel>
#include <QScrollArea>
#include <QUrl>

PicDialog::PicDialog(QWidget* parent)
    : QDialog(parent)
    , spacing_(-1)
    , referenceWidth_(300)
    , box_(new QWidget(this))
    , layout_(new Z::FlowLayout(box_))
    , t(new PreviewLoadThread(this)) {

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
    connect(t, &PreviewLoadThread::signalImageLoaded, this, &PicDialog::loadPic);
}
PicDialog::~PicDialog() { }

void PicDialog::setWidgetWidth(qreal width) {
    layout_->setRefWidth(referenceWidth_);
    referenceWidth_ = width;
}

void PicDialog::setSpacing(int spacing) {
    layout_->setSpacing(spacing);
}

void PicDialog::loadPic(const LoadingDescription&, const DImg& dimg) {
    if(dimg.isNull()) { qDebug() << "DImg null"; }
    auto* lbl = new QLabel;
    auto  pix = dimg.convertToPixmap();
    if(pix.isNull()) return;
    qDebug() << pix;
    qDebug() << lbl->sizeHint();
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
        emit this->onClose();
        return true;
    }
    return false;
}

void PicDialog::load(const QUrl& url) {
    t->load(this->createLoadingDescription(url.toLocalFile()));
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
