#ifndef DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN
#define DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN

#include <digikam/dplugingeneric.h>
#include <flowlayout.h>

#define DPLUGIN_IID "org.kde.digikam.plugin.generic.FlowView"

using namespace Digikam;

namespace Cathaysia {

class FlowPlugin : public DPluginGeneric {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DPLUGIN_IID)
    Q_INTERFACES(Digikam::DPluginGeneric)

public:
    explicit FlowPlugin(QObject* const parent = nullptr);
    ~FlowPlugin() noexcept;
    QString              name() const override;
    QString              iid() const override;
    QIcon                icon() const override;
    QString              details() const override;
    QString              description() const override;
    QList<DPluginAuthor> authors() const override;

    void setup(QObject* const) override;

signals:
    void refWidthChanged(qreal width);
    void spacingChanged(int spacing);
    void signalStyleChanged(Z::FlowLayout::Style);

protected:
    void flowView();

private:
    // using QThreadPool loader but not digikam
    int                      spacing_;
    qreal                    refWidth_;
    bool                     useCustomLoader_;
    Digikam::DInfoInterface* iface_;
    Z::FlowLayout::Style     style_;
};

}    // namespace Cathaysia

#endif    // DIGIKAMFLOWPLUGIN_DIGIKAMFLOWPLUGIN
