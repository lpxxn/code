#ifndef DOCXHELPER_H
#define DOCXHELPER_H

#include <QScriptValue>

class QScriptEngine;
class DocxHelper
{
public:
    static void registerParagraph(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerRun(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerTable(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerCell(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerListCell(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerInlineShape(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerListTables(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerListParagraphs(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerSection(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerHeader(QScriptEngine* eng, QScriptValue &parentProperty);
    static void registerFooter(QScriptEngine* eng, QScriptValue &parentProperty);
};

#endif // DOCXHELPER_H
