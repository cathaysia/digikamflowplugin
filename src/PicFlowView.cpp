#include "PicFlowView.h"

#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMenu>
#include <QPointer>
#include <QScrollArea>
#include <QThread>
#include <QTranslator>

#include <digikam/coredbaccess.h>
#include <digikam/dbengineaction.h>
#include <digikam/dinfointerface.h>
#include <digikam/dmessagebox.h>

#include <flowlayout.h>

namespace Cathaysia {

PicFlowView::PicFlowView(QObject* const parent)
    : DPluginGeneric { parent }, main_dialog_ { new QDialog }, content_ { new Z::FlowLayout } {
    // 图片的容器的宽度与主窗口保持一致
    main_dialog_->installEventFilter(this);
    // 添加 QScrollArea 及其环境
    auto mainLayout   = new QHBoxLayout;
    auto scrollWidget = new QScrollArea;
    auto box          = new QWidget;

    main_dialog_->setLayout(mainLayout);
    mainLayout->addWidget(scrollWidget);
    box->setLayout(content_);

    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollWidget->setWidget(box);
    // 设置图片的理想宽度
    content_->setWidgetWidth(300);
}

PicFlowView::~PicFlowView() noexcept {
    if(main_dialog_) delete main_dialog_;
    if(content_) delete content_;
}

QString PicFlowView::name() const {
    return tr("PicFlowPlugin");
}

QString PicFlowView::iid() const {
    return QLatin1String(DPLUGIN_IID);
}

QIcon PicFlowView::icon() const {
    return QIcon::fromTheme(QLatin1String("digikam"));
}

QString PicFlowView::description() const {
    return tr("Show pictures in a mono flow view");
}

QString PicFlowView::details() const {
    return tr("A plugin private Flow view");
}

// clang-format off
QList<Digikam::DPluginAuthor> PicFlowView::authors() const {
    return QList<DPluginAuthor>()
            << DPluginAuthor(QString::fromUtf8("Cathaysia"),
                             QString::fromUtf8("319513897@outlook.com"),
                             QString::fromUtf8("(c) 2021"))
            ;
}
// clang-format on

bool PicFlowView::eventFilter(QObject* watched, QEvent* event) {
    if(event->type() == QEvent::Resize) {
        content_->parentWidget()->resize(main_dialog_->width(), content_->innerHeight());
        return true;
    }
    return false;
}

void PicFlowView::setup(QObject* const parent) {
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setObjectName(QLatin1String("PiclLowView"));
    ac->setActionCategory(DPluginAction::ActionCategory::GenericView);
    ac->setText("PicFlowView");
    connect(ac, &DPluginAction::triggered, this, &PicFlowView::flowView);
    addAction(ac);

}

void PicFlowView::flowView() {
    auto* const iface = infoIface(sender());
    // 首先清空容器内
    if(content_){
        auto parent = content_->parentWidget();
        parent->setLayout(nullptr);
        delete content_;
        content_ = new Z::FlowLayout;
        content_->setWidgetWidth(300);
        parent->setLayout(content_);
    }
    for(auto& item: iface->currentAlbumItems()) {
        QString imgPath = item.toString().replace("file://", "");
        QLabel* img     = new QLabel();
        img->setPixmap(QPixmap(imgPath));
        img->setScaledContents(true);
        content_->addWidget(img);
    }
    content_->parentWidget()->resize(800, content_->innerHeight());
    main_dialog_->resize(800, 600);
    main_dialog_->show();
}
}    // namespace Cathaysia
