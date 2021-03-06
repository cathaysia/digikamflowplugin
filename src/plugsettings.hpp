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
#pragma once

#include "dplugindialog.h"
#include "flowlayout.h"

class QComboBox;
class QSpinBox;
class QSettings;

namespace Cathaysia {
using namespace Digikam;

class PlugSettings : public DPluginDialog {
    Q_OBJECT;

public:
    explicit PlugSettings(QWidget* const parent = nullptr);
    bool     useCustomLoader();
    int      spacing();
    int      referenceWidth();
    Z::Style style();

    // QDialog
    void accept() override;
    void reject() override;

protected:
    QWidget* getStyleOption();
    QWidget* getLoaderOption();
    QWidget* getSpacingOption();
    QWidget* getRefWidthOption();

signals:
    void refWidthChanged(qreal width);
    void spacingChanged(int spacing);
    void signalStyleChanged(Z::Style);

private:
    QSettings* settings_;
    bool       useCustomLoader_;
};
}    // namespace Cathaysia
