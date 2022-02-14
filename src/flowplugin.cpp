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

// Qt
#include <QInputDialog>
#include <QMenu>

#include "flowplugin.hpp"
#include "picdialog.hpp"

namespace Cathaysia {

FlowPlugin::FlowPlugin(QObject* const parent)
    : DPluginGeneric(parent)
    , spacing_(2)
    , refWidth_(300)
    , useCustomLoader_(true)
    , iface_(nullptr)
    , style_(Z::FlowLayout::Style::Col) { }

FlowPlugin::~FlowPlugin() noexcept { }

QString FlowPlugin::name() const {
    return tr("PicFlowPlugin");
}

QString FlowPlugin::iid() const {
    return QLatin1String(DPLUGIN_IID);
}

QIcon FlowPlugin::icon() const {
    return QIcon::fromTheme(QLatin1String("digikam"));
#include <functional>
}

QString FlowPlugin::description() const {
    return tr("Show pictures in a mono flow view");
}

QString FlowPlugin::details() const {
    return tr("A plugin provide Flow view");
}

// clang-format off
QList<Digikam::DPluginAuthor> FlowPlugin::authors() const {
    return QList<DPluginAuthor>()
            << DPluginAuthor(QString::fromUtf8("Cathaysia"),
                             QString::fromUtf8("DragonBillow@outlook.com"),
                             QString::fromUtf8("(c) 2021"))
            ;
}
// clang-format on

// Basic setting to plugin
void FlowPlugin::setup(QObject* const parent) {
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setObjectName(QLatin1String("PiclLowView"));
    ac->setActionCategory(DPluginAction::ActionCategory::GenericView);
    ac->setText("FlowPlugin");
    // add menu items
    auto setting = new QMenu;
    // add this action for open plugin
    auto openthis = setting->addAction(tr("Open view"), this, &FlowPlugin::flowView);
    // ReferenceWidth setting
    auto widthAction = setting->addAction(tr("Set reference width"), [this]() {
        bool ok     = false;
        auto result = QInputDialog::getDouble(
            nullptr, tr("Input reference width"), tr("Reference Width"), refWidth_, -1, 9999, 1, &ok);
        if(!ok) return;
        this->refWidth_ = result;
        emit refWidthChanged(result);
    });
    widthAction->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    // style
    auto styleAction = setting->addAction(tr("Set style"), [this]() {
        bool        ok = false;
        QStringList list;
        list << tr("Row") << tr("Square") << tr("Col");
        auto result = QInputDialog::getItem(nullptr, tr("Choose style"), tr("style"), list);
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
    // spacing
    auto spac = setting->addAction("Spacing", [this]() {
        bool ok     = false;
        auto result = QInputDialog::getInt(nullptr, tr("Input spacing"), tr("Spacing"), -1, spacing_, 9999, 1, &ok);
        if(!ok) return;
        this->spacing_ = result;
        emit spacingChanged(result);
    });
    // Loader
    auto loader = setting->addAction(tr("Use custom loader"), [this]() {
        QStringList list;
        list << tr("Custom Loader") << tr("Digikam");
        auto result = QInputDialog::getItem(nullptr, tr("Choose Loader"), tr("Loader"), list);
        if(result == tr("Custom Loader")) {
            this->useCustomLoader_ = true;
        } else if(result == tr("Digikam")) {
            this->useCustomLoader_ = false;
        }
    });
    loader->setWhatsThis(
        tr("Custom Loader will cause this plugin can not"
           "get profilt from digikam cache, but it should be less bugs"));
    ac->setMenu(setting);
    connect(ac, &DPluginAction::triggered, this, &FlowPlugin::flowView);
    addAction(ac);
    iface_ = infoIface(ac);
}

void FlowPlugin::flowView() {
    auto* dialog = new PicDialog;
    dialog->setSpacing(spacing_);
    dialog->setReferenceWidth(refWidth_);
    dialog->setStyle(style_);

    connect(this, &FlowPlugin::spacingChanged, dialog, &PicDialog::setSpacing);
    connect(this, &FlowPlugin::refWidthChanged, dialog, &PicDialog::setReferenceWidth);
    connect(this, &FlowPlugin::signalStyleChanged, dialog, &PicDialog::setStyle);

    dialog->resize(800, 600);
    dialog->show();

    auto items = iface_->currentAlbumItems();
    for(auto& it: items) dialog->load(it, useCustomLoader_);
}

}    // namespace Cathaysia
