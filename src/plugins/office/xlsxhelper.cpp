#include "xlsxhelper.h"
#include "typedeclare.h"

#include <QScriptEngine>
#include <QDateTime>
#include <QDebug>

/// QXlsx::Format 类导出至JavaScript环境
static QScriptValue constructXlsxFormat(QScriptContext* context, QScriptEngine* engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use the 'new' operator."));

    const int argCount = context->argumentCount();
    if (argCount > 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide zero or one arguments."));

    if (argCount == 0) {
        return engine->toScriptValue(QXlsx::Format());
    } else {
        QXlsx::Format format = engine->fromScriptValue<QXlsx::Format>(context->argument(0));
        return engine->toScriptValue(QXlsx::Format(format));
    }
}

static QScriptValue xslxFormat_fontSize(QScriptContext *context, QScriptEngine *)
{
    QXlsx::Format format = qscriptvalue_cast<QXlsx::Format>(context->thisObject());
    return format.fontSize();
}

static QScriptValue xslxFormat_setFontSize(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Format* format = qscriptvalue_cast<QXlsx::Format*>(context->thisObject());
    if (!format)
        return context->throwError(QScriptContext::TypeError, QObject::tr("this object is not an Xlsx.Format."));
    if (context->argumentCount() != 1 || !context->argument(0).isNumber())
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("fontSize is a number."));
    format->setFontSize(context->argument(0).toNumber());
    return eng->undefinedValue();
}

void XlsxHelper::registerFormat(QScriptEngine *eng, QScriptValue& parentProperty)
{
    QScriptValue proto = eng->newObject();
    proto.setProperty(QStringLiteral("fontSize"), eng->newFunction(xslxFormat_fontSize), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("fontSize"), eng->newFunction(xslxFormat_setFontSize), QScriptValue::PropertySetter);

    eng->setDefaultPrototype(qMetaTypeId<QXlsx::Format>(), proto);
    QScriptValue ctor = eng->newFunction(constructXlsxFormat, proto);
    parentProperty.setProperty(QStringLiteral("Format"), ctor);
    QScriptValue format = parentProperty.property(QStringLiteral("Format"));

    // FontScript
    format.setProperty(QStringLiteral("FontScriptNormal"), QXlsx::Format::FontScriptNormal);
    format.setProperty(QStringLiteral("FontScriptSuper"), QXlsx::Format::FontScriptSuper);
    format.setProperty(QStringLiteral("FontScriptSub"), QXlsx::Format::FontScriptSub);
}


/// QXlsx::Cell 类导出至JavaScript环境
static QScriptValue xslxCell_cellType(QScriptContext *context, QScriptEngine *)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell)
        return cell->cellType();
    else
        return context->throwError(QStringLiteral("It is not a valid cell."));
}

static QScriptValue xslxCell_dateTime(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell)
        return eng->newDate(cell->dateTime());
    else
        return context->throwError(QStringLiteral("It is not a valid cell."));
}

static QScriptValue xslxCell_format(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell)
        return eng->toScriptValue(cell->format());
    else
        return context->throwError(QStringLiteral("It is not a valid cell."));
}

//static QScriptValue xslxCell_formula(QScriptContext *context, QScriptEngine *eng)
//{
//    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
//    if (cell)
//        return eng->toScriptValue(cell->format());
//    else
//        return context->throwError(QStringLiteral("It is not a valid cell."));
//}

static QScriptValue xslxCell_hasFormula(QScriptContext *context, QScriptEngine *)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell)
        return cell->hasFormula();
    else
        return context->throwError(QStringLiteral("It is not a valid cell."));
}

static QScriptValue xslxCell_isDateTime(QScriptContext *context, QScriptEngine *)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell)
        return cell->isDateTime();
    else
        return context->throwError(QStringLiteral("It is not a valid cell."));
}

static QScriptValue xslxCell_isRichString(QScriptContext *context, QScriptEngine *)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell)
        return cell->isRichString();
    else
        return context->throwError(QStringLiteral("It is not a valid cell."));
}

static QScriptValue xslxCell_value(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Cell* cell = qscriptvalue_cast<QXlsx::Cell*>(context->thisObject());
    if (cell) {
        if (cell->isDateTime()) {
            QDateTime dt = cell->value().toDateTime();
            qDebug() << "datetime: " << dt.toString() << "  " << cell->value();
            return eng->newDate(dt);
        } else
            return eng->toScriptValue(cell->value());
    } else {
        return context->throwError(QStringLiteral("It is not a valid cell."));
    }
}

