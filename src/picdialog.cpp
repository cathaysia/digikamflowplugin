#include "picdialog.hpp"

#include <QLabel>
#include <QScrollArea>

PicDialog::PicDialog(QWidget* parent) : QDialog(parent) {
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->installEventFilter(this);
    this->setLayout(new QHBoxLayout);

    auto scrollWidget = new QScrollArea(this);
    layout()->addWidget(scrollWidget);
    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    box_ = new QWidget(this);
    scrollWidget->setWidget(box_);

    layout_ = new Z::FlowLayout(box_);
    box_->setLayout(layout_);

    layout_->setSpacing(spacing_);

    // Set ReferenceWidth
    layout_->setWidgetWidth(referenceWidth_);
}
PicDialog::~PicDialog() { }

void PicDialog::setWidgetWidth(qreal width) {
    layout_->setWidgetWidth(width);
    referenceWidth_ = width;
}

void PicDialog::setSpacing(int spacing) {
    layout_->setSpacing(spacing);
}

void PicDialog::loadPic(const LoadingDescription&, const DImg& dimg) {
    auto* lbl = new QLabel;
    auto  pix = dimg.convertToPixmap();
    if(pix.width() * pix.height() > 1920 * 1080)
        pix = pix.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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

LoadingDescription PicDialog::createLoadingDescription(const QString& filePath) {

    LoadingDescription description(filePath, PreviewSettings::fastPreview(), 0);

    if(DImg::fileFormat(filePath) == DImg::RAW) {
        description.rawDecodingSettings.optimizeTimeLoading();
        description.rawDecodingSettings.rawPrm.sixteenBitsImage   = false;
        description.rawDecodingSettings.rawPrm.halfSizeColorImage = true;
        description.rawDecodingHint                               = LoadingDescription::RawDecodingTimeOptimized;
    }

    description.postProcessingParameters.colorManagement = LoadingDescription::ConvertForDisplay;

    return description;
}
