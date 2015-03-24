/*! \file
*  \brief Picture基本界面
*  \author 杨永盛
*  \date 2013
*  \version 1.0
*  \copyright 2013 PERAGlobal Ltd. All rights reserved.
*
*  供应用程序中所有其它模块使用的基本通用界面符号ComboBox
*/
/****************************************************************************
**
** Copyright (c) 2013 PERAGlobal Ltd. All rights reserved.
** All rights reserved.
**
****************************************************************************/

#include "symbolcombobox.h"
#include <algorithm>

#include <qpixmap>
#include <qpainter>

namespace ComDataGui
{

const QwtSymbol::Style SymbolComboBox::symbols[] = {
    QwtSymbol::NoSymbol,
    QwtSymbol::Ellipse,
    QwtSymbol::Rect,
    QwtSymbol::Diamond,
    QwtSymbol::Triangle,
    QwtSymbol::DTriangle,
    QwtSymbol::UTriangle,
    QwtSymbol::LTriangle,
    QwtSymbol::RTriangle,
    QwtSymbol::Cross,
    QwtSymbol::XCross,
    QwtSymbol::HLine,
    QwtSymbol::VLine,
    QwtSymbol::Star1,
    QwtSymbol::Star2,
    QwtSymbol::Hexagon
};

/*!
 * \brief 构造符号ComboBox.
 * \param showNoSymbol, 是否有”无符号“，如果为真，则添加”无符号“到符号列表串；否则不添加.
 * \param parent，父窗口.
 */
SymbolComboBox::SymbolComboBox(bool showNoSymbol, QWidget *parent) : QComboBox(parent)
{
    init(showNoSymbol);
}

/*!
 * \brief 初始化符号ComboBox内容
 * \param showNoSymbol, 是否有”无符号“，如果为真，则添加”无符号“到符号列表串；否则不添加.
 */
void SymbolComboBox::init(bool showNoSymbol)
{
    QPixmap icon = QPixmap(15, 15);
    QColor c = QColor (Qt::gray);
    icon.fill(c);
    const QRectF e = QRectF(1, 1, 14, 14);
    const QPointF r = QPointF(7, 7);
    const QSize s = QSize(10, 10);
    QPainter p(&icon);
    p.setRenderHint(QPainter::Antialiasing);
    QwtSymbol symb;
    p.setBrush(QBrush(QColor(Qt::white)));

    if (showNoSymbol)
        this->addItem(tr("No Symbol"), QwtSymbol::NoSymbol);

    symb.setStyle (QwtSymbol::Ellipse);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon, tr("Ellipse"), QwtSymbol::Ellipse);

    symb.setStyle (QwtSymbol::Rect);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Rectangle"), QwtSymbol::Rect);

    symb.setStyle (QwtSymbol::Diamond);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Diamond"), QwtSymbol::Diamond);

    symb.setStyle (QwtSymbol::Triangle);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Triangle"), QwtSymbol::Triangle);

    symb.setStyle (QwtSymbol::DTriangle);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Down Triangle"), QwtSymbol::DTriangle);

    symb.setStyle (QwtSymbol::UTriangle);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Up Triangle"), QwtSymbol::UTriangle);

    symb.setStyle (QwtSymbol::LTriangle);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Left Triangle"), QwtSymbol::LTriangle);

    symb.setStyle (QwtSymbol::RTriangle);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Right Triangle"), QwtSymbol::RTriangle);

    symb.setStyle (QwtSymbol::Cross);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Cross"), QwtSymbol::Cross);

    symb.setStyle (QwtSymbol::XCross);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Diagonal Cross"), QwtSymbol::XCross);

    symb.setStyle (QwtSymbol::HLine);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Horizontal Line"), QwtSymbol::HLine);

    symb.setStyle (QwtSymbol::VLine);
    p.eraseRect(e);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Vertical Line"), QwtSymbol::VLine);

    symb.setStyle (QwtSymbol::Star1);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Star 1"), QwtSymbol::Star1);

    symb.setStyle (QwtSymbol::Star2);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Star 2"), QwtSymbol::Star2);

    symb.setStyle (QwtSymbol::Hexagon);
    icon.fill(c);
    symb.setSize(s);
    symb.drawSymbol(&p, r);
    this->addItem(icon,tr("Hexagon"), QwtSymbol::Hexagon);

    p.end();
}

/*!
 * \brief 设置符号样式.
 * \param style 符号样式值.
 */
void SymbolComboBox::setStyle(const QwtSymbol::Style& style)
{
    const QwtSymbol::Style*ite = std::find(symbols, symbols + sizeof(symbols), style);
    if (ite == symbols + sizeof(symbols))
        this->setCurrentIndex(0);
    else
        this->setCurrentIndex(ite - symbols);
}

/*!
 * \brief 获取选中的符号样式.
 * \return 选中的符号样式.
 */
QwtSymbol::Style SymbolComboBox::selectedSymbol() const
{
    size_t i = this->currentIndex();
    if (i < sizeof(symbols))
        return symbols[this->currentIndex()];

    return QwtSymbol::NoSymbol;
}

/*!
 * \brief 获取符号样式的索引值.
 * \param style，符号样式.
 * \return 符号样式的索引值.
 */
int SymbolComboBox::symbolIndex(const QwtSymbol::Style& style)
{
    const QwtSymbol::Style*ite = std::find(symbols, symbols + sizeof(symbols), style);
    if (ite == symbols + sizeof(symbols))
        return 0;

    return (ite - symbols);
}

/*!
 * \brief 获取符号样式的索引值.
 * \param index 符号样式的索引值.
 * \return 符号样式.
 */
QwtSymbol::Style SymbolComboBox::style(int index)
{
    if (index >= 0 && index < (int)sizeof(symbols))
        return symbols[index];

    return QwtSymbol::NoSymbol;
}

/*!
 * \brief 获取默认的符号样式.
 * \return 符号样式.
 */
QList<int> SymbolComboBox::defaultSymbols()
{
    QList<int> lst;
    for (int i = 0; i < QwtSymbol::Hexagon+1; i++)
        lst << i;

    return lst;
}

/*!
 * \brief focusInEvent事件处理, 发送信号activated().
 * \param e，事件.
 */
void SymbolComboBox::focusInEvent(QFocusEvent * e)
{
    emit activated(this);
    return QComboBox::focusInEvent(e);
}

}
