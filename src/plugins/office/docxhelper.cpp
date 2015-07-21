#include "docxhelper.h"
#include "typedeclare.h"
#include "docxdocument.h"

#include <QScriptEngine>



// Paragraph相关
static QScriptValue DocxParagraph_insertParagraphBefore(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Paragraph *p = qscriptvalue_cast<Docx::Paragraph*>(context->thisObject());
    if (!p)
        return context->throwError(QStringLiteral("It is not a valid Paragraph."));

    const int args = context->argumentCount();
    if (args < 1 || args > 2)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one or two arguments."));

    Docx::Paragraph *newp = nullptr;
    if (args == 1) {
        newp = p->insertParagraphBefore(context->argument(0).toString());
    } else {        
        int type = context->argument(1).toInt32();
        if (type == DocxDocument::ParagraphType::ListBullet) {
            newp = p->insertParagraphBefore(context->argument(0).toString(), QStringLiteral("ListBullet"));
        } else {
            newp = p->insertParagraphBefore(context->argument(0).toString(), QStringLiteral("ListNumber"));
        }
    }
    return eng->toScriptValue(newp);
}

static QScriptValue DocxParagraph_addText(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Paragraph *p = qscriptvalue_cast<Docx::Paragraph*>(context->thisObject());
    if (!p)
        return context->throwError(QStringLiteral("It is not a valid Paragraph."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    p->addText(context->argument(0).toString());
    return true;
}

static QScriptValue DocxParagraph_addRun(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Paragraph *p = qscriptvalue_cast<Docx::Paragraph*>(context->thisObject());
    if (!p)
        return context->throwError(QStringLiteral("It is not a valid Paragraph."));

    const int args = context->argumentCount();
    if (args > 2)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide less than three arguments."));

    Docx::Run *r = nullptr;
    if (args == 0) {
        r = p->addRun();
    } else if (args == 1){
        r = p->addRun(context->argument(0).toString());
    } else {
        r = p->addRun(context->argument(0).toString(), context->argument(1).toString());
    }

    return eng->toScriptValue(r);
}

static QScriptValue DocxParagraph_setAlignment(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Paragraph *p = qscriptvalue_cast<Docx::Paragraph*>(context->thisObject());
    if (!p)
        return context->throwError(QStringLiteral("It is not a valid Paragraph."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    QString str = context->argument(0).toString();
    const QVariant align = context->argument(0).toVariant();
    p->setAlignment((Docx::WD_PARAGRAPH_ALIGNMENT)align.toInt());
    return true;
}

// 注册 Paragraph
void DocxHelper::registerParagraph(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Paragraph*>(), proto);
    parentProperty.setProperty(QStringLiteral("Paragraph"), proto);

    QScriptValue paragraph = parentProperty.property(QStringLiteral("Paragraph"));

    paragraph.setProperty(QStringLiteral("insertParagraphBefore"), eng->newFunction(DocxParagraph_insertParagraphBefore));
    paragraph.setProperty(QStringLiteral("addRun"), eng->newFunction(DocxParagraph_addRun));
    paragraph.setProperty(QStringLiteral("addText"), eng->newFunction(DocxParagraph_addText));
}

// Run相关方法
static QScriptValue DocxRun_addText(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->addText(context->argument(0).toString());
    return true;
}

static QScriptValue DocxRun_addPicture(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();

    if (args < 1 || args > 3)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide between one and three arguments."));

    Docx::Length lw, lh;
    if (args == 2)
        lw = Docx::Cm::emus(context->argument(1).toNumber());

    if (args == 3)
        lh = Docx::Cm::emus(context->argument(2).toNumber());

    Docx::InlineShape *shap = r->addPicture(context->argument(0).toString(), lw, lh);

    return eng->toScriptValue(shap);

}

static QScriptValue DocxRun_setUnderLine(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setUnderLine((Docx::WD_UNDERLINE)(context->argument(0).toInt32()));
    return true;
}

static QScriptValue DocxRun_addTab(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("no arguments."));

    r->addTab();
    return true;
}

static QScriptValue DocxRun_setAllcaps(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setAllcaps(context->argument(0).toBool());
    return true;
}

static QScriptValue DocxRun_setBold(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setBold(context->argument(0).toBool());
    return true;
}

static QScriptValue DocxRun_setDoubleStrike(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setDoubleStrike(context->argument(0).toBool());
    return true;
}

static QScriptValue DocxRun_setShadow(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setShadow(context->argument(0).toBool());
    return true;
}

static QScriptValue DocxRun_setSmallcaps(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setSmallcaps(context->argument(0).toBool());
    return true;
}

static QScriptValue DocxRun_setText(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    r->setText(context->argument(0).toString());
    return true;
}

static QScriptValue DocxRun_text(QScriptContext *context, QScriptEngine * eng)
{
    Docx::Run *r = qscriptvalue_cast<Docx::Run*>(context->thisObject());
    if (!r)
        return context->throwError(QStringLiteral("It is not a valid Run."));

    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("no argument."));

    return eng->toScriptValue(r->text());

}

// 注册Run
void DocxHelper::registerRun(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Run*>(), proto);
    parentProperty.setProperty(QStringLiteral("Run"), proto);

    proto.setProperty(QStringLiteral("addText"), eng->newFunction(DocxRun_addText));
    proto.setProperty(QStringLiteral("addPicture"), eng->newFunction(DocxRun_addPicture));
    proto.setProperty(QStringLiteral("addTab"), eng->newFunction(DocxRun_addTab));
    proto.setProperty(QStringLiteral("setAllcaps"), eng->newFunction(DocxRun_setAllcaps));
    proto.setProperty(QStringLiteral("setBold"), eng->newFunction(DocxRun_setBold));
    proto.setProperty(QStringLiteral("setDoubleStrike"), eng->newFunction(DocxRun_setDoubleStrike));
    proto.setProperty(QStringLiteral("setSmallcaps"), eng->newFunction(DocxRun_setSmallcaps));
    proto.setProperty(QStringLiteral("setShadow"), eng->newFunction(DocxRun_setShadow));

    proto.setProperty(QStringLiteral("text"), eng->newFunction(DocxRun_setText), QScriptValue::PropertySetter);
    proto.setProperty(QStringLiteral("text"), eng->newFunction(DocxRun_text), QScriptValue::PropertyGetter);


    proto.setProperty(QStringLiteral("setUnderLine"), eng->newFunction(DocxRun_setUnderLine));

    proto.setProperty(QStringLiteral("SINGLE"), (int)Docx::WD_UNDERLINE::SINGLE);
    proto.setProperty(QStringLiteral("WORDS"), (int)Docx::WD_UNDERLINE::WORDS);
    proto.setProperty(QStringLiteral("DOUBLE"), (int)Docx::WD_UNDERLINE::DOUBLE);
    proto.setProperty(QStringLiteral("DOTTED"), (int)Docx::WD_UNDERLINE::DOTTED);
    proto.setProperty(QStringLiteral("THICK"), (int)Docx::WD_UNDERLINE::THICK);
    proto.setProperty(QStringLiteral("DASH"), (int)Docx::WD_UNDERLINE::DASH);
    proto.setProperty(QStringLiteral("DOT_DASH"), (int)Docx::WD_UNDERLINE::DOT_DASH);
    proto.setProperty(QStringLiteral("DOT_DOT_DASH"), (int)Docx::WD_UNDERLINE::DOT_DOT_DASH);
    proto.setProperty(QStringLiteral("WAVY"), (int)Docx::WD_UNDERLINE::WAVY);
    proto.setProperty(QStringLiteral("DOTTED_HEAVY"), (int)Docx::WD_UNDERLINE::DOTTED_HEAVY);
    proto.setProperty(QStringLiteral("DASH_HEAVY"), (int)Docx::WD_UNDERLINE::DASH_HEAVY);
    proto.setProperty(QStringLiteral("DOT_DASH_HEAVY"), (int)Docx::WD_UNDERLINE::DOT_DASH_HEAVY);
    proto.setProperty(QStringLiteral("DOT_DOT_DASH_HEAVY"), (int)Docx::WD_UNDERLINE::DOT_DOT_DASH_HEAVY);
    proto.setProperty(QStringLiteral("WAVY_HEAVY"), (int)Docx::WD_UNDERLINE::WAVY_HEAVY);
    proto.setProperty(QStringLiteral("DASH_LONG"), (int)Docx::WD_UNDERLINE::DASH_LONG);
    proto.setProperty(QStringLiteral("WAVY_DOUBLE"), (int)Docx::WD_UNDERLINE::WAVY_DOUBLE);
    proto.setProperty(QStringLiteral("DASH_LONG_HEAVY"), (int)Docx::WD_UNDERLINE::DASH_LONG_HEAVY);
}

