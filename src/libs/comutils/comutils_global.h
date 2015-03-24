#ifndef UTILS_GLOBAL_H
#define UTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTILS_LIBRARY)
#  define UTILS_EXPORT Q_DECL_EXPORT
#else
#  define UTILS_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \defgroup Utils 基础模块
 *
 *在基础模块中定义的类, 函数可以在多个应用程序开发中使用, 这里定义最基本的功能, 不包含
 *应用程序的任何业务逻辑. 应用软件的License授权功能就定义在这个模块中.
 */

#endif // UTILS_GLOBAL_H
