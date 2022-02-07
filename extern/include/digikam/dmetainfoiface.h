/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2017-05-06
 * Description : interface to item information for shared tools
 *               based on DMetadata. This interface is used in all cases
 *               where no database is available (aka Showfoto).
 *
 * Copyright (C) 2017-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2019-2020 by Minh Nghia Duong <minhnghiaduong997 at gmail dot com>
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

#ifndef DIGIKAM_DMETA_INFO_IFACE_H
#define DIGIKAM_DMETA_INFO_IFACE_H

// Local includes

#include "digikam_export.h"
#include "dinfointerface.h"

namespace Digikam
{

class DIGIKAM_EXPORT DMetaInfoIface : public DInfoInterface
{
    Q_OBJECT

public:

    explicit DMetaInfoIface(QObject* const, const QList<QUrl>&);
    ~DMetaInfoIface()                                             override;

    Q_SLOT void slotDateTimeForUrl(const QUrl& url,
                                   const QDateTime& dt,
                                   bool updModDate)               override;

    Q_SLOT void slotMetadataChangedForUrl(const QUrl& url)        override;

    Q_SIGNAL void signalItemChanged(const QUrl& url);

public:

    QList<QUrl> currentSelectedItems()                      const override;
    QList<QUrl> currentAlbumItems()                         const override;
    QList<QUrl> allAlbumItems()                             const override;
    void        parseAlbumItemsRecursive()                        override;

    DInfoMap    itemInfo(const QUrl&)                       const override;
    void        setItemInfo(const QUrl&, const DInfoMap&)   const override;

    bool        supportAlbums()                             const override;

    QWidget* uploadWidget(QWidget* const parent)            const override;
    QUrl     uploadUrl()                                    const override;

    QUrl     defaultUploadUrl()                             const override;
    void     deleteImage(const QUrl& url)                         override;

    Q_SIGNAL void signalRemoveImageFromAlbum(const QUrl&);

#ifdef HAVE_MARBLE

    QList<GPSItemContainer*> currentGPSItems()              const override;

#endif

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_DMETA_INFO_IFACE_H