// Table相关方法
static QScriptValue DocxTable_rowCells(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Table *table = qscriptvalue_cast<Docx::Table*>(context->thisObject());
    if (!table)
        return context->throwError(QStringLiteral("It is not a valid Table."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    QList<Docx::Cell*> cells = table->rowCells(context->argument(0).toNumber());
    return eng->toScriptValue(cells);
}

// 注册 Table
void DocxHelper::registerTable(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Table*>(), proto);
    parentProperty.setProperty(QStringLiteral("Table"), proto);

    proto.setProperty(QStringLiteral("rowCells"), eng->newFunction(DocxTable_rowCells));
}

// Cell 相关方法
static QScriptValue DocxCell_addText(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Cell *cell = qscriptvalue_cast<Docx::Cell*>(context->thisObject());
    if (!cell)
        return context->throwError(QStringLiteral("It is not a valid Cell."));

    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    cell->addText(context->argument(0).toString());
    return true;
}

static QScriptValue DocxCell_addParagraph(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Cell *cell = qscriptvalue_cast<Docx::Cell*>(context->thisObject());
    if (!cell)
        return context->throwError(QStringLiteral("It is not a valid Cell."));

    const int args = context->argumentCount();
    if (args > 2)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide less than two arguments."));

    Docx::Paragraph *p = nullptr;
    if (args == 0) {
        p = cell->addParagraph();
    } else if (args == 1) {
        p = cell->addParagraph(context->argument(0).toString());
    } else {
        int type = context->argument(1).toInt32();
        if (type == (int)DocxDocument::ParagraphType::ListBullet) {
            p = cell->addParagraph(context->argument(0).toString(), QStringLiteral("ListBullet"));
        } else {
            p = cell->addParagraph(context->argument(0).toString(), QStringLiteral("ListNumber"));
        }
    }
    return eng->toScriptValue(p);
}

static QScriptValue DocxCell_addTable(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Cell *cell = qscriptvalue_cast<Docx::Cell*>(context->thisObject());
    if (!cell)
        return context->throwError(QStringLiteral("It is not a valid Cell."));

    const int args = context->argumentCount();
    if (args < 2  || args > 3)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide two or three arguments."));

    Docx::Table *table = nullptr;
    if (args == 2) {
        table = cell->addTable(context->argument(0).toNumber(), context->argument(1).toNumber());
    } else {
        table = cell->addTable(context->argument(0).toNumber(), context->argument(1).toNumber(), context->argument(2).toString());
    }

    return eng->toScriptValue(table);
}

// 注册Cell
void DocxHelper::registerCell(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Cell*>(), proto);
    parentProperty.setProperty(QStringLiteral("Cell"), proto);

    proto.setProperty(QStringLiteral("addText"), eng->newFunction(DocxCell_addText));
    proto.setProperty(QStringLiteral("addParagraph"), eng->newFunction(DocxCell_addParagraph));
    proto.setProperty(QStringLiteral("addTable"), eng->newFunction(DocxCell_addTable));
}

