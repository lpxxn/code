#include "uiscript.h"
#include "uiloader.h"

#include <comguiutils/commonguiutils.h>

#include <comdatagui/graph.h>
#include <comdata/table.h>

#include <QWidget>
#include <QLineEdit>
#include <QInputDialog>
#include <QFileDialog>

#include <QScriptEngine>
#include <QStringList>
#include <QDebug>

#include <map>

Q_SCRIPT_DECLARE_QMETAOBJECT(UiLoader, QObject*)

static UIScript* stUIScript = nullptr;

/*!
 * \brief 用于在非主线程中利用JavaScript代码创建图形界面, 并与之交互.
 * \param ctx
 * \param engine
 * \return 如果脚本代码的最后一行求值结果是QWidget类型, 那么可以返回该对象.
 */
QScriptValue uiInteractive(QScriptContext* ctx, QScriptEngine* engine)
{
    if (ctx->argumentCount() != 1 || !(ctx->argument(0).isString() || ctx->argument(0).isFunction())) {
        return ctx->throwError(QScriptContext::TypeError, "'uiInteractive' method can only have a string argument or function.");
    }

    QString code = ctx->argument(0).toString();
    if (ctx->argument(0).isFunction()) {
        int begin = code.indexOf('{');
        int end = code.lastIndexOf('}');
        code = code.mid(begin + 1, end - begin - 1);
    }

    QWidget* w = nullptr;
    QMetaObject::invokeMethod(stUIScript, "uiInteractiveDelegate",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QWidget*, w),
                              Q_ARG(QScriptEngine*, engine),
                              Q_ARG(QString, code));
    return w ? engine->toScriptValue(w) : engine->nullValue();
}

/*!
 * \brief 生成数据表的图形截图.
 * \param table 数据表
 * \param columns 列索引
 * \param output 输出文件名
 * \param type 图形类型
 * \param size 图形尺寸
 * \param format 输出文件类型, 支出pdf, svg, png等格式
 * \return 是否成功输出文件
 */
//bool Graph::snapshot(ComData::Table* table, const QList<int>& columns,
//                     const QString& output, const QString& title, GraphType type,
//                     const QSize& size)

