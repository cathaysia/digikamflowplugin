#include "PicFlowView.hpp"

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

#include "PicDialog.hpp"

namespace Cathaysia {

PicFlowView::PicFlowView(QObject* const parent) : DPluginGeneric { parent } { }

PicFlowView::~PicFlowView() noexcept { }

QString PicFlowView::name() const {
    return tr("PicFlowPlugin");
}

QString PicFlowView::iid() const {
    return QLatin1String(DPLUGIN_IID);
}

QIcon PicFlowView::icon() const {
    return QIcon::fromTheme(QLatin1String("digikam"));
#include <functional>
}

QString PicFlowView::description() const {
    return tr("Show pictures in a mono flow view");
}

QString PicFlowView::details() const {
    return tr("A plugin provide Flow view");
}

// clang-format off
QList<Digikam::DPluginAuthor> PicFlowView::authors() const {
    return QList<DPluginAuthor>()
            << DPluginAuthor(QString::fromUtf8("Cathaysia"),
                             QString::fromUtf8("DragonBillow@outlook.com"),
                             QString::fromUtf8("(c) 2021"))
            ;
}
// clang-format on

// Basic setting to plugin
void PicFlowView::setup(QObject* const parent) {
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setObjectName(QLatin1String("PiclLowView"));
    ac->setActionCategory(DPluginAction::ActionCategory::GenericView);
    ac->setText("PicFlowView");
    // add menu items
    auto setting = new QMenu;
    // add this action for open plugin
    auto openthis = setting->addAction(tr("Open view"), this, &PicFlowView::flowView);
    // ReferenceWidth setting
    auto widthAction = setting->addAction(tr("Set reference width"), [this]() {
        bool ok = false;
        auto result =
            QInputDialog::getDouble(nullptr, tr("Input reference width"), tr("Reference Width"), 300, -1, 9999, 1, &ok);
        if(!ok) return;
        emit widthChanged(result);
    });
    widthAction->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    // spacing
    auto spac = setting->addAction("Spacing", [this]() {
        bool ok     = false;
        auto result = QInputDialog::getInt(nullptr, tr("Input spacing"), tr("Spacing"), -1, 0, 9999, 1, &ok);
        if(!ok) return;
        emit spacingChanged(result);
    });
    ac->setMenu(setting);
    connect(ac, &DPluginAction::triggered, this, &PicFlowView::flowView);
    addAction(ac);
    iface = infoIface(ac);
}

void PicFlowView::flowView() {
    auto* dialog = new PicDialog;

    connect(this, &PicFlowView::spacingChanged, dialog, &PicDialog::setSpacing);
    connect(this, &PicFlowView::widthChanged, dialog, &PicDialog::setWidgetWidth);

    dialog->resize(800, 600);
    dialog->show();
    /**
     * So, the steps for using ManagedLoadSaveThread:
     * 1. create ManagedLoadSaveThread
     * 2. call ManagedLoadSaveThread::load
     * 3. drag data by signal: ManagedLoadSaveThread::signalImageLoaded(LoadingDescription, DImg)
     */

    ManagedLoadSaveThread* t = new ManagedLoadSaveThread(dialog);
    connect(t, &ManagedLoadSaveThread::signalImageLoaded, dialog, &PicDialog::loadPic);
    connect(dialog, &PicDialog::close, t, &ManagedLoadSaveThread::stopAllTasks);
    auto items = iface->currentAlbumItems();
    for(auto& it: items) t->load(dialog->createLoadingDescription(it.toString().replace("file://", "")));
}

}    // namespace Cathaysia
