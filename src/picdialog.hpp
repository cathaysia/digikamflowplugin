#include <flowlayout.h>
#include <QDialog>
#include <QThreadPool>

#include <digikam/previewloadthread.h>

using namespace Digikam;

class PicDialog : public QDialog {
    Q_OBJECT
public:
    PicDialog(QWidget* parent = nullptr);
    ~PicDialog();

    void  setReferenceWidth(qreal width);
    qreal referenceWidth();
    void  setSpacing(int spacing);
    int   spacing();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void load(QUrl const& url, bool loadbyPool = false);
    void setStyle(Z::FlowLayout::Style);

public slots:
    // add picture to layout
    void add(LoadingDescription const& desc, DImg const& img);
    void add(const QPixmap&);

signals:
    void signalOnClose();
    void signalPixLoaded(QPixmap const&);

private:
    QWidget*           box_;
    Z::FlowLayout*     layout_;
    QThreadPool*       pool_;
    PreviewLoadThread* t_;
};
