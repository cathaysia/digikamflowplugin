/* ============================================================
 *
 * This file is a part of digiKam flow plugin project
 * https://github.com/cathaysia/digikamflowplugin
 *
 * Date        : 2021-05-22
 * Description : Flow View generic plugin for digiKam.
 *
 * Copyright (C) 2021-2022 by Cathaysia <DragonBillow at outlook dot com>
 *
 * Redistribution and use is allowed according to the terms of the GPL3 license.
 * For details see the accompanying LICENSE file.
 *
 * ============================================================ */

#pragma once

#include <digikam/dplugingeneric.h>
#include <flowlayout.h>

#define DPLUGIN_IID "org.kde.digikam.plugin.generic.FlowView"

using namespace Digikam;

namespace Cathaysia {

class PlugSettings;

class FlowPlugin : public DPluginGeneric {
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID DPLUGIN_IID);
    Q_INTERFACES(Digikam::DPluginGeneric);

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

protected:
    void flowView();

private:
    Digikam::DInfoInterface* iface_;
    PlugSettings*            settings_;
};

}    // namespace Cathaysia
