/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2018-07-30
 * Description : Batch Queue Manager digiKam plugin definition.
 *
 * Copyright (C) 2018-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef DIGIKAM_DPLUGIN_BQM_H
#define DIGIKAM_DPLUGIN_BQM_H

// Qt includes

#include <QWidget>

// Local includes

#include "dplugin.h"
#include "dpluginloader.h"
#include "batchtool.h"
#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_GUI_EXPORT DPluginBqm : public DPlugin
{
    Q_OBJECT

public:

    /**
     * Constructor with optional parent object
     */
    explicit DPluginBqm(QObject* const parent = nullptr);

    /**
     * Destructor
     */
    ~DPluginBqm()                                             override;

public:

    bool hasVisibilityProperty()                        const override;

    /**
     * Holds whether the plugin can be seen in parent view.
     */
    void setVisible(bool b)                                   override;

    /**
     * Return all plugin tools registered in setup() method with addTool() for a given parent.
     */
    QList<BatchTool*> tools(QObject* const parent)      const;

    /**
     * Return the amount of tools registered.
     */
    int count()                                         const override;

    /**
     * Return a plugin tool instance found by name in plugin tools list for a given parent.
     */
    BatchTool* findToolByName(const QString& name,
                              QObject* const parent)    const;

    /**
     * Return a list of batch tool group categories as strings registered in this plugin.
     */
    QStringList categories()                            const override;

    /**
     * Return the plugin interface identifier.
     */
    QString ifaceIid()                                  const override
    {
        return QLatin1String(DIGIKAM_DPLUGIN_BQM_IID);
    };

Q_SIGNALS:

    void signalVisible(bool);

protected:

    void addTool(BatchTool* const t);

public:

    /**
     * Return the info interface instance.
     */
    DInfoInterface* infoIface() const;

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

Q_DECLARE_INTERFACE(Digikam::DPluginBqm, DIGIKAM_DPLUGIN_BQM_IID)

#endif // DIGIKAM_DPLUGIN_BQM_H
