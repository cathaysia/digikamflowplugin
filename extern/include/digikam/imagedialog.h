/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2008-03-13
 * Description : image files selector dialog.
 *
 * Copyright (C) 2008-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_IMAGE_DIALOG_H
#define DIGIKAM_IMAGE_DIALOG_H

// Qt includes

#include <QUrl>
#include <QScrollArea>
#include <QFileIconProvider>
#include <QIcon>
#include <QFileInfo>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class LoadingDescription;
class ThumbnailImageCatcher;

class DIGIKAM_EXPORT ImageDialogPreview : public QScrollArea
{
    Q_OBJECT

public:

    explicit ImageDialogPreview(QWidget* const parent = nullptr);
    ~ImageDialogPreview() override;

    QSize sizeHint() const override;

public Q_SLOTS:

    void slotShowPreview(const QUrl& url);

private Q_SLOTS:

    void showPreview();
    void slotClearPreview();
    void slotThumbnail(const LoadingDescription& desc, const QPixmap& pix);

private:

    void resizeEvent(QResizeEvent* e) override;

private:

    class Private;
    Private* const d;
};

// ------------------------------------------------------------------------

class DIGIKAM_EXPORT DFileIconProvider : public QFileIconProvider
{

public:

    explicit DFileIconProvider();
    ~DFileIconProvider() override;

    QIcon icon(IconType type) const override;
    QIcon icon(const QFileInfo& info) const override;
/*
private:

    ThumbnailImageCatcher* m_catcher;
*/
};

// ------------------------------------------------------------------------

class DIGIKAM_EXPORT ImageDialog
{

public:

    explicit ImageDialog(QWidget* const parent,
                         const QUrl& url,
                         bool singleSelect=false,
                         const QString& caption=QString());
    ~ImageDialog();

    QUrl        url()         const;
    QList<QUrl> urls()        const;
    QStringList fileFormats() const;

    static QUrl        getImageURL(QWidget* const parent, const QUrl& url, const QString& caption=QString());
    static QList<QUrl> getImageURLs(QWidget* const parent, const QUrl& url, const QString& caption=QString());

private:

    // Disable
    ImageDialog(const ImageDialog&)            = delete;
    ImageDialog& operator=(const ImageDialog&) = delete;

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_IMAGE_DIALOG_H
