#ifndef DOCXDOCUMENT_H
#define DOCXDOCUMENT_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

namespace Docx {
    class Document;
}

class QIODevice;

class DocxDocument : public QObject, public QScriptable
{
    Q_OBJECT     
    Q_ENUMS(ParagraphType)
public:
    enum ParagraphType {
        ListBullet,
        ListNumber,
        NoBullet
    };

    DocxDocument(QObject *parent = nullptr);
    explicit DocxDocument(const QString& name, QObject *parent = nullptr);
    explicit DocxDocument(QIODevice* device, QObject *parent = nullptr);

    Q_INVOKABLE QScriptValue addParagraph(const QString &text = QString(), ParagraphType type = ParagraphType::NoBullet);
    Q_INVOKABLE QScriptValue addHeading(const QString &text = QString(), int level = 1);
    Q_INVOKABLE QScriptValue addTable(int rows, int cols, const QString &style = QStringLiteral("TableGrid"));
    Q_INVOKABLE QScriptValue addPicture(const QString &path, int width = 0, int height = 0);
    Q_INVOKABLE QScriptValue addPageBreak();
    Q_INVOKABLE QScriptValue paragraphs();
    Q_INVOKABLE QScriptValue tables();
    Q_INVOKABLE QScriptValue addSection();
    ~DocxDocument();

    Q_INVOKABLE void save(const QString& path);

private:
    Docx::Document* m_document;
};

#endif // DOCXDOCUMENT_H