QScriptValue ui_saveGraph(QScriptContext* ctx, QScriptEngine* engine)
{
    const int count = ctx->argumentCount();
    if (count != 2 && count != 3)
        return ctx->throwError(QScriptContext::SyntaxError,
                               QStringLiteral("Function arguments is (array, path, [options])"));
    if (!ctx->argument(0).isArray())
        return ctx->throwError(QScriptContext::SyntaxError, QStringLiteral("The first argument is a data array."));
    if (!ctx->argument(1).isString())
        return ctx->throwError(QScriptContext::SyntaxError, QStringLiteral("The second argument is a file path string."));
    if (count == 3 && !ctx->argument(2).isObject())
        return ctx->throwError(QScriptContext::SyntaxError, QStringLiteral("The third argument is a object."));

    ComData::Table* table = nullptr;
    QVariant temp = ctx->argument(0).toVariant();
    QVariantList array = temp.toList();
    if (array.isEmpty()) return engine->undefinedValue();

    // options
    QString title;
    ComDataGui::GraphType gt = ComDataGui::GT_Line;
    QSize sz(120, 80);
    QList<int> significances;

    if (count == 3) {
        QVariant temp = ctx->argument(2).toVariant();
        if (temp.type() == QVariant::Map) {
            QVariantMap options = temp.toMap();
            if (options.contains(QStringLiteral("title")))
                title = options[QStringLiteral("title")].toString();
            if (options.contains(QStringLiteral("type")))
                gt = ComDataGui::GraphType(options[QStringLiteral("type")].toInt());
            if (options.contains(QStringLiteral("size"))) {
                const QStringList sizes = options[QStringLiteral("size")].toString().split('*');
                if (sizes.size() == 2) {
                    sz.setWidth(int(25.4 * sizes[0].toInt() / 85.));
                    sz.setHeight(int(25.4 * sizes[1].toInt() / 85.));
                }
            }
            if (options.contains(QStringLiteral("significance"))) {
                QVariantList sigs = options[QStringLiteral("significance")].toList();
                bool ok;
                for (const QVariant& v : sigs) {
                    int sig = v.toInt(&ok);
                    if (ok)
                        significances << sig;
                    else {
                        significances.clear();
                        break;
                    }
                }
            }
        }
    }

    if (array[0].type() == QVariant::List) { // 二维数组
        for (int i = 1; i < array.length(); ++i) {
            if (array[0].type() != QVariant::List)
                return ctx->throwError(QScriptContext::SyntaxError, QStringLiteral("Data array has error."));
        }
        table = new ComData::Table();
        bool x = true;
        for (int n = 0, len = array.length(); n < len; ++n) {
            QVariantList innerArr = array[n].toList();
            ComData::DoubleColumn* dc = new ComData::DoubleColumn(innerArr.length());
            for (int i = 0, sz = innerArr.length(); i < sz; ++i)
                dc->setCellPrimitive(i, innerArr[i].toDouble());

            if (significances.length() != len)
                dc->setSignificance(x ? ComData::ColumnBase::X : ComData::ColumnBase::Y);
            else
                dc->setSignificance(ComData::ColumnBase::Significance(significances[n]));
            x = false;
            table->append(dc);
        }
    } else { // 一维数组
        table = new ComData::Table();
        ComData::DoubleColumn* dc = new ComData::DoubleColumn(array.length());
        for (int i = 0, sz = array.length(); i < sz; ++i)
            dc->setCellPrimitive(i, array[i].toDouble());
        dc->setSignificance(ComData::ColumnBase::Y);
        table->append(dc);
    }
    QString path = ctx->argument(1).toString();

    QList<int> columns;
    for (int i = 0; i < table->size(); ++i)
        columns << i;


    ComDataGui::Graph::snapshot(table, columns, path, title, gt, sz);
    delete table;

    return engine->undefinedValue();
}

template <int type>
QScriptValue ui_messageBox(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count != 3  && count != 4)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is (parent, title, text, [detailedText])"));

    for (int i = 1; i < count; ++i) {
        if (!context->argument(i).isString())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is (parent, title, text, [detailedText])"));
    }

    QWidget* parent = qobject_cast<QWidget*>(context->argument(0).toQObject());
    QString title, text, detailed;
    title = context->argument(1).toString();
    text = context->argument(2).toString();
    if (count == 4)
        detailed = context->argument(3).toString();
    switch (type) {
    case 0:
        GuiUtils::CommonGuiUtils::information(parent, title, text, detailed);
        break;
    case 1:
        GuiUtils::CommonGuiUtils::warning(parent, title, text, detailed);
        break;
    case 2:
        GuiUtils::CommonGuiUtils::critical(parent, title, text, detailed);
        break;
    case 3:
        return GuiUtils::CommonGuiUtils::question(parent, title, text, detailed);
    }
    return eng->undefinedValue();
}

QScriptValue ui_getDouble(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count < 3 || count > 7)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getDouble(parent, title, label, [value=0], [min=-2147483647], [max=2147483647], [decimals=1])"));

    for (int i = 1; i < 3; ++i) {
        if (!context->argument(i).isString())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getDouble(parent, title, label, [value=0], [min=-2147483647], [max=2147483647], [decimals=1])"));
    }

    for (int i = 3; i < count; ++i) {
        if (!context->argument(i).isNumber())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getDouble(parent, title, label, [value=0], [min=-2147483647], [max=2147483647], [decimals=1])"));
    }

    QWidget* parent = qobject_cast<QWidget*>(context->argument(0).toQObject());
    QString title, label;
    title = context->argument(1).toString();
    label = context->argument(2).toString();

    double value = 0, min = -2147483647, max = 2147483647;
    int decimals = 1;
    QScriptValue temp = context->argument(3);
    if (temp.isUndefined())
        goto END;
    value = temp.toNumber();
    temp = context->argument(4);
    if (temp.isUndefined())
        goto END;
    min = temp.toNumber();
    temp = context->argument(5);
    if (temp.isUndefined())
        goto END;
    max = temp.toNumber();
    temp = context->argument(6);
    if (temp.isUndefined())
        goto END;
    decimals = temp.toInt32();

