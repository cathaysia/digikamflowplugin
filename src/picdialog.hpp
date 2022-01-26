#include <flowlayout.h>
#include <QDialog>

#include <digikam/previewloadthread.h>

using namespace Digikam;

class PicDialog : public QDialog {
    Q_OBJECT
public:
    PicDialog(QWidget* parent = nullptr);
    ~PicDialog();

    void loadPic(const LoadingDescription&, const DImg& dimg);
    void setWidgetWidth(qreal width);
    void setSpacing(int spacing);

    bool eventFilter(QObject* watched, QEvent* event) override;

    LoadingDescription createLoadingDescription(const QString& filePath);

    void load(QUrl const& url);

signals:
    void onClose();

private:
    int                spacing_;
    qreal              referenceWidth_;
    QWidget*           box_;
    Z::FlowLayout*     layout_;
    PreviewLoadThread* t;
};
