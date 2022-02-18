#pragma once

#include <digikam/dplugindialog.h>
#include <flowlayout.h>

class QComboBox;
class QSpinBox;
class QSettings;

namespace Cathaysia {
using namespace Digikam;

class PlugSettings : public DPluginDialog {
    Q_OBJECT;

public:
    explicit PlugSettings(QWidget* const parent = nullptr);
    bool                 useCustomLoader();
    int                  spacing();
    int                  referenceWidth();
    Z::FlowLayout::Style style();

protected:
    QWidget* getStyleOption();
    QWidget* getLoaderOption();
    QWidget* getSpacingOption();
    QWidget* getRefWidthOption();

signals:
    void refWidthChanged(qreal width);
    void spacingChanged(int spacing);
    void signalStyleChanged(Z::FlowLayout::Style);

private:
    QSettings*           settings_;
    Z::FlowLayout::Style style_ = Z::FlowLayout::Style::Col;
};
}    // namespace Cathaysia
