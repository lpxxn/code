#include "docxdocument.h"
#include "typedeclare.h"

#include <QScriptEngine>
#include <QDebug>

#include <docx/document.h>
#include <docx/shared.h>

using namespace Docx;

DocxDocument::DocxDocument(QObject *parent) : QObject(parent)
{
    m_document = new Document();
}

DocxDocument::DocxDocument(const QString &name, QObject *parent) : QObject(parent)
{
    m_document = new Document(name);
}

DocxDocument::DocxDocument(QIODevice *device, QObject *parent) : QObject(parent)
{
    m_document = new Document(device);
}

QScriptValue DocxDocument::addParagraph(const QString &text, ParagraphType type)
{
    Paragraph *p = nullptr;
    if (type == ParagraphType::ListBullet) {
        p = m_document->addParagraph(text, QStringLiteral("ListBullet"));
    } else if (type == ParagraphType::ListNumber){
        p = m_document->addParagraph(text, QStringLiteral("ListNumber"));
    } else {
        p = m_document->addParagraph(text);
    }
    return engine()->toScriptValue(p);
}

QScriptValue DocxDocument::addHeading(const QString &text, int level)
{
    Paragraph *p = m_document->addHeading(text, level);
    return engine()->toScriptValue(p);
}

QScriptValue DocxDocument::addTable(int rows, int cols, const QString &style)
{
    Docx::Table *table = m_document->addTable(rows, cols, style);
    return engine()->toScriptValue(table);
}

QScriptValue DocxDocument::addPicture(const QString &path, int width, int height)
{
    Docx::Length lw, lh;
    if (width > 0)
        lw = Docx::Cm::emus(width);

    if (height > 0)
        lh = Docx::Cm::emus(height);

    Docx::InlineShape *shap = m_document->addPicture(path, lw, lh);
    return engine()->toScriptValue(shap);
}

QScriptValue DocxDocument::addPageBreak()
{
    Paragraph *p = m_document->addPageBreak();
    return engine()->toScriptValue(p);
}

QScriptValue DocxDocument::paragraphs()
{
    QList<Docx::Paragraph*> ps = m_document->paragraphs();
    return engine()->toScriptValue(ps);
}

QScriptValue DocxDocument::tables()
{
    QList<Docx::Table*> tables = m_document->tables();
    return engine()->toScriptValue(tables);
}

QScriptValue DocxDocument::addSection()
{
    Docx::Section *section = m_document->addSection();
    return engine()->toScriptValue(section);
}

DocxDocument::~DocxDocument()
{
    qDebug() << "delete DocxDocument.";
    delete m_document;
}

void DocxDocument::save(const QString &path)
{
    m_document->save(path);
}

