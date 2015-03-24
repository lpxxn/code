#ifndef GUIUTILS_GLOBAL_H
#define GUIUTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMGUIUTILS_LIBRARY)
#  define COMGUIUTILS_EXPORT Q_DECL_EXPORT
#else
#  define COMGUIUTILS_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \defgroup GuiUtils 图形界面基础模块
 *
 * 在图形界面基础模块中定义各种通用的图形界面类, 可以被其它各个模块重用.
 */

#endif // GUIUTILS_GLOBAL_H
