/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-09-08
 * Description : RAW Import digiKam plugin definition.
 *
 * Copyright (C) 2020 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_DPLUGIN_RAW_IMPORT_H
#define DIGIKAM_DPLUGIN_RAW_IMPORT_H

// Local includes

#include "dplugin.h"
#include "dimg.h"
#include "loadingdescription.h"
#include "dpluginloader.h"
#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT DPluginRawImport : public DPlugin
{
    Q_OBJECT

public:

    /**
     * Constructor with optional parent object
     */
    explicit DPluginRawImport(QObject* const parent = nullptr);

    /**
     * Destructor
     */
    ~DPluginRawImport() override;

public:

    /**
     * This kind of plugin only provide one tool.
     */
    int count() const override { return 1; };

    /**
     * This kind of plugin do not use a category.
     */
    QStringList categories() const override { return QStringList(); };

    /**
     * This kind of plugin do not have GUI visibility attribute.
     */
    void setVisible(bool) override {};

    /**
     * Return the plugin interface identifier.
     */
    QString ifaceIid() const override { return QLatin1String(DIGIKAM_DPLUGIN_RAWIMPORT_IID); };

public:

    /**
     * Function to re-implement to invoke Raw processor for a Raw file path.
     * Default Raw decoding settings is also availalble.
     */
    virtual bool run(const QString& path, const DRawDecoding& def) = 0;

Q_SIGNALS:

    /**
     * Signal to emit to notify host application to load Raw with these decoding settings.
     */
    void signalLoadRaw(const Digikam::LoadingDescription&);

    /**
     * Signal to emit to notify host application to load pre-decoded Raw preprocessed with these decoding settings.
     */
    void signalDecodedImage(const Digikam::LoadingDescription&, const Digikam::DImg&);
};

} // namespace Digikam

Q_DECLARE_INTERFACE(Digikam::DPluginRawImport, DIGIKAM_DPLUGIN_RAWIMPORT_IID)

#endif // DIGIKAM_DPLUGIN_RAW_IMPORT_H
