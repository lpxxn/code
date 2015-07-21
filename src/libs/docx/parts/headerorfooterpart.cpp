#include "headerorfooterpart.h"
#include "documentpart.h"
#include "../package.h"
#include "../text.h"

namespace Docx {

HeaderOrFooterPart::HeaderOrFooterPart(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package)
    : Part(partName, contentType, QByteArray(), package)
{
    m_dom = new QDomDocument();
    m_dom->setContent(blob);
}

QByteArray HeaderOrFooterPart::blob() const
{
    return m_dom->toByteArray();
}

void HeaderOrFooterPart::checkDocument()
{
    m_docPart = m_package->mainDocument();
}

HeaderOrFooterPart::~HeaderOrFooterPart()
{

}

Paragraph *HeaderOrFooterPart::addParagraph(const QString &text, const QString &style)
{
    QDomElement pEle = m_dom->createElement(QStringLiteral("w:p"));

    m_currentPara = new Paragraph(m_docPart, pEle);

    if (!text.isEmpty())
        m_currentPara->addRun(text, style);

    QDomElement ele = element();
    ele.appendChild(pEle);
    return m_currentPara;
}

void HeaderOrFooterPart::showNumber(PageNumberType numberType)
{
    if (!m_currentPara)
        addParagraph();

    // begin
    Run *run = m_currentPara->addRun();
    QDomElement runEle = run->element();
    QDomElement rldEle = addOrAssignElement(m_dom, &runEle, QStringLiteral("w:fldChar"));
    rldEle.setAttribute(QStringLiteral("w:fldCharType"), QStringLiteral("begin"));

    // instrText
    run = m_currentPara->addRun();
    runEle = run->element();
    const QString strInstr("w:instrText");
    rldEle = addOrAssignElement(m_dom, &runEle, strInstr);

    QDomText rinstrEle = m_dom->createTextNode(strInstr);
    rldEle.appendChild(rinstrEle);

    switch (numberType) {
    case PageNumberType::NormalNum:
        rinstrEle.setData(R"~(PAGE  \* MERGEFORMAT)~");
        break;
    case PageNumberType::RomanNum:
        rinstrEle.setData(R"~(PAGE  \* ROMAN  \* MERGEFORMAT)~");
        break;
    default:
        break;
    }


    // separate
    run = m_currentPara->addRun();
    runEle = run->element();
    rldEle = addOrAssignElement(m_dom, &runEle, QStringLiteral("w:fldChar"));
    rldEle.setAttribute(QStringLiteral("w:fldCharType"), QStringLiteral("separate"));

    // end
    run = m_currentPara->addRun();
    runEle = run->element();
    rldEle = addOrAssignElement(m_dom, &runEle, QStringLiteral("w:fldChar"));
    rldEle.setAttribute(QStringLiteral("w:fldCharType"), QStringLiteral("end"));
}


HeaderPart::HeaderPart(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package)
    : HeaderOrFooterPart(partName, contentType, blob, package)
{

}

QDomElement HeaderPart::element() const
{
    return m_dom->firstChildElement();
}

void HeaderPart::afterUnmarshal()
{
    checkDocument();
}

HeaderPart *HeaderPart::load(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package)
{
    return new HeaderPart(partName, contentType, blob, package);
}

HeaderPart::~HeaderPart()
{

}

FooterPart::FooterPart(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package)
    : HeaderOrFooterPart(partName, contentType, blob, package)
{

}

QDomElement FooterPart::element() const
{
    return m_dom->firstChildElement();
}

void FooterPart::afterUnmarshal()
{
    checkDocument();
}

FooterPart *FooterPart::load(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package)
{
    return new FooterPart(partName, contentType, blob, package);
}

FooterPart::~FooterPart()
{

}

}



