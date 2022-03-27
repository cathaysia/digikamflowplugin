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

// Qt
#include <QInputDialog>
#include <QMenu>

#include "picdialog.hpp"
#include "plugflow.hpp"
#include "plugsettings.hpp"

namespace Cathaysia {

void logHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    QByteArray  localMsg = msg.toLocal8Bit();
    const char* file     = context.file ? context.file : "";
    const char* function = context.function ? context.function : "";

    switch(type) {
        case QtDebugMsg:
            fprintf(stderr,
                    "cathaysia.digikam.flowview(Debug): %s (%s:%u, %s)\n",
                    localMsg.constData(),
                    file,
                    context.line,
                    function);
            break;
        case QtInfoMsg:
            fprintf(stderr,
                    "cathaysia.digikam.flowview(Info): %s (%s:%u, %s)\n",
                    localMsg.constData(),
                    file,
                    context.line,
                    function);
            break;
        case QtWarningMsg:
            fprintf(stderr,
                    "cathaysia.digikam.flowview(Warning): %s (%s:%u, %s)\n",
                    localMsg.constData(),
                    file,
                    context.line,
                    function);
            break;
        case QtCriticalMsg:
            fprintf(stderr,
                    "cathaysia.digikam.flowview(Critical): %s (%s:%u, %s)\n",
                    localMsg.constData(),
                    file,
                    context.line,
                    function);
            break;
        case QtFatalMsg:
            fprintf(stderr,
                    "cathaysia.digikam.flowview(Fatal): %s (%s:%u, %s)\n",
                    localMsg.constData(),
                    file,
                    context.line,
                    function);
            break;
    }
}

FlowPlugin::FlowPlugin(QObject* const parent) : DPluginGeneric(parent), iface_(nullptr), settings_(nullptr) {
    settings_ = new PlugSettings(nullptr);
    settings_->setPlugin(this);

    qInstallMessageHandler(logHandler);
}

FlowPlugin::~FlowPlugin() noexcept {
    // BUG: this code makes digikam core dump
    // delete settings_;
}

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
                             QString::fromUtf8("(c) 2021"));
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
    // configure
    auto configure = setting->addAction(tr("Configure"), [this]() {
        this->settings_->show();
    });
    ac->setMenu(setting);
    connect(ac, &DPluginAction::triggered, this, &FlowPlugin::flowView);
    addAction(ac);
    iface_ = infoIface(ac);
}

void FlowPlugin::flowView() {
    auto* dialog = new PicDialog;
    dialog->setSpacing(settings_->spacing());
    dialog->setReferenceWidth(settings_->referenceWidth());
    dialog->setStyle(settings_->style());

    connect(settings_, &PlugSettings::signalStyleChanged, dialog, &PicDialog::setStyle);
    connect(settings_, &PlugSettings::spacingChanged, dialog, &PicDialog::setSpacing);
    connect(settings_, &PlugSettings::refWidthChanged, dialog, &PicDialog::setReferenceWidth);

    dialog->resize(800, 600);
    dialog->show();

    auto items = iface_->currentAlbumItems();
    qDebug() << "These images will be loaded: " << items;
    for(auto& it: items) dialog->load(it, settings_->useCustomLoader());
}

}    // namespace Cathaysia
