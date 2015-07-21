#include "styles.h"
#include "style.h"
#include "../shared.h"
#include "../opc/constants.h"
#include "../parts/numberingpart.h"

#include <QRegularExpression>
#include <QFile>

namespace Docx {

Styles::Styles(QDomDocument *dom) : m_dom(dom)
{
    m_paraStyles = new ParagraphStyles(m_dom);
    m_tableStyles = new TableStyles(m_dom);
    classifyStyles();
}

void Styles::classifyStyles()
{
    QDomNodeList eles = m_dom->elementsByTagName(QStringLiteral("w:style"));

    for (int eleIndex = 0, eleCount = eles.count(); eleIndex < eleCount; eleIndex++) {
        QDomElement currEle = eles.at(eleIndex).toElement();
        const QString strType = currEle.attribute(QStringLiteral("w:type"), "");
        if (strType == QStringLiteral("paragraph")) {
            m_paraStyles->addParagraphStyle(currEle);
        } else if (strType == QStringLiteral("table")) {
            m_tableStyles->addTableStyles(currEle);
        }
    }

    fillNoExistStyle();
}

QString Styles::titleHeadingId() const
{
    return m_paraStyles->titleStyle()->styleId();
}

QString Styles::headingStyleId(int headIndex) const
{
    QList<ParagraphStyle*> headStyles = m_paraStyles->headingStyles();
    return headStyles.at(--headIndex)->styleId();
}

QString Styles::talbeGridStyleId() const
{
    return m_tableStyles->tableGridStyle()->styleId();
}

void Styles::fillNoExistStyle()
{
    QDomDocument tempDoc = docXmlDom(Constants::TEMPLATE_STYLE_PATH);

    ParagraphStyles tempParaStyles(&tempDoc);
    TableStyles tempTableStyles(&tempDoc);
    QDomNodeList eles = tempDoc.elementsByTagName(QStringLiteral("w:style"));
    for (int eleIndex = 0, eleCount = eles.count(); eleIndex < eleCount; eleIndex++) {
        QDomElement currEle = eles.at(eleIndex).toElement();
        const QString strType = currEle.attribute(QStringLiteral("w:type"), "");
        if (strType == QStringLiteral("paragraph")) {
            tempParaStyles.addParagraphStyle(currEle);
        } else if (strType == QStringLiteral("table")) {
            tempTableStyles.addTableStyles(currEle);
        }
    }

    m_paraStyles->fillParagraphStyle(tempParaStyles);
    m_tableStyles->fillTableStyle(tempTableStyles);
}

void Styles::checkNumberingStyle(const NumberingPart *numPart)
{
    bool contentNumber = m_paraStyles->isContentListNumber();
    bool contentBullet = m_paraStyles->isContentListBullet();
    if (contentNumber && contentBullet)
        return;

    QDomDocument tempDoc = docXmlDom(Constants::TEMPLATE_LISTSTYLE_PATH);

    QDomElement temperRootEle = tempDoc.firstChildElement();
    QDomElement domFirstEle = m_dom->firstChildElement();

    // 第一个为 ListBullet
    QDomElement bulletEle = temperRootEle.childNodes().at(0).toElement();

    // 第二个为 ListNumber
    QDomElement numberEle = temperRootEle.childNodes().at(1).toElement();

    if (!contentBullet) {
        modifyNumId(bulletEle, 1, numPart);
        domFirstEle.appendChild(bulletEle);
    }

    if (!contentNumber) {
        modifyNumId(numberEle, 2, numPart);
        domFirstEle.appendChild(numberEle);
    }

}

Styles::~Styles()
{

}

void Styles::modifyNumId(QDomElement &listStyle, int listType, const NumberingPart *numPart)
{
    QDomElement pprEle = listStyle.elementsByTagName(QStringLiteral("w:pPr")).at(0).toElement();
    QDomElement numPrEle = pprEle.firstChildElement();
    QDomElement numIdEle = numPrEle.elementsByTagName(QStringLiteral("w:numId")).at(0).toElement();
    if (listType == 1)  // bullet
        numIdEle.setAttribute(QStringLiteral("w:val"), numPart->listBulletId());
    else if (listType == 2) // number
        numIdEle.setAttribute(QStringLiteral("w:val"), numPart->listNumberId());
}

}

