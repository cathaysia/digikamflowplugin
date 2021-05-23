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

#include "flowlayout.h"
#include <QDebug>
#include <QRect>
#include <QWidget>
#include <QTimer>

#include <functional>
#include <iostream>

namespace Z {
FlowLayout::FlowLayout(QWidget* parent) : QLayout(parent), inner_height_(0), timer_{new QTimer} {
    // 延迟刷新
    timer_->setInterval(300);
    connect(timer_, &QTimer::timeout, this, &FlowLayout::update);
}
FlowLayout::~FlowLayout() {
    QLayoutItem* item = nullptr;
    while((item = takeAt(0))) delete item;
}

/**
 * @brief 添加时不进行检查是否有重复项
 *
 * @param item p_item:...
 */
void FlowLayout::addItem(QLayoutItem* item) {
    list_.append(item);
    // 这里不 update 的话，没有 resize 之前，item 是叠在一起的
    timer_->start();
}
int FlowLayout::count() const {
    return list_.length();
}
QLayoutItem* FlowLayout::itemAt(int index) const {
    return index >= 0 && index < list_.length() ? list_.at(index) : nullptr;
}
QSize FlowLayout::sizeHint() const {
    return QSize(geometry().width(), inner_height_);
}

QLayoutItem* FlowLayout::takeAt(int index) {
    return index >= 0 && index < list_.length() ? list_.takeAt(index) : nullptr;
}
void FlowLayout::setGeometry(const QRect& r) {
    QLayout::setGeometry(r);
    if(width_ != -1) doMonoLayout();
    else
        assert(0);
    // 对子项进行布局
}
void FlowLayout::setWidgetWidth(size_t size) {
    this->width_ = size;
}
size_t FlowLayout::widgetWidth() {
    return width_;
}

qreal FlowLayout::innerHeight() {
    return inner_height_;
}

const QList<QLayoutItem*> FlowLayout::list() const {
    return list_;
}


void FlowLayout::doMonoLayout() {
    assert(width_ != -1);
    if(list_.length() <=0) return;
    // 一行内有 n 个 widget 和 n-1 个 spacing
    // n*width_ + (n-1)*spacing = this->width()
    // n*(width_+spacing) - spacing = this.width()
    int n = (this->geometry().width() + spacing()) / (width_ + spacing());
    // 防止 n 比 list_.length() 大
    n = std::min(n, list_.length());
    // 防止 n 为零
    /**
     * 2021-05-22
     * 这里如果 n 为零的话，可能会导致核心转储，因此一定要注意！
     * 上一行代码和这一行代码不能互换位置，否则当 list_.length == 0 时会除零
     */
    n = n == 0 ? 1 : n;

    qreal realWidth = width_ + (this->geometry().width() - n * width_) / n;
    auto  yFlags    = std::make_shared<std::vector<int>>();
    yFlags->resize(n);

    int   xFlag      = 0;
    qreal realHeight = 0;
    int min = 0;
    for(size_t i = 0; i < list_.length(); ++i) {
        // realHieght = realWidth*(y/x)
        // ry/rx=y/x ==> ry=rx*y/x
        realHeight = realWidth * list_[i]->sizeHint().rheight() / list_[i]->sizeHint().rwidth();
        list_[i]->setGeometry(QRect(xFlag, (*yFlags)[min], realWidth, realHeight));
        (*yFlags)[min] += (realHeight + spacing());
        xFlag = xFlag + realWidth + spacing() > this->geometry().width() ? 0 : xFlag + realWidth + spacing();
        //
        for(int i =0; i<yFlags->size(); ++i){
            if(yFlags->at(i)<yFlags->at(min)) min = i;
        }
    }
    // 设置父组件的尺寸
    int maxHeight = 0;
    std::for_each(yFlags->begin(), yFlags->end(), [&maxHeight](int val) {
        if(val > maxHeight) maxHeight = val;
    });
    this->parentWidget()->resize(this->geometry().width(), maxHeight);
    inner_height_ = maxHeight;
}

}    // namespace Z