END:
    bool ok;
    double v = QInputDialog::getDouble(parent, title, label, value, min, max, decimals, &ok, Qt::Tool);
    return ok ? v : eng->undefinedValue();
}

QScriptValue ui_getInt(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count < 3 || count > 7)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getInt(parent, title, label, [value=0], [min=-2147483647], [max=2147483647], [step=1])"));

    for (int i = 1; i < 3; ++i) {
        if (!context->argument(i).isString())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getInt(parent, title, label, [value=0], [min=-2147483647], [max=2147483647], [step=1])"));
    }

    for (int i = 3; i < count; ++i) {
        if (!context->argument(i).isNumber())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getInt(parent, title, label, [value=0], [min=-2147483647], [max=2147483647], [step=1])"));
    }

    QWidget* parent = qobject_cast<QWidget*>(context->argument(0).toQObject());
    QString title, label;
    title = context->argument(1).toString();
    label = context->argument(2).toString();

    int value = 0, min = -2147483647, max = 2147483647;
    int decimals = 1;
    QScriptValue temp = context->argument(3);
    if (temp.isUndefined())
        goto END;
    value = temp.toInt32();
    temp = context->argument(4);
    if (temp.isUndefined())
        goto END;
    min = temp.toInt32();
    temp = context->argument(5);
    if (temp.isUndefined())
        goto END;
    max = temp.toInt32();
    temp = context->argument(6);
    if (temp.isUndefined())
        goto END;
    decimals = temp.toInt32();

END:
    bool ok;
    int v = QInputDialog::getInt(parent, title, label, value, min, max, decimals, &ok, Qt::Tool);
    return ok ? v : eng->undefinedValue();
}

QScriptValue ui_getText(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count < 3 || count > 5)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getText(parent, title, label, [text=''], [mode=ui.Normal])"));

    for (int i = 1; i < 3; ++i) {
        if (!context->argument(i).isString())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getText(parent, title, label, [text=''], [mode=ui.Normal])"));
    }

    if (count >= 4 && !context->argument(3).isString())
        return context->throwError(QScriptContext::SyntaxError,
               QStringLiteral("Function arguments is getText(parent, title, label, [text=''], [mode=ui.Normal])"));

    if (count == 5 && !context->argument(4).isNumber())
        return context->throwError(QScriptContext::SyntaxError,
               QStringLiteral("Function arguments is getText(parent, title, label, [text=''], [mode=ui.Normal])"));

    QWidget* parent = qobject_cast<QWidget*>(context->argument(0).toQObject());
    QString title, label, text;
    QLineEdit::EchoMode mode = QLineEdit::Normal;
    title = context->argument(1).toString();
    label = context->argument(2).toString();

    QScriptValue temp = context->argument(3);
    if (temp.isUndefined())
        goto END;
    text = temp.toString();
    temp = context->argument(4);
    if (temp.isUndefined())
        goto END;
    mode = QLineEdit::EchoMode(temp.toInt32());

END:
    bool ok;
    text = QInputDialog::getText(parent, title, label, mode, text, &ok, Qt::Tool);
    return ok ? text : eng->undefinedValue();
}

QScriptValue ui_getMultiLineText(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count < 3 || count > 4)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getMultiLineText(parent, title, label, [text=''])"));

    for (int i = 1; i < 3; ++i) {
        if (!context->argument(i).isString())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getMultiLineText(parent, title, label, [text=''])"));
    }

    if (count >= 4 && !context->argument(3).isString())
        return context->throwError(QScriptContext::SyntaxError,
               QStringLiteral("Function arguments is getMultiLineText(parent, title, label, [text=''])"));

    QWidget* parent = qobject_cast<QWidget*>(context->argument(0).toQObject());
    QString title, label, text;
    title = context->argument(1).toString();
    label = context->argument(2).toString();

    QScriptValue temp = context->argument(3);
    if (temp.isUndefined())
        goto END;
    text = temp.toString();

END:
    bool ok;
    text = QInputDialog::getMultiLineText(parent, title, label, text, &ok, Qt::Tool);
    return ok ? text : eng->undefinedValue();
}

QScriptValue ui_getExistingDirectory(QScriptContext *context, QScriptEngine *eng)
{
    Q_UNUSED(eng)
    const int count = context->argumentCount();
    if (count > 4)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getExistingDirectory([parent=null], [caption=''], [dir=''], [options=ui.ShowDirsOnly])"));

    if (count > 1 && !context->argument(1).isString())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getExistingDirectory([parent=null], [caption=''], [dir=''], [options=ui.ShowDirsOnly])"));

    if (count > 2 && !context->argument(2).isString())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getExistingDirectory([parent=null], [caption=''], [dir=''], [options=ui.ShowDirsOnly])"));

    if (count > 3 && !context->argument(3).isNumber())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getExistingDirectory([parent=null], [caption=''], [dir=''], [options=ui.ShowDirsOnly])"));

    QWidget* parent = nullptr;
    QString caption, dir;
    QFileDialog::Option options = QFileDialog::ShowDirsOnly;

    QScriptValue temp = context->argument(0);
    if (temp.isUndefined())
        goto END;
    parent = qobject_cast<QWidget*>(temp.toQObject());

    temp = context->argument(1);
    if (temp.isUndefined())
        goto END;
    caption = temp.toString();
    temp = context->argument(2);
    if (temp.isUndefined())
        goto END;
    dir = temp.toString();

    temp = context->argument(3);
    if (temp.isUndefined())
        goto END;
    options = QFileDialog::Option(temp.toInt32());

END:
    return QDir::toNativeSeparators(QFileDialog::getExistingDirectory(parent, caption, dir, options));
}

QScriptValue ui_getItem(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count < 3 || count > 6)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getItem(parent, title, label, items, [current=0], [editable=true])"));

    for (int i = 1; i < 3; ++i) {
        if (!context->argument(i).isString())
            return context->throwError(QScriptContext::SyntaxError,
                                       QStringLiteral("Function arguments is getItem(parent, title, label, items, [current=0], [editable=true])"));
                                           }

    if (count >= 4 && !context->argument(3).isArray())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is getItem(parent, title, label, items, [current=0], [editable=true])"));

    QWidget* parent = qobject_cast<QWidget*>(context->argument(0).toQObject());
    QString title, label;
    title = context->argument(1).toString();
    label = context->argument(2).toString();

    QStringList items = context->argument(3).toVariant().toStringList();
    int current = 0;
    bool editable = true;
    QScriptValue temp = context->argument(4);
    if (temp.isUndefined())
        goto END;
    current = temp.toInt32();
    temp = context->argument(5);
    if (temp.isUndefined())
        goto END;
    editable = temp.toBool();

END:
    bool ok;
    title = QInputDialog::getItem(parent, title, label, items, current, editable, &ok, Qt::Tool);
    return ok ? title : eng->undefinedValue();
}

