#ifndef XLSXHELPER_H
#define XLSXHELPER_H

#include <QScriptValue>

class QScriptEngine;

class XlsxHelper
{
public:
    static void registerFormat(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerCell(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerImage(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerWorksheet(QScriptEngine* eng, QScriptValue &);
};


#endif // XLSXHELPER_H
