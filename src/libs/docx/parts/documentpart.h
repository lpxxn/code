#ifndef DOCUMENTPART_H
#define DOCUMENTPART_H

#include "../docx_global.h"
#include "../opc/part.h"

#include <QPair>
#include <QDomDocument>
#include <QByteArray>
#include <QImage>

namespace Docx {

//class Body;
class CT_Default;
class Paragraph;
class Run;
class Table;
class InlineShapes;
class ImagePart;
class ImageParts;
class InlineShape;
class StylesPart;
class NumberingPart;
class Section;
class Sections;

class DocumentPart : public Part
{
public:
    DocumentPart(const QString &partName, const QString &contentType, const QByteArray &blob = QByteArray(), Package *package = nullptr);
    Paragraph* addParagraph(const QString &text, const QString &style = QStringLiteral(""));
    static DocumentPart *load(const PackURI &partName, const QString &contentType, const QByteArray &blob = QByteArray(), Package *package = nullptr);
    Table *addTable(int rows, int cols, const QString &style = QString::fromLatin1("TableGrid"));
    void afterUnmarshal();
    QDomDocument * element() const;
    QByteArray blob() const;
    QPair<ImagePart *, QString> getOrAddImagePart(const PackURI &imageDescriptor);
    QPair<ImagePart *, QString> getOrAddImagePart(const QImage &img);
    QPair<ImagePart *, QString> getOrAddImagePart(ImagePart *imagPart);
    QList<Paragraph*> paragraphs();
    QList<Table*> tables();
    StylesPart *stylePart();
    NumberingPart *numberingPart();
    int nextId();
    Section *addSection();
    Package *package();
    virtual ~DocumentPart();

private:
    void findAttributes(const QDomNodeList &eles, const QString &attr, QStringList *nums);
    QDomNode lastsectPr() const;
    void checkNumbering();

private:
    //Body *m_body;
    QDomDocument *m_dom;
    InlineShapes *m_inlineshapes;
    StylesPart *m_stylePart = nullptr;
    NumberingPart *m_numberingPart = nullptr;
    Sections *m_sections = nullptr;

    // add
    QList<Paragraph *> m_addParagraphs;
    QList<Table *> m_addtables;

    // load
    QList<Paragraph *> m_ps;
    QList<Table *> m_tables;

    friend class Paragraph;
    friend class Run;
    friend class InlineShapes;
    friend class Table;

};

class InlineShapes
{
public:
    InlineShapes(DocumentPart *part);
    ~InlineShapes();

    InlineShape *addPicture(const QString &imagePath, Run *run);
    InlineShape *addPicture(const QImage &img, Run *run);

private:
    InlineShape *addPicture(const QPair<ImagePart *, QString> &imagePartAndId, Run *run);

private:
    DocumentPart *m_part;
    QDomDocument *m_dom;
    friend class InlineShape;
};

}

#endif // DOCUMENTPART_H
