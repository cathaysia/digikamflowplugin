/*
 * Copyright (c) 2021 <copyright holder> <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef Z_FLOWLAYOUT_H
#define Z_FLOWLAYOUT_H
#include <QLayout>
#include <thread>
class QWidget;
class QTimer;

namespace Z {

/**
 * @todo write docs
 */
class FlowLayout : public QLayout {
public:
    FlowLayout(QWidget* parent = nullptr);
    ~FlowLayout();
    int          count() const override;
    void         addItem(QLayoutItem* item) override;
    void         setGeometry(const QRect& r) override;
    qreal        innerHeight();
    QSize        sizeHint() const override;
    QLayoutItem* itemAt(int index) const override;
    QLayoutItem* takeAt(int index) override;
    const QList<QLayoutItem*> list() const;

    /**
     * @brief 设置布局内窗口的宽度。布局时根据 sizeHint() 对部件进行缩放。
     *
     * @param size 若 size = -1，则根据部件的 sizeHint() 设置宽度，否则设置为 size
     */
    void          setWidgetWidth(size_t size);
    inline size_t widgetWidth();

protected:
    /**
     * @brief 当设置了 WidgetWidth 后调用此函数，此函数是流式布局的特化
     *
     */
    void doMonoLayout();

private:
    QList<QLayoutItem*> list_;
    size_t              width_ = -1;
    std::atomic<qreal>  inner_height_;
    QTimer* timer_;
    //     qreal inner_height_ = 0;
};

}    // namespace Z

#endif    // Z_FLOWLAYOUT_H
