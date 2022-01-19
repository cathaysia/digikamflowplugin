#ifndef DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN
#define DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN

#include <digikam/dplugingeneric.h>
#include <digikam/loadingdescription.h>

#include <flowlayout.h>

#include <utility>

#define DPLUGIN_IID "org.kde.digikam.plugin.generic.FlowView"

using namespace Digikam;

namespace Cathaysia {

class PicFlowView : public DPluginGeneric {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DPLUGIN_IID)
    Q_INTERFACES(Digikam::DPluginGeneric)

public:
    typedef std::pair<QDialog*, Z::FlowLayout*> ShareData;
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
    void widthChanged(qreal width);
    void spacingChanged(int spacing);

protected:
    void flowView();

private:
    Digikam::DInfoInterface* iface = nullptr;
};

}    // namespace Cathaysia

#endif    // DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN
