#ifndef DOCXDOCUMENT_H
#define DOCXDOCUMENT_H

#include <QObject>

namespace Docx {
    class Document;
}

class QIODevice;

class DocxDocument : public QObject
{
    Q_OBJECT
public:
    DocxDocument(QObject *parent = nullptr);
    explicit DocxDocument(const QString& name, QObject *parent = nullptr);
    explicit DocxDocument(QIODevice* device, QObject *parent = nullptr);
    ~DocxDocument();

    Q_INVOKABLE void save(const QString& path);

private:
    Docx::Document* m_document;
};

#endif // DOCXDOCUMENT_H
