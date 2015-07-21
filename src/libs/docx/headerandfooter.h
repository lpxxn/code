#ifndef HEADERANDFOOTER_H
#define HEADERANDFOOTER_H
#include "docx_global.h"
#include "./enums/enumtext.h"

#include <QList>
#include <QDomDocument>

namespace Docx {

class DocumentPart;
class HeaderPart;
class FooterPart;
class Package;
class Paragraph;



class DOCX_EXPORT Header
{
public:
    Header(DocumentPart *docPart, Package *package);
    Paragraph* addParagraph(const QString &text = QString(), const QString &style = QString());
    void showNumber(PageNumberType numberType);
    ~Header();

private:
    HeaderPart *header() const;
    void setHeader(HeaderPart *header);

private:
    HeaderPart *m_header;
    DocumentPart *m_docPart;
    Package *m_package;

    friend class Section;
};

class DOCX_EXPORT Footer
{
public:
    Footer(DocumentPart *docPart, Package *package);
    Paragraph* addParagraph(const QString &text = QString(), const QString &style = QString());
    void showNumber(PageNumberType numberType);
    ~Footer();

private:
    FooterPart *footer() const;
    void setFooter(FooterPart *footer);

private:
    FooterPart *m_footer;
    DocumentPart *m_docPart;
    Package *m_package;

    friend class Section;
};
}


#endif // HEADERANDFOOTER_H
