
// Qt
#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QImage>
#include <QImageReader>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QQueue>
#include <QScrollArea>
#include <QSemaphore>
#include <QThread>
#include <QThreadPool>
#include <QTranslator>

#include <digikam/managedloadsavethread.h>
#include <digikam/previewloadthread.h>

#include "flowplugin.hpp"
#include "picdialog.hpp"

namespace Cathaysia {

FlowPlugin::FlowPlugin(QObject* const parent) : DPluginGeneric { parent } { }

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
        bool ok = false;
        auto result =
            QInputDialog::getDouble(nullptr, tr("Input reference width"), tr("Reference Width"), 300, -1, 9999, 1, &ok);
        if(!ok) return;
        emit widthChanged(result);
    });
    widthAction->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    auto styleAction = setting->addAction(tr("Set style"), [this]() {
        bool        ok = false;
        QStringList list;
        list << tr("Row") << tr("Square") << tr("Col");
        auto result = QInputDialog::getItem(nullptr, tr("Choose style"), tr("style"), list);
        if(result == "Row") {
            this->style_ = Z::FlowLayout::Style::Row;
            emit this->signalStyleChanged(style_);
        } else if(result == "Square") {
            this->style_ = Z::FlowLayout::Style::Square;
            emit this->signalStyleChanged(style_);
        } else if(result == "Col") {
            this->style_ = Z::FlowLayout::Style::Col;
            emit this->signalStyleChanged(style_);
        }
    });
    // spacing
    auto spac = setting->addAction("Spacing", [this]() {
        bool ok     = false;
        auto result = QInputDialog::getInt(nullptr, tr("Input spacing"), tr("Spacing"), -1, 0, 9999, 1, &ok);
        if(!ok) return;
        emit spacingChanged(result);
    });
    ac->setMenu(setting);
    connect(ac, &DPluginAction::triggered, this, &FlowPlugin::flowView);
    addAction(ac);
    iface = infoIface(ac);
}

void FlowPlugin::flowView() {
    auto* dialog = new PicDialog;
    dialog->setStyle(style_);

    connect(this, &FlowPlugin::spacingChanged, dialog, &PicDialog::setSpacing);
    connect(this, &FlowPlugin::widthChanged, dialog, &PicDialog::setReferenceWidth);
    connect(this, &FlowPlugin::signalStyleChanged, dialog, &PicDialog::setStyle);

    dialog->resize(800, 600);
    dialog->show();

    auto items = iface->currentAlbumItems();
    for(auto& it: items) dialog->load(it, true);
}

}    // namespace Cathaysia