template<int type>
QScriptValue ui_getOpenFileName(QScriptContext *context, QScriptEngine *eng)
{
    const int count = context->argumentCount();
    if (count > 4)
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is ([parent=null], [caption=''], [dir=''], [filter=''], [options=0])"));

    if (count > 1 && !context->argument(1).isString())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is ([parent=null], [caption=''], [dir=''], [filter=''], [options=0])"));

    if (count > 2 && !context->argument(2).isString())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is ([parent=null], [caption=''], [dir=''], [filter=''], [options=0])"));

    if (count > 3 && !context->argument(3).isString())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is ([parent=null], [caption=''], [dir=''], [filter=''], [options=0])"));

    if (count > 4 && !context->argument(4).isNumber())
        return context->throwError(QScriptContext::SyntaxError,
                                   QStringLiteral("Function arguments is ([parent=null], [caption=''], [dir=''], [filter=''], [options=0])"));

    QWidget* parent = nullptr;
    QString caption, dir, filter;
    QFileDialog::Option options = QFileDialog::ShowDirsOnly;

    QScriptValue temp = context->argument(0);
    if (temp.isUndefined())
        goto END;
    parent = qobject_cast<QWidget*>(temp.toQObject());

    temp = context->argument(1);
    if (temp.isUndefined())
        goto END;
    caption = temp.toString();

    temp = context->argument(2);
    if (temp.isUndefined())
        goto END;
    dir = temp.toString();

    temp = context->argument(3);
    if (temp.isUndefined())
        goto END;
    filter = temp.toString();

    temp = context->argument(4);
    if (temp.isUndefined())
        goto END;
    options = QFileDialog::Option(temp.toInt32());

END:
    switch (type) {
    case 0:
        return QDir::toNativeSeparators(QFileDialog::getOpenFileName(parent, caption, dir, filter, 0, options));
    case 1:
    {
        QStringList files;
        for (const QString& file : QFileDialog::getOpenFileNames(parent, caption, dir, filter, 0, options))
            files << QDir::toNativeSeparators(file);
        return eng->toScriptValue(files);
    }
    case 2:
        return QDir::toNativeSeparators(QFileDialog::getSaveFileName(parent, caption, dir, filter, 0, options));
    }
    return eng->undefinedValue();
}

