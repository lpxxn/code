#ifndef COMACTIONMANAGER_GLOBAL_H
#define COMACTIONMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMACTIONMANAGER_LIBRARY)
#  define COMACTIONMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define COMACTIONMANAGER_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \defgroup Actionmanager 动作管理模块
 *
 * 动作管理模块中的类为应用程序提供动作, 菜单栏, 菜单, 以及快捷键的统一管理.
 */

#endif // COMACTIONMANAGER_GLOBAL_H
