#ifndef COMDATAGUI_GLOBAL_H
#define COMDATAGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMDATAGUI_LIBRARY)
#  define COMDATAGUI_EXPORT Q_DECL_EXPORT
#else
#  define COMDATAGUI_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \defgroup ComData 基础数据模块
 *
 *
 */

#endif // COMDATAGUI_GLOBAL_H
