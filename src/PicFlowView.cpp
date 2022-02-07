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

#include <digikam/dimg.h>

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

// Update layout after the size of dialog has changed
bool PicFlowView::eventFilter(QObject* watched, QEvent* event) {
    auto dialog = qobject_cast<QDialog*>(watched);
    if(!dialog) return false;
    if(event->type() == QEvent::Resize) {
        auto content = dialog->findChild<Z::FlowLayout*>("FlowLayout");
        content->parentWidget()->resize(dialog->width(), content->innerHeight());
        return true;
    } else if(event->type() == QEvent::Close) {
        // INFO: This button is global, so if close a opend window maybe cause all producer stop load
        // data
        stop_ = true;
        return true;
    } else if(event->type() == QEvent::Show) {
        stop_ = false;
        return true;
    }
    return false;
}
// Basic setting to plugin
void PicFlowView::setup(QObject* const parent) {
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setObjectName(QLatin1String("PiclLowView"));
    ac->setActionCategory(DPluginAction::ActionCategory::GenericView);
    ac->setText("PicFlowView");
    // add menu items
    auto setting = new QMenu;
    // ReferenceWidth setting
    auto widthAction = setting->addAction(tr("Set reference width"), [this]() {
        bool ok     = false;
        auto result = QInputDialog::getDouble(
            nullptr, tr("Input reference width"), tr("Reference Width"), width_, -1, 9999, 1, &ok);
        if(ok) {
            this->width_ = result;
            emit widthChanged(result);
        };
    });
    widthAction->setWhatsThis(tr("Set refenence width, picture will use it as it's width <b>as much as possible</b>."));
    // add this action for open plugin
    auto openthis = setting->addAction(tr("Open view"), this, &PicFlowView::flowView);
    // scale
    auto scaledAction = setting->addAction(tr("Scale Picture"), [this](bool enableIt) {
        this->enable_scaled_ = enableIt;
    });
    scaledAction->setCheckable(true);
    scaledAction->setChecked(true);
    // spacing
    auto spac = setting->addAction("Spacing", [this]() {
        bool ok     = false;
        auto result = QInputDialog::getInt(nullptr, tr("Input spacing"), tr("Spacing"), spacing_, 0, 9999, 1, &ok);
        if(ok) {
            this->spacing_ = result;
            emit spacingChanged(result);
        };
    });
    ac->setMenu(setting);
    connect(ac, &DPluginAction::triggered, this, &PicFlowView::flowView);
    addAction(ac);
    iface = infoIface(ac);
}

// Build a dialog
Cathaysia::PicFlowView::ShareData PicFlowView::getShareData() {
    // From outside to innner be:
    // mainDialog -> dialogLayout -> scrollWidget -> box -> flowLayout
    auto mainDialog   = new QDialog;
    auto dialogLayout = new QHBoxLayout(mainDialog);
    auto scrollWidget = new QScrollArea(mainDialog);
    auto box          = new QWidget(mainDialog);
    auto flowLayout   = new Z::FlowLayout(box);
    // Destory after close dialog
    mainDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    mainDialog->installEventFilter(this);

    dialogLayout->addWidget(scrollWidget);

    scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollWidget->setWidget(box);

    box->setLayout(flowLayout);

    flowLayout->setObjectName("FlowLayout");
    flowLayout->setSpacing(spacing_);

    // Set ReferenceWidth
    flowLayout->setWidgetWidth(width_);
    //
    connect(this, &PicFlowView::widthChanged, flowLayout, &Z::FlowLayout::setWidgetWidth);
    connect(this, &PicFlowView::spacingChanged, flowLayout, &Z::FlowLayout::setSpacing);
    return ShareData(mainDialog, flowLayout);
}

void PicFlowView::flowView() {
    /**
     * TODO: 1. Whether share mutex between multi dialog?
     * INFO: After some testing, I find it's not possible to provide customer come in critical
     * section by flags. So I push a empty QImage at the end of img queue. See it as a flag that customer should exit.
     */
    auto shareData  = getShareData();
    auto mainDialog = shareData.first;
    auto flowLayout = shareData.second;
    // Show earlier
    mainDialog->resize(800, 600);
    mainDialog->show();

    QQueue<QPixmap> imgBuf;

    QSemaphore semMutex(1);
    QSemaphore empty(10);
    QSemaphore full(0);
    // Lambda for execute task
    using imgIt = QList<QUrl>::Iterator;
    auto task   = ([&](const imgIt& begin, const imgIt& end) {
        std::for_each(begin, end, [&](auto const& item) {
            QString imgPath = item.toString().replace("file://", "");
            if(stop_) {
                qDebug() << "terminate producer thread 1";
                return;
            }
            qDebug() << "producer: load " << imgPath;
            // QPixmap has implicit data share, so smart ptr is not need here.
            DImg    img(imgPath);
            QPixmap pix = img.convertToPixmap();
            if(pix.isNull()) return;
            // Scale picture for reduce memory footprint
            // pix = pix.scaled(1344,756, Qt::KeepAspectRatio, Qt::FastTransformation).scaled(960,540,
            //         Qt::KeepAspectRatio, Qt::SmoothTransformation);
            // a smeller picture scale to 1920x1080 will need more memory
            if(enable_scaled_ && (pix.width() * pix.height() > 1928 * 1080))
                pix = pix.scaled(1920, 1080, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            if(stop_) {
                qDebug() << "terminate producer thread 2";
                return;
            }
            // After stop_ set to true, producer thread always stop at above.
            empty.acquire();
            semMutex.acquire();
            imgBuf.enqueue(pix);

            semMutex.release();
            full.release();
          });
    });

    // join can prevent digikam's close that cause by plugin close.
    std::jthread producer([&]() {
        QThreadPool localPool;
        auto        items = iface->currentAlbumItems();
        imgIt       end   = items.begin();
        size_t      step  = 3;    // per task thread can deal 3 pictures.
        while(end < items.end()) {
            localPool.start(std::bind(task, end, end + step > items.end() ? items.end() : end + step));
            end += step;
        }
        // for(auto const& item: iface->currentAlbumItems()) localPool.start(std::bind(task, item));
        qDebug() << "waitting for producer stop";
        localPool.waitForDone();
        QPixmap nullpix;
        imgBuf.push_back(nullpix);
        full.release();
        qDebug() << "producer thread complete";
    });

    // Add QImage to GUI in main thread
    size_t  counter = 0;
    QPixmap tmp;
    while(true) {
        qDebug() << "Consumption for the " << counter << " time";
        QLabel* img = new QLabel();
        // Enter critical section
        full.acquire();
        semMutex.acquire();
        tmp = imgBuf.dequeue();
        semMutex.release();
        empty.release();
        // Leave critical section

        if(tmp.isNull()) {
            qDebug() << "Find empty object, exit";
            break;
        }
        img->setPixmap(tmp);

        img->setScaledContents(true);
        flowLayout->addWidget(img);
        // avoid dialog lag
        qApp->processEvents();
        qDebug() << "Consumption for the " << counter << " time completeed.";
        ++counter;
    }
    qDebug() << "Pictures load completed";
}
}    // namespace Cathaysia
