#pragma once

#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class AspectRatioPixmapLabel : public QLabel {
    Q_OBJECT;
public:
    explicit AspectRatioPixmapLabel(QWidget *parent = 0);
    QSize   sizeHint() const override;
    QPixmap scaledPixmap() const;
    void    setPixmap(const QPixmap &pix);
    // overwrite these function
    void setScaledContents(bool);
    bool hasScaledContents() const;

    void adjust();

private:
    QImage pix_;
};
