#include <flowlayout.h>
#include <QDialog>
#include <QThreadPool>

#include <digikam/previewloadthread.h>

using namespace Digikam;

class PicDialog : public QDialog {
    Q_OBJECT
public:
    Q_PROPERTY(int spacing_ READ spacing WRITE setSpacing);
    Q_PROPERTY(qreal referenceWidth_ READ referenceWidth WRITE setReferenceWidth);

    PicDialog(QWidget* parent = nullptr);
    ~PicDialog();

    void  setReferenceWidth(qreal width);
    qreal referenceWidth();
    void  setSpacing(int spacing);
    int   spacing();

    bool eventFilter(QObject* watched, QEvent* event) override;

    // load picture
    // When loadbyPool == false, this plugin will get all benefit from from digikam
    // When loadbyPool == true, this plugin load image by QThreadPool, this make it faster in some
    // machine
    void load(QUrl const& url, bool loadbyPool = false);
    void setStyle(Z::FlowLayout::Style);

public slots:
    // add picture to layout
    void add(const QPixmap&);
    void add(const LoadingDescription&, const DImg& dimg);

signals:
    void signalOnClose();
    void signalPixLoaded(QPixmap const&);

protected:
    LoadingDescription createLoadingDescription(const QString& filePath);

private:
    int                spacing_;
    qreal              referenceWidth_;
    QWidget*           box_;
    Z::FlowLayout*     layout_;
    PreviewLoadThread* t;
    QThreadPool*       pool_;
};