static QScriptValue DocxCells_at(QScriptContext *context, QScriptEngine *eng)
{
    QList<Docx::Cell*> cells = qscriptvalue_cast<QList<Docx::Cell*>>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    return eng->toScriptValue(cells.at(context->argument(0).toNumber()));
}

static QScriptValue DocxCells_count(QScriptContext *context, QScriptEngine * /*eng*/)
{
    QList<Docx::Cell*> cells = qscriptvalue_cast<QList<Docx::Cell*>>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please no need argument."));

    return cells.count();
}

//注册 QList<Cell*>
void DocxHelper::registerListCell(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<QList<Docx::Cell*>>(), proto);
    parentProperty.setProperty(QStringLiteral("Cells"), proto);

    proto.setProperty(QStringLiteral("at"), eng->newFunction(DocxCells_at));
    proto.setProperty(QStringLiteral("count"), eng->newFunction(DocxCells_count));
}

// 注册InlineShape*
void DocxHelper::registerInlineShape(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::InlineShape*>(), proto);
    parentProperty.setProperty(QStringLiteral("InlineShape"), proto);

}

static QScriptValue DocxListTables_at(QScriptContext *context, QScriptEngine *eng)
{
    QList<Docx::Table*> cells = qscriptvalue_cast<QList<Docx::Table*>>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    return eng->toScriptValue(cells.at(context->argument(0).toNumber()));
}