void registerFunctions(QScriptValue& uiProto, QScriptEngine *eng)
{
    std::map<QString, QScriptEngine::FunctionSignature> uiMap {
        { QStringLiteral("uiInteractive"), uiInteractive },
        { QStringLiteral("information"), ui_messageBox<0> },
        { QStringLiteral("warning"),     ui_messageBox<1> },
        { QStringLiteral("critical"),    ui_messageBox<2> },
        { QStringLiteral("question"),    ui_messageBox<3> },
        { QStringLiteral("getDouble"),   ui_getDouble },
        { QStringLiteral("getInt"),      ui_getInt },
        { QStringLiteral("getText"),     ui_getText },
        { QStringLiteral("getMultiLineText"), ui_getMultiLineText },
        { QStringLiteral("getItem"), ui_getItem },
        { QStringLiteral("getExistingDirectory"), ui_getExistingDirectory },
        { QStringLiteral("getOpenFileName"), ui_getOpenFileName<0> },
        { QStringLiteral("getOpenFileNames"), ui_getOpenFileName<1> },
        { QStringLiteral("getSaveFileName"), ui_getOpenFileName<2> },
        { QStringLiteral("saveGraph"), ui_saveGraph }
    };

    for (auto it = uiMap.cbegin(); it != uiMap.cend(); ++it)
        uiProto.setProperty(it->first, eng->newFunction(it->second));

    uiProto.setProperty(QStringLiteral("Normal"), QLineEdit::Normal);
    uiProto.setProperty(QStringLiteral("NoEcho"), QLineEdit::NoEcho);
    uiProto.setProperty(QStringLiteral("Password"), QLineEdit::Password);
    uiProto.setProperty(QStringLiteral("PasswordEchoOnEdit"), QLineEdit::PasswordEchoOnEdit);

    uiProto.setProperty(QStringLiteral("ShowDirsOnly"), QFileDialog::ShowDirsOnly);
    uiProto.setProperty(QStringLiteral("DontResolveSymlinks"), QFileDialog::DontResolveSymlinks);
    uiProto.setProperty(QStringLiteral("DontConfirmOverwrite"), QFileDialog::DontConfirmOverwrite);
//    uiProto.setProperty(QStringLiteral("DontUseSheet"), QFileDialog::DontUseSheet);
    uiProto.setProperty(QStringLiteral("DontUseNativeDialog"), QFileDialog::DontUseNativeDialog);
    uiProto.setProperty(QStringLiteral("ReadOnly"), QFileDialog::ReadOnly);
    uiProto.setProperty(QStringLiteral("HideNameFilterDetails"), QFileDialog::HideNameFilterDetails);
    uiProto.setProperty(QStringLiteral("DontUseCustomDirectoryIcons"), QFileDialog::DontUseCustomDirectoryIcons);

    uiProto.setProperty(QStringLiteral("Area"), ComDataGui::GT_Area);
    uiProto.setProperty(QStringLiteral("StackArea"), ComDataGui::GT_StackArea);
    uiProto.setProperty(QStringLiteral("FillArea"), ComDataGui::GT_FillArea);
    uiProto.setProperty(QStringLiteral("Column"), ComDataGui::GT_Column);
    uiProto.setProperty(QStringLiteral("ColumnPlusLabel"), ComDataGui::GT_ColumnPlusLabel);
    uiProto.setProperty(QStringLiteral("Bar"), ComDataGui::GT_Bar);
    uiProto.setProperty(QStringLiteral("BarPlusLabel"), ComDataGui::GT_BarPlusLabel);
    uiProto.setProperty(QStringLiteral("StackColumn"), ComDataGui::GT_StackColumn);
    uiProto.setProperty(QStringLiteral("StackBar"), ComDataGui::GT_StackBar);
    uiProto.setProperty(QStringLiteral("FloatingColumn"), ComDataGui::GT_FloatingColumn);
    uiProto.setProperty(QStringLiteral("FloatingBar"), ComDataGui::GT_FloatingBar);
    uiProto.setProperty(QStringLiteral("Bubble"), ComDataGui::GT_Bubble);
    uiProto.setProperty(QStringLiteral("Line"), ComDataGui::GT_Line);
    uiProto.setProperty(QStringLiteral("HorizontalStep"), ComDataGui::GT_HorizontalStep);
    uiProto.setProperty(QStringLiteral("VerticalStep"), ComDataGui::GT_VerticalStep);
    uiProto.setProperty(QStringLiteral("Stick"), ComDataGui::GT_Stick);
    uiProto.setProperty(QStringLiteral("SplineConnected"), ComDataGui::GT_SplineConnected);

    uiProto.setProperty(QStringLiteral("X"), ComData::ColumnBase::X);
    uiProto.setProperty(QStringLiteral("Y"), ComData::ColumnBase::Y);
    uiProto.setProperty(QStringLiteral("Z"), ComData::ColumnBase::Z);
    uiProto.setProperty(QStringLiteral("V"), ComData::ColumnBase::V);
    uiProto.setProperty(QStringLiteral("VX"), ComData::ColumnBase::VX);
    uiProto.setProperty(QStringLiteral("VY"), ComData::ColumnBase::VY);
    uiProto.setProperty(QStringLiteral("VZ"), ComData::ColumnBase::VZ);
    uiProto.setProperty(QStringLiteral("XError"), ComData::ColumnBase::XError);
    uiProto.setProperty(QStringLiteral("YError"), ComData::ColumnBase::YError);
//    uiProto.setProperty(QStringLiteral("Label"), ComData::ColumnBase::Label);
//    uiProto.setProperty(QStringLiteral("Disregard"), ComData::ColumnBase::Disregard);
}

UIScript::UIScript()
{
    Q_ASSERT(!stUIScript);
    if (!stUIScript)
        stUIScript = this;
}

QStringList UIScript::keys() const
{
    QStringList result;
    result << QStringLiteral("ui");
    return result;
}

void UIScript::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QStringLiteral("ui")) {
        QScriptValue global = engine->globalObject();
        QScriptString ui = engine->toStringHandle(QStringLiteral("ui"));
        QScriptValue uiObj = engine->newObject();
        global.setProperty(ui, uiObj, QScriptValue::Undeletable);
        QScriptValue uiLoaderClass = engine->scriptValueFromQMetaObject<UiLoader>();
        uiObj.setProperty(QStringLiteral("UiLoader"), uiLoaderClass);

        registerFunctions(uiObj, engine);
    }
}

QWidget* UIScript::uiInteractiveDelegate(QScriptEngine *engine, const QString &code)
{
    QScriptValue res = engine->evaluate(code);
    qDebug() << "delegate: " << res.toString();
    if (engine->hasUncaughtException()) {
        qDebug() << "Exception: " << engine->uncaughtException().toString();
        return nullptr;
    }
    return qscriptvalue_cast<QWidget*>(res);
}
