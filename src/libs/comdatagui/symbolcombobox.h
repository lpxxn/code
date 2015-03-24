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

#ifndef SYMBOLCOMBOBOX_H
#define SYMBOLCOMBOBOX_H

#include "comdatagui_global.h"

#include <qwt/qwt_symbol.h>

#include <QComboBox>

namespace ComDataGui
{

/*!
 * \class SymbolComboBox
 * \brief 用于选择Qwt内建Symbol的复合框.
 */
class COMDATAGUI_EXPORT SymbolComboBox : public QComboBox
{
    Q_OBJECT
public:
    SymbolComboBox(bool showNoSymbol = true, QWidget *parent = 0);

    void setStyle(const QwtSymbol::Style& c);
    QwtSymbol::Style selectedSymbol() const;

    static QwtSymbol::Style style(int index);
    static int symbolIndex(const QwtSymbol::Style& style);
    static QList<int> defaultSymbols();

signals:
    void activated(SymbolComboBox *);

protected:
    void init(bool showNoSymbol);
    void focusInEvent(QFocusEvent *);

private:
    static const QwtSymbol::Style symbols[];
};

}
#endif // SYMBOLCOMBOBOX_H