void XlsxHelper::registerCell(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    proto.setProperty(QStringLiteral("cellType"), eng->newFunction(xslxCell_cellType), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("dateTime"), eng->newFunction(xslxCell_dateTime), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("format"), eng->newFunction(xslxCell_format), QScriptValue::PropertyGetter);
//    proto.setProperty(QStringLiteral("formula"), eng->newFunction(xslxCell_formula), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("hasFormula"), eng->newFunction(xslxCell_hasFormula), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("isDateTime"), eng->newFunction(xslxCell_isDateTime), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("isRichString"), eng->newFunction(xslxCell_isRichString), QScriptValue::PropertyGetter);
    proto.setProperty(QStringLiteral("value"), eng->newFunction(xslxCell_value), QScriptValue::PropertyGetter);

    eng->setDefaultPrototype(qMetaTypeId<QXlsx::Cell*>(), proto);
    parentProperty.setProperty(QStringLiteral("Cell"), proto);
    QScriptValue cell = parentProperty.property(QStringLiteral("Cell"));

    // CellType
    cell.setProperty(QStringLiteral("BooleanType"), QXlsx::Cell::BooleanType);
    cell.setProperty(QStringLiteral("NumberType"), QXlsx::Cell::NumberType);
    cell.setProperty(QStringLiteral("ErrorType"), QXlsx::Cell::ErrorType);
    cell.setProperty(QStringLiteral("SharedStringType"), QXlsx::Cell::SharedStringType);
    cell.setProperty(QStringLiteral("StringType"), QXlsx::Cell::StringType);
    cell.setProperty(QStringLiteral("InlineStringType"), QXlsx::Cell::InlineStringType);
}



/// QXlsx::Worksheet 类导出至JavaScript环境
static QScriptValue xslxWorksheet_cellAt(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    QXlsx::Cell* cell = nullptr;
    if (context->argumentCount() == 2 && context->argument(0).isNumber()
            && context->argument(1).isNumber()) {
        cell = ws->cellAt(context->argument(0).toInt32(), context->argument(1).toInt32());
    } else if (context->argumentCount() == 1 && context->argument(0).isString()) {
        cell = ws->cellAt(context->argument(0).toString());
    } else {
        return context->throwError(QStringLiteral("Function call syntax error."));
    }
    return cell ? eng->toScriptValue(cell) : eng->nullValue();
}

static QScriptValue xslxWorksheet_read(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    if (context->argumentCount() == 2 && context->argument(0).isNumber()
            && context->argument(1).isNumber()) {
        return eng->toScriptValue(ws->read(context->argument(0).toInt32(),
                                           context->argument(1).toInt32()));
    } else if (context->argumentCount() == 1 && context->argument(0).isString()) {
        return eng->toScriptValue(ws->read(context->argument(0).toString()));
    } else {
        return context->throwError(QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_write(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 3)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 4)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(3));
        return ws->write(row, col, context->argument(2).toVariant(), format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->write(context->argument(0).toString(), context->argument(1).toVariant(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_writeBlank(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 1)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 2)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->writeBlank(row, col, format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 2)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(1));
        return ws->writeBlank(context->argument(0).toString(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_writeBool(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 3)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 4)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(3));
        return ws->writeBool(row, col, context->argument(2).toBool(), format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->writeBool(context->argument(0).toString(), context->argument(1).toBool(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_writeDateTime(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 3)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 4)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(3));
        return ws->writeDateTime(row, col, context->argument(2).toDateTime(), format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->writeDateTime(context->argument(0).toString(), context->argument(1).toDateTime(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_writeInlineString(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 3)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 4)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(3));
        return ws->writeInlineString(row, col, context->argument(2).toString(), format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->writeInlineString(context->argument(0).toString(), context->argument(1).toString(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_writeNumeric(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 3)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 4)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(3));
        return ws->writeNumeric(row, col, context->argument(2).toNumber(), format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->writeNumeric(context->argument(0).toString(), context->argument(1).toNumber(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

static QScriptValue xslxWorksheet_writeString(QScriptContext *context, QScriptEngine */*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2)
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));

    if (context->argument(0).isNumber()) {
        if (n < 3)
            return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
        int row = context->argument(0).toInt32();
        int col = context->argument(1).toInt32();
        QXlsx::Format format;
        if (n == 4)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(3));
        return ws->writeString(row, col, context->argument(2).toString(), format);
    } else if (context->argument(0).isString()) {
        QXlsx::Format format;
        if (n == 3)
            format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
        return ws->writeString(context->argument(0).toString(), context->argument(1).toString(), format);
    } else {
        return context->throwError(QScriptContext::SyntaxError, QStringLiteral("Function call syntax error."));
    }
}

void XlsxHelper::registerWorksheet(QScriptEngine *eng, QScriptValue &/*parentProperty*/)
{
    QScriptValue proto = eng->newObject();
    proto.setProperty(QStringLiteral("cellAt"), eng->newFunction(xslxWorksheet_cellAt));
    proto.setProperty(QStringLiteral("read"), eng->newFunction(xslxWorksheet_read));
    proto.setProperty(QStringLiteral("write"), eng->newFunction(xslxWorksheet_write));
    proto.setProperty(QStringLiteral("writeBlank"), eng->newFunction(xslxWorksheet_writeBlank));
    proto.setProperty(QStringLiteral("writeBool"), eng->newFunction(xslxWorksheet_writeBool));
    proto.setProperty(QStringLiteral("writeDateTime"), eng->newFunction(xslxWorksheet_writeDateTime));
    proto.setProperty(QStringLiteral("writeInlineString"), eng->newFunction(xslxWorksheet_writeInlineString));
    proto.setProperty(QStringLiteral("writeNumeric"), eng->newFunction(xslxWorksheet_writeNumeric));
    proto.setProperty(QStringLiteral("writeString"), eng->newFunction(xslxWorksheet_writeString));

    eng->setDefaultPrototype(qMetaTypeId<QXlsx::Worksheet*>(), proto);
}

