#include "section.h"
#include "shared.h"
#include "package.h"
#include "./parts/headerorfooterpart.h"
#include "./parts/documentpart.h"
#include "./opc/constants.h"
#include "./opc/rel.h"
#include "./headerandfooter.h"

namespace Docx {


Section::Section(const QDomElement &ele, DocumentPart *docPart, Package *package)
    : m_ele(ele), m_docPart(docPart), m_package(package)
{

}

Header *Section::defaultHeader()
{
    if (!m_header) {
        m_header = createHeaderPart();
    }
    return m_header;
}

Footer *Section::defaultFooter()
{
    if (!m_footer) {
        m_footer = createFooterPart();
    }
    return m_footer;
}

void Section::restartNum(bool restar)
{
    QDomElement headerEle = addOrAssignElement(m_docPart->element(), &m_ele, QStringLiteral("w:pgNumType"));
    QString strNum("1");
    if (!restar)
        strNum = QStringLiteral("0");

    headerEle.setAttribute(QStringLiteral("w:start"), strNum);
}

Section::~Section()
{

}

Header *Section::createHeaderPart()
{
    HeaderAndFooterParts *hafs = m_package->headerAndFooterParts();
    const int headerCount = hafs->headers().count();
    const QString relId = m_docPart->rels()->nextrId();

    const QString strHeader = R"~(<w:hdr xmlns:wpc="http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas"
            xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" xmlns:o="urn:schemas-microsoft-com:office:office"
            xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math"
            xmlns:v="urn:schemas-microsoft-com:vml" xmlns:wp14="http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing"
            xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing" xmlns:w10="urn:schemas-microsoft-com:office:word"
            xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main" xmlns:w14="http://schemas.microsoft.com/office/word/2010/wordml"
            xmlns:wpg="http://schemas.microsoft.com/office/word/2010/wordprocessingGroup" xmlns:wpi="http://schemas.microsoft.com/office/word/2010/wordprocessingInk"
            xmlns:wne="http://schemas.microsoft.com/office/word/2006/wordml" xmlns:wps="http://schemas.microsoft.com/office/word/2010/wordprocessingShape"
            mc:Ignorable="w14 wp14">

            </w:p>
            </w:hdr>)~";

    QDomDocument tempDoc;
    tempDoc.setContent(strHeader);
    const PackURI strHeaderName(QString("word/header%1.xml").arg(headerCount));

    HeaderPart *headerPart = HeaderPart::load(strHeaderName, Constants::WML_HEADER, tempDoc.toByteArray(), m_package);
    headerPart->afterUnmarshal();
    m_docPart->loadRel(Constants::HEADER_REL_TYPE, strHeaderName.fileName(), headerPart, relId, false);
    hafs->appendHeader(headerPart);
    setDefaultHeaderId(relId);

    Header *header = new Header(m_docPart, m_package);
    header->setHeader(headerPart);
    return header;
}

Footer *Section::createFooterPart()
{
    HeaderAndFooterParts *hafs = m_package->headerAndFooterParts();
    const int footerCount = hafs->footers().count();
    const QString relId = m_docPart->rels()->nextrId();

    const QString strfooter = R"~(<w:ftr xmlns:wpc="http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas"
            xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" xmlns:o="urn:schemas-microsoft-com:office:office"
            xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math"
            xmlns:v="urn:schemas-microsoft-com:vml" xmlns:wp14="http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing"
            xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing" xmlns:w10="urn:schemas-microsoft-com:office:word"
            xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main" xmlns:w14="http://schemas.microsoft.com/office/word/2010/wordml"
            xmlns:wpg="http://schemas.microsoft.com/office/word/2010/wordprocessingGroup" xmlns:wpi="http://schemas.microsoft.com/office/word/2010/wordprocessingInk"
            xmlns:wne="http://schemas.microsoft.com/office/word/2006/wordml" xmlns:wps="http://schemas.microsoft.com/office/word/2010/wordprocessingShape"
            mc:Ignorable="w14 wp14">

            </w:p>
        </w:ftr>)~";

    QDomDocument tempDoc;
    tempDoc.setContent(strfooter);
    const PackURI strFooterName(QString("word/footer%1.xml").arg(footerCount));
    FooterPart *footerPart = FooterPart::load(strFooterName, Constants::WML_FOOTER, tempDoc.toByteArray(), m_package);
    footerPart->afterUnmarshal();
    m_docPart->loadRel(Constants::FOOTER_REL_TYPE, strFooterName.fileName(), footerPart, relId, false);
    hafs->appendFooter(footerPart);
    setDefaultFooterId(relId);

    Footer *footer = new Footer(m_docPart, m_package);
    footer->setFooter(footerPart);
    return footer;
}

/*!
 * \brief  关联相应的页眉id
 * \param id
 */
void Section::setDefaultHeaderId(const QString &id)
{
    QDomElement headerEle = addOrAssignElement(m_docPart->element(), &m_ele, QStringLiteral("w:headerReference"), true);
    headerEle.setAttribute(QStringLiteral("w:type"), QStringLiteral("default"));
    headerEle.setAttribute(QStringLiteral("r:id"), id);
}

/*!
 * \brief 关联相应的页角id
 * \param id
 */
void Section::setDefaultFooterId(const QString &id)
{
    QDomElement headerEle = addOrAssignElement(m_docPart->element(), &m_ele, QStringLiteral("w:footerReference"), true);
    headerEle.setAttribute(QStringLiteral("w:type"), QStringLiteral("default"));
    headerEle.setAttribute(QStringLiteral("r:id"), id);
}

// Sections
Sections::Sections(DocumentPart *docPart) : m_docPart(docPart)
{

}

Section *Sections::addSection(const QDomElement &ele)
{
    Section *sec = new Section(ele, m_docPart, m_docPart->package());
    m_sections.append(sec);
    return sec;
}

Sections::~Sections()
{
    qDeleteAll(m_sections);
    m_sections.clear();
}

}
