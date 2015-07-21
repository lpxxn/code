#include "headerandfooter.h"
#include "./parts/headerorfooterpart.h"

namespace Docx {

Header::Header(DocumentPart *docPart, Package *package)
    : m_docPart(docPart), m_package(package)
{

}

Paragraph *Header::addParagraph(const QString &text, const QString &style)
{
    return m_header->addParagraph(text, style);
}

void Header::showNumber(PageNumberType numberType)
{
    m_header->showNumber(numberType);
}

Header::~Header()
{

}

HeaderPart *Header::header() const
{
    return m_header;
}

void Header::setHeader(HeaderPart *header)
{
    m_header = header;
}



Footer::Footer(DocumentPart *docPart, Package *package)
    : m_docPart(docPart), m_package(package)
{

}

Paragraph *Footer::addParagraph(const QString &text, const QString &style)
{
    return m_footer->addParagraph(text, style);
}

void Footer::showNumber(PageNumberType numberType)
{
    m_footer->showNumber(numberType);
}

Footer::~Footer()
{

}
FooterPart *Footer::footer() const
{
    return m_footer;
}

void Footer::setFooter(FooterPart *footer)
{
    m_footer = footer;
}



}

