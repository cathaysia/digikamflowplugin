#include "plugsettings.hpp"

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

namespace Cathaysia {

// TODO: data persistence
PlugSettings::PlugSettings(QWidget* const parent) : DPluginDialog(parent, "FlowPlugSettings") {
    setLayout(new QVBoxLayout(this));

    layout()->addWidget(getStyleOption());
    layout()->addWidget(getSpacingOption());
    layout()->addWidget(getRefWidthOption());
    layout()->addWidget(getLoaderOption());
    resize(layout()->sizeHint());
}

QWidget* PlugSettings::getStyleOption() {
    auto        styleBox = new QComboBox(this);
    QStringList list;
    list << tr("Row") << tr("Col") << tr("Square");
    styleBox->addItems(list);
    styleBox->setCurrentText(tr("Col"));
    connect(styleBox, &QComboBox::currentTextChanged, [this](QString const& result) {
        if(result == tr("Row")) {
            this->style_ = Z::FlowLayout::Style::Row;
            emit this->signalStyleChanged(style_);
        } else if(result == tr("Square")) {
            this->style_ = Z::FlowLayout::Style::Square;
            emit this->signalStyleChanged(style_);
        } else if(result == tr("Col")) {
            this->style_ = Z::FlowLayout::Style::Col;
            emit this->signalStyleChanged(style_);
        }
    });
    QLabel* lbl = new QLabel(tr("Style"), this);
    lbl->setBuddy(styleBox);
    QWidget* w = new QWidget(this);
    w->setLayout(new QHBoxLayout(this));
    w->layout()->addWidget(lbl);
    w->layout()->addWidget(styleBox);
    return w;
}

QWidget* PlugSettings::getLoaderOption() {
    auto        loaderBox = new QComboBox(this);
    QStringList list;
    list << tr("Custom Loader") << tr("Digiakm Loader");
    loaderBox->addItems(list);
    loaderBox->setCurrentText(tr("Custom Loader"));
    loaderBox->setWhatsThis(
        tr("Custom Loader will cause this plugin can not"
           "get profilt from digikam cache, but it should be less bugs"));
    connect(loaderBox, &QComboBox::currentTextChanged, [this](QString const& result) {
        if(result == tr("Custom Loader")) {
            this->useCustomLoader_ = true;
        } else if(result == tr("Digikam")) {
            this->useCustomLoader_ = false;
        }
    });
    QLabel* lbl = new QLabel(tr("Image Loader"), this);
    lbl->setBuddy(loaderBox);
    QWidget* w = new QWidget(this);
    w->setLayout(new QHBoxLayout(this));
    w->layout()->addWidget(lbl);
    w->layout()->addWidget(loaderBox);
    return w;
};

QWidget* PlugSettings::getSpacingOption() {
    auto spinBox = new QSpinBox(this);
    spinBox->setMinimum(-1);
    spinBox->setMaximum(INT_MAX);
    spinBox->setValue(spacing_);
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        this->spacing_ = i;
        emit this->spacingChanged(i);
    });
    QLabel* lbl = new QLabel(tr("Spacing"), this);
    lbl->setBuddy(spinBox);
    QWidget* w = new QWidget(this);
    w->setLayout(new QHBoxLayout(this));
    w->layout()->addWidget(lbl);
    w->layout()->addWidget(spinBox);
    return w;
}

// TODO: can pixture's width be zero?
QWidget* PlugSettings::getRefWidthOption() {
    auto spinBox = new QSpinBox(this);
    spinBox->setMinimum(1);
    spinBox->setMaximum(INT_MAX);
    spinBox->setValue(refWidth_);
    spinBox->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        this->refWidth_ = i;
        emit this->refWidthChanged(refWidth_);
    });
    QLabel* lbl = new QLabel(tr("Reference width"), this);
    lbl->setBuddy(spinBox);
    QWidget* w = new QWidget(this);
    w->setLayout(new QHBoxLayout(this));
    w->layout()->addWidget(lbl);
    w->layout()->addWidget(spinBox);
    return w;
}
bool PlugSettings::useCustomLoader() {
    return useCustomLoader_;
}
int PlugSettings::spacing() {
    return spacing_;
}
int PlugSettings::referenceWidth() {
    return refWidth_;
}
Z::FlowLayout::Style PlugSettings::style() {
    return style_;
}
}    // namespace Cathaysia
