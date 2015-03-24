/*! \file
*  \brief Picture基本界面
*  \author 杨永盛 谭立方
*  \date 2013
*  \version 1.0
*  \copyright 2013 PERAGlobal Ltd. All rights reserved.
*
*  供应用程序中所有其它模块使用的基本通用界面拾色器按钮类
*/
/****************************************************************************
**
** Copyright (c) 2013 PERAGlobal Ltd. All rights reserved.
** All rights reserved.
**
****************************************************************************/

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include "qtcolorpicker.h"
#include "comguiutils_global.h"

namespace GuiUtils {

class COMGUIUTILS_EXPORT ColorPushButton : public QtColorPicker
{
    Q_OBJECT

public:
    ColorPushButton(QWidget *parent = 0);
    ~ColorPushButton();
    void setColor(const QColor& c){ setCurrentColor(c); }
    QColor color(){ return currentColor(); }

signals:
    void colorChanged();
};

}
#endif
