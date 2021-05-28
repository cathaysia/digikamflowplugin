#ifndef DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN
#define DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN

#include <digikam/coredbaccess.h>
#include <digikam/dplugingeneric.h>

#include <flowlayout.h>
#include <semaphore>

#define DPLUGIN_IID "org.kde.digikam.plugin.generic.FlowView"

using namespace Digikam;

namespace Cathaysia {

class PicFlowView : public DPluginGeneric {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DPLUGIN_IID)
    Q_INTERFACES(Digikam::DPluginGeneric)

public:
    explicit PicFlowView(QObject* const parent = nullptr);
    ~PicFlowView() noexcept;
    QString              name() const override;
    QString              iid() const override;
    QIcon                icon() const override;
    QString              details() const override;
    QString              description() const override;
    QList<DPluginAuthor> authors() const override;

    void setup(QObject* const) override;


signals:
    void imagePathResponse(const QString path);

protected:
    void flowView();
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    /**
     * @brief 图片的参考宽度
     *
     */
    qreal            width_ = 300;
    QDialog*         main_dialog_;
    Z::FlowLayout*   content_;
    std::atomic_bool stop_ = false;
};

}    // namespace Cathaysia

#endif    // DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN
