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

#include "flowlayout.h"
#include <QDialog>
#include <QThreadPool>

#include "previewloadthread.h"

using namespace Digikam;

class PicDialog : public QDialog {
    Q_OBJECT;

public:
    PicDialog(QWidget* parent = nullptr);
    ~PicDialog();

    void  setReferenceWidth(qreal width);
    qreal referenceWidth();
    void  setSpacing(int spacing);
    int   spacing();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void load(QUrl const& url, bool loadbyPool = false);
    void setStyle(Z::Style);

public slots:
    // add picture to layout
    void add(LoadingDescription const& desc, DImg const& img);
    void add(const QPixmap&);

signals:
    void signalPixLoaded(QPixmap const&);

private:
    QAtomicInt         stop_;
    QWidget*           box_;
    Z::FlowLayout*     layout_;
    QThreadPool*       pool_;
    PreviewLoadThread* t_;
};
