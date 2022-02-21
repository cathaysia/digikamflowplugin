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

#include <QComboBox>
#include <QLabel>
#include <QMetaEnum>
#include <QSettings>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>

namespace Cathaysia {

PlugSettings::PlugSettings(QWidget* const parent)
    : DPluginDialog(parent, "FlowPlugSettings")
    , settings_(new QSettings("cathaysia.digikam.flowview", QSettings::IniFormat, this)) {

    m_buttons->addButton(QDialogButtonBox::Ok);
    m_buttons->button(QDialogButtonBox::Ok)->setDefault(true);

    setLayout(new QVBoxLayout(this));

    layout()->addWidget(getStyleOption());
    layout()->addWidget(getSpacingOption());
    layout()->addWidget(getRefWidthOption());
    layout()->addWidget(getLoaderOption());
    layout()->addWidget(m_buttons);
    resize(layout()->sizeHint());

    connect(m_buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(accept()));
}

QWidget* PlugSettings::getStyleOption() {
    auto        styleBox = new QComboBox(this);
    QStringList list;
    list << tr("Row") << tr("Col") << tr("Square");
    styleBox->addItems(list);
    styleBox->setCurrentText(tr("Col"));
    connect(styleBox, &QComboBox::currentTextChanged, [this](QString const& result) {
        settings_->setValue("style", result);
        emit this->signalStyleChanged(this->style());
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
            settings_->setValue("useCustomLoader", true);
        } else if(result == tr("Digikam")) {
            settings_->setValue("useCustomLoader", false);
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
    spinBox->setValue(this->spacing());
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        settings_->setValue("spacing", i);
        emit this->spacingChanged(this->spacing());
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
    spinBox->setValue(this->referenceWidth());
    spinBox->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int i) {
        settings_->setValue("refWidth", i);
        emit this->refWidthChanged(this->referenceWidth());
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
    return settings_->value("useCustomLoader", true).toBool();
}
int PlugSettings::spacing() {
    return settings_->value("spacing", 3).toInt();
}
int PlugSettings::referenceWidth() {
    return settings_->value("refWidth", 300).toInt();
}
Z::FlowLayout::Style PlugSettings::style() {
    QString const& result = settings_->value("style", "Col").toString();
    if(result == "Row") {
        return Z::FlowLayout::Style::Row;
    } else if(result == "Square") {
        return Z::FlowLayout::Style::Square;
    } else {
        return Z::FlowLayout::Style::Col;
    }
}
}    // namespace Cathaysia
