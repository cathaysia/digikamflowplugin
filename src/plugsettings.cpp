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
#include "plugsettings.hpp"

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMetaEnum>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QString>

QWidget* ARRANGE_WIDGET(const QString& lblText, QWidget* widget, QWidget* parent) {
    QLabel* lbl = new QLabel(lblText, parent);
    lbl->setBuddy(widget);
    QWidget* w = new QWidget(parent);
    w->setLayout(new QHBoxLayout);
    w->layout()->addWidget(lbl);
    w->layout()->addWidget(widget);
    return w;
}

namespace Cathaysia {

QComboBox* styleBox    = nullptr;
QComboBox* loaderBox   = nullptr;
QSpinBox*  spacingSpin = nullptr;
QSpinBox*  refSpin     = nullptr;

inline const QString strLoaderCustom() {
    return QObject::tr("Custom Loader");
}

inline const QString strLoaderDigikam() {
    return QObject::tr("Digikam Loader");
}

PlugSettings::PlugSettings(QWidget* const parent)
    : DPluginDialog(parent, QStringLiteral("FlowPlugSettings"))
    , settings_(new QSettings(QStringLiteral("cathaysia.digikam.flowview"), qApp->applicationName(), this))
    , useCustomLoader_(settings_->value("useCustomLoader", false).toBool()) {

    m_buttons->addButton(QDialogButtonBox::Ok);
    m_buttons->addButton(QDialogButtonBox::Cancel);
    m_buttons->button(QDialogButtonBox::Ok)->setDefault(true);

    setLayout(new QVBoxLayout);

    layout()->addWidget(getStyleOption());
    layout()->addWidget(getSpacingOption());
    layout()->addWidget(getRefWidthOption());
    layout()->addWidget(getLoaderOption());
    layout()->addWidget(m_buttons);
    resize(layout()->sizeHint());

    connect(m_buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &PlugSettings::accept);
    connect(m_buttons->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &PlugSettings::reject);

    // restore();
}

void PlugSettings::accept() {
    // store();
    settings_->setValue("spacing", spacingSpin->value());
    settings_->setValue("style", styleBox->currentText());
    settings_->setValue("useCustomLoader", useCustomLoader_);
    settings_->setValue("refWidth", refSpin->value());

    emit spacingChanged(spacing());
    emit signalStyleChanged(style());
    emit refWidthChanged(referenceWidth());
    QDialog::accept();
}

void PlugSettings::reject() {
    // // restore();
    // style_           = settings_->value("style", "Col").toString();
    // spacing_         = settings_->value("spacing", 3).toInt();
    // useCustomLoader_ = settings_->value("useCustomLoader", false).toBool();
    // refWidth_        = settings_->value("refWidth", 300).toInt();
    // reload setting-widget's value
    styleBox->setCurrentText(style());

    if(useCustomLoader_) loaderBox->setCurrentText(strLoaderCustom());
    else
        loaderBox->setCurrentText(strLoaderDigikam());

    spacingSpin->setValue(spacing());
    refSpin->setValue(referenceWidth());

    emit spacingChanged(spacing());
    emit signalStyleChanged(style());
    emit refWidthChanged(referenceWidth());
    QDialog::reject();
}

QWidget* PlugSettings::getStyleOption() {
    styleBox = new QComboBox(this);
    QStringList list;
    list << tr("Row") << tr("Col") << tr("Square");
    styleBox->addItems(list);
    styleBox->setCurrentText(style());
    connect(styleBox, &QComboBox::currentTextChanged, [this](QString const& result) {
        if(result == style()) return;
        emit this->signalStyleChanged(result);
    });

    return ARRANGE_WIDGET(tr("Style"), styleBox, this);
}

QWidget* PlugSettings::getLoaderOption() {
    loaderBox = new QComboBox(this);
    QStringList list;
    list << strLoaderCustom() << strLoaderDigikam();
    loaderBox->addItems(list);

    if(useCustomLoader()) loaderBox->setCurrentText(strLoaderCustom());
    else
        loaderBox->setCurrentText(strLoaderDigikam());

    loaderBox->setWhatsThis(
        tr("Custom Loader will cause this plugin can not"
           "get profilt from digikam cache, but it should be less bugs"));

    connect(loaderBox, &QComboBox::currentTextChanged, [this](QString const& result) {
        if(result == strLoaderDigikam()) this->useCustomLoader_ = false;
        else
            this->useCustomLoader_ = true;
    });

    return ARRANGE_WIDGET(tr("Image Loader"), loaderBox, this);
};

QWidget* PlugSettings::getSpacingOption() {
    spacingSpin = new QSpinBox(this);
    spacingSpin->setMinimum(-1);
    spacingSpin->setMaximum(INT_MAX);
    spacingSpin->setValue(spacing());
    connect(spacingSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        emit this->spacingChanged(i);
    });

    // connect(this, &PlugSettings::spacingChanged, [&](int spacing) {
    //     spinBox->setValue(this->spacing());
    // });

    return ARRANGE_WIDGET(tr("Spacing"), spacingSpin, this);
}

QWidget* PlugSettings::getRefWidthOption() {
    refSpin = new QSpinBox(this);
    refSpin->setMinimum(1);
    refSpin->setMaximum(INT_MAX);
    refSpin->setValue(this->referenceWidth());
    refSpin->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    connect(refSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        emit this->refWidthChanged(i);
    });

    return ARRANGE_WIDGET(tr("Reference widget"), refSpin, this);
}
bool PlugSettings::useCustomLoader() {
    return useCustomLoader_;
}
int PlugSettings::spacing() {
    return settings_->value("spacing", 3).toInt();
}
int PlugSettings::referenceWidth() {
    return settings_->value("refWidth", 300).toInt();
}
Z::Style PlugSettings::style() {
    return settings_->value("style", "Col").toString();
}
}    // namespace Cathaysia
