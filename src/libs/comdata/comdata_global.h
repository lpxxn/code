#ifndef COMDATA_GLOBAL_H
#define COMDATA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMDATA_LIBRARY)
#  define COMDATA_EXPORT Q_DECL_EXPORT
#else
#  define COMDATA_EXPORT Q_DECL_IMPORT
#endif

/*!
 * \defgroup ComData 基础数据模块
 *
 *
 */

#endif // COMDATA_GLOBAL_H
