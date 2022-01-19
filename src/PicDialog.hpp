#include <flowlayout.h>
#include <QDialog>

#include <digikam/managedloadsavethread.h>

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

signals:
    void onClose();

private:
    int            spacing_        = -1;
    qreal          referenceWidth_ = 300;
    QWidget*       box_            = nullptr;
    Z::FlowLayout* layout_         = nullptr;
};
