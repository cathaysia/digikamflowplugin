/* ============================================================
 *
 * This file is a part of digiKam flow plugin project
 * https://github.com/cathaysia/digikamflowplugin
 *
 * Date        : 2021-05-22
 * Description : Flow View generic plugin for digiKam.
 *
 * Copyright (C) 2021-2022 by Cathaysia <DragonBillow at outlook dot com>
 *
 * Redistribution and use is allowed according to the terms of the GPL3 license.
 * For details see the accompanying LICENSE file.
 *
 * ============================================================ */
#include "plugsettings.hpp"

#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMetaEnum>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QString>

namespace Cathaysia {

PlugSettings::PlugSettings(QWidget* const parent)
    : DPluginDialog(parent, QStringLiteral("FlowPlugSettings"))
    , settings_(new QSettings(QStringLiteral("cathaysia.digikam.flowview"), qApp->applicationName(), this))
    , style_(settings_->value("style", "Col").toString())
    , spacing_(settings_->value("spacing", 3).toInt())
    , useCustomLoader_(settings_->value("useCustomLoader", true).toBool())
    , refWidth_(settings_->value("refWidth", 300).toInt()) {

    m_buttons->addButton(QDialogButtonBox::Ok);
    m_buttons->addButton(QDialogButtonBox::Cancel);
    m_buttons->button(QDialogButtonBox::Ok)->setDefault(true);

    setLayout(new QVBoxLayout(this));

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
    settings_->setValue("spacing", spacing_);
    settings_->setValue("style", style_);
    settings_->setValue("useCustomLoader", useCustomLoader_);
    settings_->setValue("refWidth", refWidth_);

    emit spacingChanged(spacing());
    emit signalStyleChanged(style());
    emit refWidthChanged(referenceWidth());
    QDialog::accept();
}

void PlugSettings::reject() {
    // restore();
    style_           = settings_->value("style", "Col").toString();
    spacing_         = settings_->value("spacing", 3).toInt();
    useCustomLoader_ = settings_->value("useCustomLoader", true).toBool();
    refWidth_        = settings_->value("refWidth", 300).toInt();
    // TODO: reload setting-widget's value

    emit spacingChanged(spacing());
    emit signalStyleChanged(style());
    emit refWidthChanged(referenceWidth());
    QDialog::reject();
}

QWidget* PlugSettings::getStyleOption() {
    auto        styleBox = new QComboBox(this);
    QStringList list;
    list << tr("Row") << tr("Col") << tr("Square");
    styleBox->addItems(list);
    styleBox->setCurrentText(style_);
    connect(styleBox, &QComboBox::currentTextChanged, [this](QString const& result) {
        if(style_ == result) return;
        this->style_ = result;
        emit this->signalStyleChanged(this->style());
    });

    // connect(this, &PlugSettings::signalStyleChanged, [&](Z::Style style) {
    //     if(this->style() == Z::Style::Row) {
    //         styleBox->setCurrentText("Row");
    //     } else if(this->style() == Z::Style::Square) {
    //         styleBox->setCurrentText("Square");
    //     } else {
    //         styleBox->setCurrentText("Col");
    //     }
    // });

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
    list << tr("Custom Loader") << tr("Digikam Loader");
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

    // connect(this, &PlugSettings::signalLoaderChanged, [&](bool useCustomLoader) {
    //     if(this->useCustomLoader()) {
    //         loaderBox->setCurrentText("Custom Loader");
    //     } else {
    //         loaderBox->setCurrentText("Digikam Loader");
    //     }
    // });

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
    spinBox->setValue(this->spacing());
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        this->spacing_ = i;
        emit this->spacingChanged(this->spacing());
    });

    // connect(this, &PlugSettings::spacingChanged, [&](int spacing) {
    //     spinBox->setValue(this->spacing());
    // });
    QLabel* lbl = new QLabel(tr("Spacing"), this);
    lbl->setBuddy(spinBox);
    QWidget* w = new QWidget(this);
    w->setLayout(new QHBoxLayout(this));
    w->layout()->addWidget(lbl);
    w->layout()->addWidget(spinBox);
    return w;
}

QWidget* PlugSettings::getRefWidthOption() {
    auto spinBox = new QSpinBox(this);
    spinBox->setMinimum(1);
    spinBox->setMaximum(INT_MAX);
    spinBox->setValue(this->referenceWidth());
    spinBox->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        this->refWidth_ = i;
        emit this->refWidthChanged(this->referenceWidth());
    });

    // connect(this, &PlugSettings::refWidthChanged, [&](qreal refWidth) {
    //     spinBox->setValue(refWidth);
    // });
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
Z::Style PlugSettings::style() {
    if(style_ == "Row") {
        return Z::Style::Row;
    } else if(style_ == "Square") {
        return Z::Style::Square;
    } else {
        return Z::Style::Col;
    }
}
}    // namespace Cathaysia
