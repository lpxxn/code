#include "xlsxhelper.h"
#include "typedeclare.h"

#include <QScriptEngine>
#include <QDateTime>
#include <QDebug>
#include <QImage>

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

static QScriptValue xslxFormat_setHorizontalAlignment(QScriptContext *context, QScriptEngine *eng)
{
    QXlsx::Format* format = qscriptvalue_cast<QXlsx::Format*>(context->thisObject());
    if (!format)
        return context->throwError(QScriptContext::TypeError, QObject::tr("this object is not an Xlsx.Format."));
    if (context->argumentCount() != 1 || !context->argument(0).isNumber())
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("fontSize is a number."));

    format->setHorizontalAlignment((QXlsx::Format::HorizontalAlignment)context->argument(0).toInt32());

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

    // HorizontalAlignment
    format.setProperty(QStringLiteral("AlignLeft"), QXlsx::Format::AlignLeft);
    format.setProperty(QStringLiteral("AlignHCenter"), QXlsx::Format::AlignHCenter);
    format.setProperty(QStringLiteral("AlignRight"), QXlsx::Format::AlignRight);
    proto.setProperty(QStringLiteral("setHorizontalAlignment"), eng->newFunction(xslxFormat_setHorizontalAlignment));
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

static QScriptValue constructXlsxImage(QScriptContext* context, QScriptEngine* engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use the 'new' operator."));

    const int argCount = context->argumentCount();
    if (argCount > 1 || argCount == 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    return engine->toScriptValue(QImage(context->argument(0).toString()));

}

static QScriptValue xslxCell_setSize(QScriptContext *context, QScriptEngine *eng)
{
    QImage img = qscriptvalue_cast<QImage>(context->thisObject());
    if (img.isNull())
        return context->throwError(QScriptContext::TypeError, QObject::tr("this object is not an Xlsx.Image."));
    int args = context->argumentCount();

    if (args == 0 || args > 2)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide two arguments."));

    img = img.scaled(context->argument(0).toNumber(), context->argument(1).toNumber(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return eng->toScriptValue(img);
}

void XlsxHelper::registerImage(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    proto.setProperty(QStringLiteral("setSize"), eng->newFunction(xslxCell_setSize));

    eng->setDefaultPrototype(qMetaTypeId<QImage>(), proto);
    QScriptValue ctor = eng->newFunction(constructXlsxImage, proto);
    parentProperty.setProperty(QStringLiteral("Image"), ctor);
    //QScriptValue image = parentProperty.property(QStringLiteral("Image"));

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

static QScriptValue xslxWorksheet_write(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_writeBlank(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_writeBool(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_writeDateTime(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_writeInlineString(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_writeNumeric(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_writeString(QScriptContext *context, QScriptEngine* /*eng*/)
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

static QScriptValue xslxWorksheet_rowCount(QScriptContext *context, QScriptEngine* /*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    QXlsx::CellRange cellRange = ws->dimension();

    return cellRange.lastRow();
}

static QScriptValue xslxWorksheet_columnCount(QScriptContext *context, QScriptEngine* /*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    QXlsx::CellRange cellRange = ws->dimension();

    return cellRange.lastColumn();
}

static QScriptValue xslxWorksheet_insertImage(QScriptContext *context, QScriptEngine* /*eng*/)
{

    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 3 || n == 0)
        return context->throwError(QStringLiteral("please provide three arguments"));

    return ws->insertImage(context->argument(0).toNumber(), context->argument(1).toNumber(), QImage(context->argument(2).toString()));
}

static QScriptValue xslxWorksheet_mergeCells(QScriptContext *context, QScriptEngine* /*eng*/)
{
    //const QString &range, const QXlsx::Format &format= QXlsx::Format()
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 2 || n == 0)
        return context->throwError(QStringLiteral("please provide one or two arguments"));

    QString range = context->argument(0).toString();
    QXlsx::Format format;
    if (n == 2)
        format = qscriptvalue_cast<QXlsx::Format>(context->argument(1));

    return ws->mergeCells(QXlsx::CellRange(range), format);
}

static QScriptValue xslxWorksheet_unmergeCells(QScriptContext *context, QScriptEngine* /*eng*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one or two arguments"));

    QString range = context->argument(0).toString();
    return ws->unmergeCells(QXlsx::CellRange(range));
}

static QScriptValue xslxWorksheet_setColumnWidth(QScriptContext *context, QScriptEngine* /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2 || n > 3)
        return context->throwError(QStringLiteral("please provide two or three arguments"));

    if (n == 2)
        return ws->setColumnWidth(context->argument(1).toNumber(), context->argument(1).toNumber(), context->argument(0).toNumber());

    return ws->setColumnWidth(context->argument(1).toNumber(), context->argument(2).toNumber(), context->argument(0).toNumber());


}

static QScriptValue xslxWorksheet_setColumnFormat(QScriptContext *context, QScriptEngine* /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2 || n > 3)
        return context->throwError(QStringLiteral("please provide two or three arguments"));

    QXlsx::Format format = qscriptvalue_cast<QXlsx::Format>(context->argument(0));
    if (n == 2)
        return ws->setColumnFormat(context->argument(1).toNumber(), context->argument(1).toNumber(), format);

    return ws->setColumnFormat(context->argument(1).toNumber(), context->argument(2).toNumber(), format);
}

static QScriptValue xslxWorksheet_setColumnHidden(QScriptContext *context, QScriptEngine* /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n < 2 || n > 3)
        return context->throwError(QStringLiteral("please provide two or three arguments"));

    if (n == 2)
        return ws->setColumnHidden(context->argument(1).toNumber(), context->argument(1).toNumber(), context->argument(0).toBool());

    return ws->setColumnHidden(context->argument(1).toNumber(), context->argument(2).toNumber(), context->argument(0).toBool());
}

static QScriptValue xslxWorksheet_columnWidth(QScriptContext *context, QScriptEngine* /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one argument"));

    return ws->columnWidth(context->argument(0).toNumber());
}

static QScriptValue xslxWorksheet_columnFormat(QScriptContext *context, QScriptEngine *engine)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one argument"));

    return engine->toScriptValue(ws->columnFormat(context->argument(0).toNumber()));
}

static QScriptValue xslxWorksheet_isColumnHidden(QScriptContext *context, QScriptEngine* /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one argument"));

    return ws->isColumnHidden(context->argument(0).toNumber());
}

static QScriptValue xslxWorksheet_setRowHeight(QScriptContext *context, QScriptEngine *engine)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 3 || n == 0)
        return context->throwError(QStringLiteral("please provide three arguments"));

    return engine->toScriptValue(ws->setRowHeight(context->argument(0).toNumber()
                                                  , context->argument(1).toNumber()
                                                  , context->argument(2).toNumber()));
}

static QScriptValue xslxWorksheet_setRowFormat(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 3 || n == 0)
        return context->throwError(QStringLiteral("please provide three arguments"));

    QXlsx::Format format = qscriptvalue_cast<QXlsx::Format>(context->argument(2));
    return ws->setRowFormat(context->argument(0).toNumber(), context->argument(1).toNumber(), format);
}

static QScriptValue xslxWorksheet_setRowHidden(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 3 || n == 0)
        return context->throwError(QStringLiteral("please provide three arguments"));

    return ws->setRowHidden(context->argument(0).toNumber(), context->argument(1).toNumber(),context->argument(2).toBool());
}

static QScriptValue xslxWorksheet_rowHeight(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one argument"));

    return ws->rowHeight(context->argument(0).toNumber());
}

static QScriptValue xslxWorksheet_rowFormat(QScriptContext *context, QScriptEngine *engine)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one argument"));

    return engine->toScriptValue(ws->rowFormat(context->argument(0).toNumber()));
}

static QScriptValue xslxWorksheet_isRowHidden(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 1 || n == 0)
        return context->throwError(QStringLiteral("please provide one argument"));

    return ws->isRowHidden(context->argument(0).toNumber());
}

static QScriptValue xslxWorksheet_groupRows(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 3 || n == 0)
        return context->throwError(QStringLiteral("please provide two or three argument"));

    if (n == 3)
        return ws->groupRows(context->argument(0).toNumber(), context->argument(1).toNumber(), context->argument(2).toBool());

    return ws->groupRows(context->argument(0).toNumber(), context->argument(1).toNumber());
}

static QScriptValue xslxWorksheet_groupColumns(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n > 3 || n == 0)
        return context->throwError(QStringLiteral("please provide two or three arguments"));

    if (n == 3)
        return ws->groupColumns(context->argument(0).toNumber(), context->argument(1).toNumber(), context->argument(2).toBool());

    return ws->groupColumns(context->argument(0).toNumber(), context->argument(1).toNumber());
}

static QScriptValue xslxWorksheet_isWindowProtected(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isWindowProtected();
}

static QScriptValue xslxWorksheet_setWindowProtected(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setWindowProtected(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isFormulasVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isFormulasVisible();
}

static QScriptValue xslxWorksheet_setFormulasVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setFormulasVisible(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isGridLinesVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isGridLinesVisible();
}

static QScriptValue xslxWorksheet_setGridLinesVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setGridLinesVisible(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isRowColumnHeadersVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isRowColumnHeadersVisible();
}

static QScriptValue xslxWorksheet_setRowColumnHeadersVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setRowColumnHeadersVisible(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isZerosVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isZerosVisible();
}

static QScriptValue xslxWorksheet_setZerosVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setZerosVisible(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isRightToLeft(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isRightToLeft();
}

static QScriptValue xslxWorksheet_setRightToLeft(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setRightToLeft(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isSelected(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isSelected();
}

static QScriptValue xslxWorksheet_setSelected(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setSelected(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isRulerVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isRulerVisible();
}

static QScriptValue xslxWorksheet_setRulerVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setRulerVisible(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isOutlineSymbolsVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isOutlineSymbolsVisible();
}

static QScriptValue xslxWorksheet_setOutlineSymbolsVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setOutlineSymbolsVisible(context->argument(0).toBool());
    return true;
}

static QScriptValue xslxWorksheet_isWhiteSpaceVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n != 0)
        return context->throwError(QStringLiteral("no arguments"));

    return ws->isWhiteSpaceVisible();
}

static QScriptValue xslxWorksheet_setWhiteSpaceVisible(QScriptContext *context, QScriptEngine * /*engine*/)
{
    QXlsx::Worksheet* ws = qscriptvalue_cast<QXlsx::Worksheet*>(context->thisObject());
    if (!ws)
        return context->throwError(QStringLiteral("It is not a valid Worksheet."));

    const int n = context->argumentCount();
    if (n == 0 || n > 1)
        return context->throwError(QStringLiteral("please provide one argument"));

    ws->setWhiteSpaceVisible(context->argument(0).toBool());
    return true;
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

    proto.setProperty(QStringLiteral("rowCount"), eng->newFunction(xslxWorksheet_rowCount));
    proto.setProperty(QStringLiteral("columnCount"), eng->newFunction(xslxWorksheet_columnCount));
    proto.setProperty(QStringLiteral("mergeCells"), eng->newFunction(xslxWorksheet_mergeCells));
    proto.setProperty(QStringLiteral("unmergeCells"), eng->newFunction(xslxWorksheet_unmergeCells));
    proto.setProperty(QStringLiteral("insertImage"), eng->newFunction(xslxWorksheet_insertImage));

    proto.setProperty(QStringLiteral("setColumnWidth"), eng->newFunction(xslxWorksheet_setColumnWidth));
    proto.setProperty(QStringLiteral("setColumnFormat"), eng->newFunction(xslxWorksheet_setColumnFormat));
    proto.setProperty(QStringLiteral("setColumnHidden"), eng->newFunction(xslxWorksheet_setColumnHidden));
    proto.setProperty(QStringLiteral("columnWidth"), eng->newFunction(xslxWorksheet_columnWidth));
    proto.setProperty(QStringLiteral("columnFormat"), eng->newFunction(xslxWorksheet_columnFormat));
    proto.setProperty(QStringLiteral("isColumnHidden"), eng->newFunction(xslxWorksheet_isColumnHidden));

    proto.setProperty(QStringLiteral("setRowHeight"), eng->newFunction(xslxWorksheet_setRowHeight));
    proto.setProperty(QStringLiteral("setRowFormat"), eng->newFunction(xslxWorksheet_setRowFormat));
    proto.setProperty(QStringLiteral("setRowHidden"), eng->newFunction(xslxWorksheet_setRowHidden));
    proto.setProperty(QStringLiteral("rowHeight"), eng->newFunction(xslxWorksheet_rowHeight));
    proto.setProperty(QStringLiteral("rowFormat"), eng->newFunction(xslxWorksheet_rowFormat));
    proto.setProperty(QStringLiteral("isRowHidden"), eng->newFunction(xslxWorksheet_isRowHidden));

    proto.setProperty(QStringLiteral("groupRows"), eng->newFunction(xslxWorksheet_groupRows));
    proto.setProperty(QStringLiteral("groupColumns"), eng->newFunction(xslxWorksheet_groupColumns));

    proto.setProperty(QStringLiteral("isWindowProtected"), eng->newFunction(xslxWorksheet_isWindowProtected));
    proto.setProperty(QStringLiteral("setWindowProtected"), eng->newFunction(xslxWorksheet_setWindowProtected));

    proto.setProperty(QStringLiteral("isFormulasVisible"), eng->newFunction(xslxWorksheet_isFormulasVisible));
    proto.setProperty(QStringLiteral("setFormulasVisible"), eng->newFunction(xslxWorksheet_setFormulasVisible));

    proto.setProperty(QStringLiteral("isGridLinesVisible"), eng->newFunction(xslxWorksheet_isGridLinesVisible));
    proto.setProperty(QStringLiteral("setGridLinesVisible"), eng->newFunction(xslxWorksheet_setGridLinesVisible));

    proto.setProperty(QStringLiteral("isRowColumnHeadersVisible"), eng->newFunction(xslxWorksheet_isRowColumnHeadersVisible));
    proto.setProperty(QStringLiteral("setRowColumnHeadersVisible"), eng->newFunction(xslxWorksheet_setRowColumnHeadersVisible));

    proto.setProperty(QStringLiteral("isZerosVisible"), eng->newFunction(xslxWorksheet_isZerosVisible));
    proto.setProperty(QStringLiteral("setZerosVisible"), eng->newFunction(xslxWorksheet_setZerosVisible));

    proto.setProperty(QStringLiteral("isRightToLeft"), eng->newFunction(xslxWorksheet_isRightToLeft));
    proto.setProperty(QStringLiteral("setRightToLeft"), eng->newFunction(xslxWorksheet_setRightToLeft));

    proto.setProperty(QStringLiteral("isSelected"), eng->newFunction(xslxWorksheet_isSelected));
    proto.setProperty(QStringLiteral("setSelected"), eng->newFunction(xslxWorksheet_setSelected));

    proto.setProperty(QStringLiteral("isRulerVisible"), eng->newFunction(xslxWorksheet_isRulerVisible));
    proto.setProperty(QStringLiteral("setRulerVisible"), eng->newFunction(xslxWorksheet_setRulerVisible));

    proto.setProperty(QStringLiteral("isOutlineSymbolsVisible"), eng->newFunction(xslxWorksheet_isOutlineSymbolsVisible));
    proto.setProperty(QStringLiteral("setOutlineSymbolsVisible"), eng->newFunction(xslxWorksheet_setOutlineSymbolsVisible));

    proto.setProperty(QStringLiteral("isWhiteSpaceVisible"), eng->newFunction(xslxWorksheet_isWhiteSpaceVisible));
    proto.setProperty(QStringLiteral("setWhiteSpaceVisible"), eng->newFunction(xslxWorksheet_setWhiteSpaceVisible));
    //


    eng->setDefaultPrototype(qMetaTypeId<QXlsx::Worksheet*>(), proto);
}