static QScriptValue DocxListTables_count(QScriptContext *context, QScriptEngine * /*eng*/)
{
    QList<Docx::Table*> tables = qscriptvalue_cast<QList<Docx::Table*>>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please no need argument."));

    return tables.count();
}

// 注册List<Table*>
void DocxHelper::registerListTables(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<QList<Docx::Table*>>(), proto);
    parentProperty.setProperty(QStringLiteral("Tables"), proto);

    proto.setProperty(QStringLiteral("at"), eng->newFunction(DocxListTables_at));
    proto.setProperty(QStringLiteral("count"), eng->newFunction(DocxListTables_count));
}

static QScriptValue DocxListParagraphs_at(QScriptContext *context, QScriptEngine *eng)
{
    QList<Docx::Paragraph*> ps = qscriptvalue_cast<QList<Docx::Paragraph*>>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please provide one argument."));

    return eng->toScriptValue(ps.at(context->argument(0).toNumber()));
}

static QScriptValue DocxListParagraphs_count(QScriptContext *context, QScriptEngine * /*eng*/)
{
    QList<Docx::Paragraph*> ps = qscriptvalue_cast<QList<Docx::Paragraph*>>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("please no need argument."));

    return ps.count();
}

// 注册List<Paragraph*>
void DocxHelper::registerListParagraphs(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<QList<Docx::Paragraph*>>(), proto);
    parentProperty.setProperty(QStringLiteral("Paragraph"), proto);

    proto.setProperty(QStringLiteral("at"), eng->newFunction(DocxListParagraphs_at));
    proto.setProperty(QStringLiteral("count"), eng->newFunction(DocxListParagraphs_count));
}

static QScriptValue DocxSection_defaultHeader(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Section *section = qscriptvalue_cast<Docx::Section*>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("no need argument."));

    return eng->toScriptValue(section->defaultHeader());
}

static QScriptValue DocxSection_defaultFooter(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Section *section = qscriptvalue_cast<Docx::Section*>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("no need argument."));

    return eng->toScriptValue(section->defaultFooter());
}

static QScriptValue DocxSection_restarNum(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Section *section = qscriptvalue_cast<Docx::Section*>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 0)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("no need argument."));

    section->restartNum();
    return true;
}

