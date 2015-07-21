#include "officescript.h"
#include "docxdocument.h"
#include "xlsxdocument.h"
#include "xlsxhelper.h"
#include "docxhelper.h"

#include <comutils/licensemanager.h>

#include <QScriptEngine>
#include <QIODevice>
#include <QDebug>

#include <map>

/*!
 * \brief 构造Xlsx的文档对象.
 * \param context 解释器上下文对象
 * \param engine 解释器对象
 * \return 在解释器中构造的对象
 */
static QScriptValue constructXlsxDocument(QScriptContext* context, QScriptEngine* engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use the 'new' operator."));
    const int count = context->argumentCount();
    if (count > 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use 'new office.Xlsx([name|file])'."));
    if (count == 1) {
        QScriptValue arg1 = context->argument(0);
        if (arg1.isString()) {
            XlsxDocument* xlsx = new XlsxDocument(arg1.toString());
            return engine->newQObject(xlsx, QScriptEngine::ScriptOwnership,
                                      QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
        } else if (arg1.isQObject()) {
            QIODevice* io = qobject_cast<QIODevice*>(arg1.toQObject());
            if (io) {
                XlsxDocument* xlsx = new XlsxDocument(io);
                return engine->newQObject(xlsx, QScriptEngine::ScriptOwnership,
                                          QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
            }
        }
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use 'new office.Xlsx([name|file])'."));
    } else {
        XlsxDocument* xlsx = new XlsxDocument();
        return engine->newQObject(xlsx, QScriptEngine::ScriptOwnership,
                                  QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    }
}

/*!
 * \brief 构造Docx的文档对象.
 * \param context 解释器上下文对象
 * \param engine 解释器对象
 * \return 在解释器中构造的对象
 */
static QScriptValue constructDocxDocument(QScriptContext* context, QScriptEngine* engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use the 'new' operator."));
    const int count = context->argumentCount();
    if (count > 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use 'new office.Docx([name|file])'."));
    if (count == 1) {
        QScriptValue arg1 = context->argument(0);
        if (arg1.isString()) {
            DocxDocument* docx = new DocxDocument(arg1.toString());
            return engine->newQObject(docx, QScriptEngine::ScriptOwnership,
                                      QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
        } else if (arg1.isQObject()) {
            QIODevice* io = qobject_cast<QIODevice*>(arg1.toQObject());
            if (io) {
                DocxDocument* docx = new DocxDocument(io);
                return engine->newQObject(docx, QScriptEngine::ScriptOwnership,
                                          QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
            }
        }
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please use 'new office.Docx([name|file])'."));
    } else {
        DocxDocument* docx = new DocxDocument();
        return engine->newQObject(docx, QScriptEngine::ScriptOwnership,
                                  QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    }
}

static void registerFunctions(QScriptValue& officeProto, QScriptEngine *eng)
{
    std::map<QString, QScriptEngine::FunctionSignature> officeMap {
//        { QStringLiteral("uiInteractive"), uiInteractive },

    };

    for (auto it = officeMap.cbegin(); it != officeMap.cend(); ++it)
        officeProto.setProperty(it->first, eng->newFunction(it->second));

//    officeProto.setProperty(QStringLiteral("Normal"), QLineEdit::Normal);
}

OfficeScript::OfficeScript()
{
}

QStringList OfficeScript::keys() const
{
    QStringList result;
    result << QStringLiteral("office");
    return result;
}

void OfficeScript::initialize(const QString &key, QScriptEngine *engine)
{
    CommonUtils::LicenseManager* licMng = CommonUtils::LicenseManager::instance();
    Q_ASSERT(licMng);
//    if (!licMng->checkout(QStringLiteral("SC.OFFICE"), licMng->featureVersion()))
//        return;

    if (key == QStringLiteral("office")) {
        QScriptValue global = engine->globalObject();
        QScriptString office = engine->toStringHandle(QStringLiteral("office"));
        QScriptValue officeObj = engine->newObject();
        global.setProperty(office, officeObj, QScriptValue::Undeletable);

        // docx
        QScriptValue docxCtor = engine->newFunction(constructDocxDocument);
        QScriptValue ddocxMetaObject = engine->newQMetaObject(&DocxDocument::staticMetaObject, docxCtor);
        officeObj.setProperty(QStringLiteral("Docx"), ddocxMetaObject);
        QScriptValue docx = officeObj.property(QStringLiteral("Docx"));
        DocxHelper::registerParagraph(engine, docx);
        DocxHelper::registerRun(engine, docx);
        DocxHelper::registerTable(engine, docx);
        DocxHelper::registerCell(engine, docx);
        DocxHelper::registerListCell(engine, docx);
        DocxHelper::registerInlineShape(engine, docx);
        DocxHelper::registerListTables(engine, docx);
        DocxHelper::registerListParagraphs(engine, docx);
        DocxHelper::registerSection(engine, docx);
        DocxHelper::registerHeader(engine, docx);
        DocxHelper::registerFooter(engine, docx);

        // xlsx
        QScriptValue xlsxCtor = engine->newFunction(constructXlsxDocument);
        QScriptValue metaObject = engine->newQMetaObject(&XlsxDocument::staticMetaObject, xlsxCtor);
        officeObj.setProperty(QStringLiteral("Xlsx"), metaObject);

        QScriptValue xlsx = officeObj.property(QStringLiteral("Xlsx"));
        XlsxHelper::registerFormat(engine, xlsx);
        XlsxHelper::registerCell(engine, xlsx);
        XlsxHelper::registerImage(engine, xlsx);
        XlsxHelper::registerWorksheet(engine, xlsx);

        registerFunctions(officeObj, engine);
    }
}

