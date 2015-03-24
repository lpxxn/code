#include "xlsxdocument.h"
#include "typedeclare.h"

#include <QXlsx/xlsxdocument.h>

#include <QScriptEngine>
#include <QDebug>

using namespace QXlsx;

XlsxDocument::XlsxDocument(QObject *parent)
{
    m_document = new Document(parent);
}

XlsxDocument::XlsxDocument(const QString &name, QObject *parent)
{
    m_document = new Document(name, parent);
}

XlsxDocument::XlsxDocument(QIODevice *device, QObject *parent) : QObject(parent)
{
    m_document = new Document(device, parent);
}

XlsxDocument::~XlsxDocument()
{
    qDebug() << "delete XlsxDocument.";
    delete m_document;
}

QScriptValue XlsxDocument::cellAt() const
{
    const int n = argumentCount();
    Cell* cell = nullptr;
    if (n == 2 && argument(0).isNumber() && argument(1).isNumber()) {
        int row = argument(0).toInt32();
        int col = argument(1).toInt32();
        cell = m_document->cellAt(row, col);
    } else if (n == 1 && argument(0).isString()) {
        cell = m_document->cellAt(argument(0).toString());
    }

    return cell ? engine()->toScriptValue(cell) : engine()->undefinedValue();
}

bool XlsxDocument::copySheet(const QString &srcName, const QString &distName)
{
    return m_document->copySheet(srcName, distName);
}

bool XlsxDocument::deleteSheet(const QString &name)
{
    return m_document->deleteSheet(name);
}

QString XlsxDocument::documentProperty(const QString &key) const
{
    return m_document->documentProperty(key);
}

QStringList XlsxDocument::documentPropertyNames() const
{
    return m_document->documentPropertyNames();
}

bool XlsxDocument::addSheet(const QString &name, XlsxDocument::SheetType type)
{
    return m_document->addSheet(name, AbstractSheet::SheetType(type));
}

bool XlsxDocument::save() const
{
    return m_document->save();
}

bool XlsxDocument::saveAs(const QString &name) const
{
    return m_document->saveAs(name);
}

bool XlsxDocument::write()
{
    const int n = argumentCount();
    QXlsx::Format f;
    if (n >= 3 && argument(0).isNumber() && argument(1).isNumber()) {
        int row = argument(0).toInt32();
        int col = argument(1).toInt32();
        if (n == 4)
            f = qscriptvalue_cast<QXlsx::Format>(argument(3));
        return m_document->write(row, col, argument(2).toVariant(), f);
    } else if (n >= 2 && argument(0).isString()) {
        if (n == 3)
            f = qscriptvalue_cast<QXlsx::Format>(argument(2));
        return m_document->write(argument(0).toString(), argument(1).toVariant(), f);
    }
    return false;
}

QVariant XlsxDocument::read() const
{
    const int n = argumentCount();
    if (n == 2 && argument(0).isNumber() && argument(1).isNumber()) {
        int row = argument(0).toInt32();
        int col = argument(1).toInt32();
        return m_document->read(row, col);
    } else if (n == 1 && argument(0).isString()) {
        return m_document->read(argument(0).toString());
    }
    return engine()->undefinedValue().toVariant();
}

bool XlsxDocument::selectSheet(const QString &name)
{
    return m_document->selectSheet(name);
}

void XlsxDocument::setDocumentProperty(const QString &key, const QString &property)
{
    m_document->setDocumentProperty(key, property);
}

QScriptValue XlsxDocument::worksheet(const QString &sheetName) const
{
    QXlsx::Worksheet* ws = dynamic_cast<QXlsx::Worksheet*>(m_document->sheet(sheetName));
    return ws ? engine()->toScriptValue(ws) : engine()->nullValue();
}

QStringList XlsxDocument::sheetNames() const
{
    return m_document->sheetNames();
}

