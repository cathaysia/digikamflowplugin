#include "PicFlowView.h"

// 标准库
#include <chrono>
#include <list>
#include <semaphore>

// Qt 库
#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QImageReader>
#include <QInputDialog>
#include <QLabel>
#include <QList>
#include <QMenu>
#include <QPointer>
#include <QScrollArea>
#include <QThread>
#include <QTranslator>

// digikam 库
#include <digikam/coredbaccess.h>
#include <digikam/dbengineaction.h>
#include <digikam/dinfointerface.h>
#include <digikam/dmessagebox.h>

// 本地库
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

    content_->setParent(box);

    main_dialog_->setLayout(mainLayout);
    mainLayout->addWidget(scrollWidget);
    box->setLayout(content_);

    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollWidget->setWidget(box);
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
    // 添加菜单项
    auto setting     = new QMenu;
    auto widthAction = setting->addAction(tr("设置参考宽度"));
    widthAction->setWhatsThis(tr("设置图片的参考宽度，图片的宽度会在更<b>倾向于</b>选择此宽度"));
    // 添加设置
    connect(widthAction, &QAction::triggered, [this]() {
        bool ok     = false;
        auto result = QInputDialog::getDouble(nullptr, tr("输入参考宽度"), tr("参考宽度"), 300, 10, 9999, 1, &ok);
        if(ok) this->width_ = result;
    });
    ac->setMenu(setting);
    connect(ac, &DPluginAction::triggered, this, &PicFlowView::flowView);
    addAction(ac);
}

void PicFlowView::flowView() {
    auto* const iface = infoIface(sender());
    // 设置图片的参考宽度
    content_->setWidgetWidth(width_);
    // 首先清空容器内的元素
    while(content_->list().length()) { content_->takeAt(0); }
    // 先显示
    content_->parentWidget()->resize(800, content_->innerHeight());
    main_dialog_->resize(800, 600);
    main_dialog_->show();
    /**
     * @todo
     *  QImage 的生成放在子线程中
     * 可以使用生产者-消费者
     */
    std::list<QPixmap> imgBuf;
    std::atomic_bool   over = false;

    std::binary_semaphore semMutex(1);
    std::binary_semaphore empty(10);
    std::binary_semaphore full(0);

    // 生产者线程
    std::thread producer([&semMutex, &empty, &full, &imgBuf, &iface, &over]() {
        for(auto& item: iface->currentAlbumItems()) {
            empty.acquire();
            semMutex.acquire();

            QString imgPath = item.toString().replace("file://", "");
            QPixmap pix(imgPath);
            if(!pix.isNull()) {
                imgBuf.push_back(pix);
                semMutex.release();
                // 只有在图片加载的时候才释放 full
                full.release();
            } else {
                semMutex.release();
            }
        }
        over = true;
    });
    producer.detach();
    /**
     * @todo 修复 bug
     *
     * 现在存在第二次添加的图层重叠的问题
     *
     */
    // 先对数据检查一遍
    bool hasVaildImg    = false;
    auto supportFormats = QImageReader::supportedImageFormats();
    qDebug() << supportFormats;
    for(auto& item: iface->currentAlbumItems()) {
        for(auto& suffix: supportFormats) {
            hasVaildImg = item.toString().endsWith(suffix);
            if(hasVaildImg) break;
        }
        if(hasVaildImg) break;
    }
    qDebug() << hasVaildImg;

    if(!hasVaildImg) return;
    // GUI 消费线程
    while(!over || imgBuf.size()) {
        QLabel* img = new QLabel();
        // 进入临界区
        full.acquire();
        semMutex.acquire();

        img->setPixmap(imgBuf.front());
        imgBuf.pop_front();

        semMutex.release();
        empty.release();
        // 离开临界区
        img->setScaledContents(true);
        content_->addWidget(img);
        // 防止界面卡顿
        qApp->processEvents();
    }
}
}    // namespace Cathaysia
