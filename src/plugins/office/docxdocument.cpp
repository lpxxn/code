#include "docxdocument.h"

#include <docx/document.h>

#include <QDebug>

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

DocxDocument::~DocxDocument()
{
    qDebug() << "delete DocxDocument.";
    delete m_document;
}

void DocxDocument::save(const QString &path)
{
    m_document->save(path);
}

