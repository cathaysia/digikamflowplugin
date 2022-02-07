/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2005-12-17
 * Description : image file IO threaded interface.
 *
 * Copyright (C) 2005-2011 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
 * Copyright (C) 2005-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_LOAD_SAVE_THREAD_H
#define DIGIKAM_LOAD_SAVE_THREAD_H

// Qt includes

#include <QThread>
#include <QMutex>
#include <QList>
#include <QDateTime>
#include <QWaitCondition>
#include <QEvent>

// Local includes

#include "dimg.h"
#include "digikam_export.h"
#include "dynamicthread.h"
#include "loadingdescription.h"

namespace Digikam
{

class DMetadata;
class LoadSaveTask;

class DIGIKAM_EXPORT LoadSaveNotifier
{
public:

    LoadSaveNotifier();
    virtual ~LoadSaveNotifier();

    virtual void imageStartedLoading(const LoadingDescription& loadingDescription)                  = 0;
    virtual void loadingProgress(const LoadingDescription& loadingDescription, float progress)      = 0;
    virtual void imageLoaded(const LoadingDescription& loadingDescription, const DImg& img)         = 0;
    virtual void moreCompleteLoadingAvailable(const LoadingDescription& oldLoadingDescription,
                                              const LoadingDescription& newLoadingDescription)      = 0;
    virtual void imageStartedSaving(const QString& filePath)                                        = 0;
    virtual void savingProgress(const QString& filePath, float progress)                            = 0;
    virtual void imageSaved(const QString& filePath, bool success)                                  = 0;
    virtual void thumbnailLoaded(const LoadingDescription& loadingDescription, const QImage& img)   = 0;

private:

    Q_DISABLE_COPY(LoadSaveNotifier)
};

// -------------------------------------------------------------------------------------------------------

class DIGIKAM_EXPORT LoadSaveFileInfoProvider
{
public:

    LoadSaveFileInfoProvider();
    virtual ~LoadSaveFileInfoProvider();

    /**
     * Gives a hint at the orientation of the image.
     * This can be used to supersede the Exif information in the file.
     * Will not be used if DMetadata::ORIENTATION_UNSPECIFIED (default value)
     */
    virtual int   orientationHint(const QString& path)  = 0;

    /**
     * Gives a hint at the size of the image.
     * This can be used to supersede the Exif information in the file.
     */
    virtual QSize dimensionsHint(const QString& path)   = 0;

private:

    Q_DISABLE_COPY(LoadSaveFileInfoProvider)
};

// -------------------------------------------------------------------------------------------------------

class DIGIKAM_EXPORT LoadSaveThread : public DynamicThread,
                                      public LoadSaveNotifier
{
    Q_OBJECT

public:

    enum NotificationPolicy
    {
        /**
         * Always send notification, unless the last event is still in the event queue
         */
        NotificationPolicyDirect,
        /**
         * Always wait for a certain amount of time after the last event sent.
         * In particular, the first event will be sent only after waiting for this time span.
         * (Or no event will be sent, when the loading has finished before)
         * This is the default.
         */
        NotificationPolicyTimeLimited
    };

    /**
     * used by SharedLoadSaveThread only
     */
    enum AccessMode
    {
        /**
         * image will only be used for reading
         */
        AccessModeRead,
        /**
         * image data will possibly be changed
         */
        AccessModeReadWrite
    };

public:

    explicit LoadSaveThread(QObject* const parent = nullptr);

    /**
     * Destructor:
     * The thread will execute all pending tasks and wait for this upon destruction
     */
    ~LoadSaveThread() override;

    /**
     * Append a task to load the given file to the task list
     */
    void load(const LoadingDescription& description);

    /**
     * Append a task to save the image to the task list
     */
    void save(const DImg& image, const QString& filePath, const QString& format);

    void setNotificationPolicy(NotificationPolicy notificationPolicy);

public:

    static void setInfoProvider(LoadSaveFileInfoProvider* const infoProvider);
    static LoadSaveFileInfoProvider* infoProvider();

    /**
     * Retrieves the Exif orientation, either from the info provider if available,
     * or from the metadata
     */
    static int exifOrientation(const QString& filePath,
                               const DMetadata& metadata,
                               bool isRaw,
                               bool fromRawEmbeddedPreview);

Q_SIGNALS:

    /**
     * All signals are delivered to the thread from where the LoadSaveThread object
     * has been created. This thread must use its event loop to get the signals.
     * You must connect to these signals with Qt::AutoConnection (default) or Qt::QueuedConnection.
     */

    /**
     * This signal is emitted when the loading process begins.
     */
    void signalImageStartedLoading(const LoadingDescription& loadingDescription);

    /**
     * This signal is emitted whenever new progress info is available
     * and the notification policy allows emitting the signal.
     * No progress info will be sent for preloaded images (ManagedLoadSaveThread).
     */
    void signalLoadingProgress(const LoadingDescription& loadingDescription, float progress);

    /**
     * This signal is emitted when the loading process has finished.
     * If the process failed, img is null.
     */
    void signalImageLoaded(const LoadingDescription& loadingDescription, const DImg& img);

    /**
     * This signal is emitted if
     *  - you are doing shared loading (SharedLoadSaveThread)
     *  - you started a loading operation with a LoadingDescription for
     *    a reduced version of the image
     *  - another thread started a loading operation for a more complete version
     * You may want to cancel the current operation and start with the given loadingDescription
     */
    void signalMoreCompleteLoadingAvailable(const LoadingDescription& oldLoadingDescription,
                                            const LoadingDescription& newLoadingDescription);

    void signalImageStartedSaving(const QString& filePath);
    void signalSavingProgress(const QString& filePath, float progress);
    void signalImageSaved(const QString& filePath, bool success);

    void signalThumbnailLoaded(const LoadingDescription& loadingDescription, const QImage& img);

public:

    void imageStartedLoading(const LoadingDescription& loadingDescription)                override;
    void loadingProgress(const LoadingDescription& loadingDescription, float progress)    override;
    void imageLoaded(const LoadingDescription& loadingDescription, const DImg& img)       override;
    void moreCompleteLoadingAvailable(const LoadingDescription& oldLoadingDescription,
                                      const LoadingDescription& newLoadingDescription)    override;
    void imageStartedSaving(const QString& filePath)                                      override;
    void savingProgress(const QString& filePath, float progress)                          override;
    void imageSaved(const QString& filePath, bool success)                                override;
    void thumbnailLoaded(const LoadingDescription& loadingDescription, const QImage& img) override;

    virtual bool querySendNotifyEvent() const;
    virtual void taskHasFinished();

protected:

    void run()                                                                            override;

    void notificationReceived();

protected:

    QMutex               m_mutex;

    QList<LoadSaveTask*> m_todo;

    LoadSaveTask*        m_currentTask;

    NotificationPolicy   m_notificationPolicy;

private:

    // Disable
    LoadSaveThread(const LoadSaveThread&)            = delete;
    LoadSaveThread& operator=(const LoadSaveThread&) = delete;

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_LOAD_SAVE_THREAD_H