// 注册 Section
void DocxHelper::registerSection(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Section*>(), proto);
    parentProperty.setProperty(QStringLiteral("Section"), proto);
    QScriptValue section = parentProperty.property(QStringLiteral("Section"));

    section.setProperty(QStringLiteral("defaultHeader"), eng->newFunction(DocxSection_defaultHeader));
    section.setProperty(QStringLiteral("defaultFooter"), eng->newFunction(DocxSection_defaultFooter));
    section.setProperty(QStringLiteral("restartNum"), eng->newFunction(DocxSection_restarNum));

    section.setProperty(QStringLiteral("NormalNum"), Docx::PageNumberType::NormalNum);
    section.setProperty(QStringLiteral("RomanNum"), Docx::PageNumberType::RomanNum);
}

static QScriptValue DocxHeader_addParagraph(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Header *header = qscriptvalue_cast<Docx::Header*>(context->thisObject());
    const int args = context->argumentCount();
    if (args > 2)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("less than two arguments."));

    if (args == 0)
        return eng->toScriptValue(header->addParagraph());

    if (args == 1)
        return eng->toScriptValue(header->addParagraph(context->argument(0).toString()));

    int type = context->argument(1).toInt32();
    Docx::Paragraph *p = nullptr;
    if (type == (int)DocxDocument::ParagraphType::ListBullet) {
        p = header->addParagraph(context->argument(0).toString(), QStringLiteral("ListBullet"));
    } else {
        p = header->addParagraph(context->argument(0).toString(), QStringLiteral("ListNumber"));
    }
    return eng->toScriptValue(p);
}

static QScriptValue DocxHeader_showNumber(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Header *header = qscriptvalue_cast<Docx::Header*>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("need one argument."));

    header->showNumber((Docx::PageNumberType)context->argument(0).toInt32());
    return true;
}

// 注册 Header
void DocxHelper::registerHeader(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Header*>(), proto);
    parentProperty.setProperty(QStringLiteral("Header"), proto);

    proto.setProperty(QStringLiteral("addParagraph"), eng->newFunction(DocxHeader_addParagraph));
    proto.setProperty(QStringLiteral("showNumber"), eng->newFunction(DocxHeader_showNumber));

}

static QScriptValue DocxFooter_addParagraph(QScriptContext *context, QScriptEngine *eng)
{
    Docx::Footer *footer = qscriptvalue_cast<Docx::Footer*>(context->thisObject());
    const int args = context->argumentCount();
    if (args > 2)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("less than two arguments."));

    if (args == 0)
        return eng->toScriptValue(footer->addParagraph());

    if (args == 1)
        return eng->toScriptValue(footer->addParagraph(context->argument(0).toString()));

    int type = context->argument(1).toInt32();
    Docx::Paragraph *p = nullptr;
    if (type == (int)DocxDocument::ParagraphType::ListBullet) {
        p = footer->addParagraph(context->argument(0).toString(), QStringLiteral("ListBullet"));
    } else {
        p = footer->addParagraph(context->argument(0).toString(), QStringLiteral("ListNumber"));
    }
    return eng->toScriptValue(p);
}

static QScriptValue DocxFooter_showNumber(QScriptContext *context, QScriptEngine * /*eng*/)
{
    Docx::Footer *footer = qscriptvalue_cast<Docx::Footer*>(context->thisObject());
    const int args = context->argumentCount();
    if (args != 1)
        return context->throwError(QScriptContext::SyntaxError, QObject::tr("need one argument."));

    footer->showNumber((Docx::PageNumberType)context->argument(0).toInt32());
    return true;
}

// 注册 Footer
void DocxHelper::registerFooter(QScriptEngine *eng, QScriptValue &parentProperty)
{
    QScriptValue proto = eng->newObject();
    eng->setDefaultPrototype(qMetaTypeId<Docx::Footer*>(), proto);
    parentProperty.setProperty(QStringLiteral("Footer"), proto);

    proto.setProperty(QStringLiteral("addParagraph"), eng->newFunction(DocxFooter_addParagraph));
    proto.setProperty(QStringLiteral("showNumber"), eng->newFunction(DocxFooter_showNumber));
}


