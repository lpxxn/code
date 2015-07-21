#include "numberingpart.h"
#include "../opc/constants.h"
#include "../shared.h"

#include <QDomDocument>
#include <QFile>

namespace Docx {

const QString strNum(QStringLiteral("w:num"));
const QString strabsNum(QStringLiteral("w:abstractNum"));
const QString strabsNumId(QStringLiteral("w:abstractNumId"));

NumberingPart::NumberingPart(const PackURI &partName, const QString &contentType, const QByteArray &blob)
    : Part(partName, contentType, blob)
{
    m_dom = new QDomDocument();
    m_dom->setContent(blob);
    checkStyle();
}

NumberingPart::~NumberingPart()
{

}

QByteArray NumberingPart::blob() const
{
    return m_dom->toByteArray();
}

void NumberingPart::afterUnmarshal()
{
}

NumberingPart *NumberingPart::load(const PackURI &partName, const QString &contentType, const QByteArray &blob)
{
    return new NumberingPart(partName, contentType, blob);
}

/*!
 * \brief 查检numbering是否完整
 */
void NumberingPart::checkStyle()
{
    QDomElement rootEle =  m_dom->firstChildElement();
    QDomNodeList numEles = rootEle.elementsByTagName(QStringLiteral("w:abstractNum"));
    const QString strMultType("w:multiLevelType");
    bool haveListNum(false), haveListBullet(false);
    int listNumberId, listBulletId;
    for (int indexNum = 0, numCount = numEles.count(); indexNum < numCount; indexNum++) {
        QDomElement currEle = numEles.at(indexNum).toElement();
        QString absNumId = currEle.attribute(QStringLiteral("w:abstractNumId"));
        QDomElement multiEle = currEle.elementsByTagName(strMultType).at(0).toElement();
        QString strmultiVal = multiEle.attribute(QStringLiteral("w:val"));
        if (strmultiVal != QStringLiteral("hybridMultilevel"))
            continue;

        // lvl
        QDomElement lvlEle = currEle.elementsByTagName(QStringLiteral("w:lvl")).at(0).toElement();
        QDomElement fmtEle = lvlEle.elementsByTagName(QStringLiteral("w:numFmt")).at(0).toElement();
        QString strFmtVal = fmtEle.attribute(QStringLiteral("w:val"));

        if (strFmtVal == QStringLiteral("decimal")) {
            haveListNum = true;
            listNumberId = absNumId.toInt();
            continue;
        }

        if (strFmtVal == QStringLiteral("bullet")) {
            haveListBullet = true;
            listBulletId = absNumId.toInt();
            continue;
        }
    }

    addNumberStyle(haveListNum, listNumberId, haveListBullet, listBulletId);
}

void NumberingPart::addNumberStyle(bool haveListNum, int absListNumId,  bool haveListBullet, int absBulletId)
{
    QDomDocument tempDoc = docXmlDom(Constants::TEMPLATE_NUMBERING_PATH);

    QDomElement tempRootEle = tempDoc.firstChildElement();
    // index 0 为 数字, index 1 为 bullet
    QDomNodeList absNums = tempRootEle.elementsByTagName(strabsNum);

    QDomElement domRootEle = m_dom->firstChildElement();
    QDomNodeList domAbsNums = domRootEle.elementsByTagName(strabsNum);
    int absDomIndex = domAbsNums.count();

    // w:num
    QDomNodeList domNums = domRootEle.elementsByTagName(strNum);
    int numIndex = domNums.count() + 1;

    QDomElement decimalEle =  absNums.at(0).toElement();
    QDomElement bulletEle =  absNums.at(1).toElement();

    if (!haveListNum) {

        decimalEle.setAttribute(strabsNumId, absDomIndex);
        QDomNodeList curabsNums = domRootEle.elementsByTagName(strabsNum);

        domRootEle.insertAfter(curabsNums.at(curabsNums.count() - 1), decimalEle);

        domRootEle.appendChild(addNumStyle(numIndex, absDomIndex));
        m_listNumberId = QString::number(numIndex);
        numIndex++;
        ++absDomIndex;
    } else {
        numberStyle(absListNumId, m_listNumberId, numIndex, domNums);
    }

    if (!haveListBullet) {

        bulletEle.setAttribute(strabsNumId, absDomIndex);
        QDomNodeList curabsNums = domRootEle.elementsByTagName(strabsNum);
        domRootEle.insertAfter(curabsNums.at(curabsNums.count() - 1), bulletEle);

        domRootEle.appendChild(addNumStyle(numIndex, absDomIndex));
        m_listBulletId = QString::number(numIndex);
        numIndex++;
    } else {
        numberStyle(absBulletId, m_listBulletId, numIndex, domNums);
    }

}

void NumberingPart::numberStyle(int absNumId, QString &listNumId, int numIndex, const QDomNodeList &domNums)
{
    for (int index = 0; index < numIndex; index++) {
        QDomElement currNumEle = domNums.at(index).toElement();
        QDomElement currChildEle = currNumEle.elementsByTagName(strabsNumId).at(0).toElement();
        int currlistNumberId = currChildEle.attribute(QStringLiteral("w:val")).toInt();
        if (currlistNumberId == absNumId) {
            listNumId = currNumEle.attribute(QStringLiteral("w:numId"));
            break;
        }
    }
}

QDomElement NumberingPart::addNumStyle(int numId, int absNumId)
{
    const QString strNumId(QStringLiteral("w:numId"));
    const QString strNumAbsId(QStringLiteral("w:abstractNumId"));

    QDomElement newNumEle = m_dom->createElement(strNum);
    newNumEle.setAttribute(strNumId, numId);
    QDomElement newabsEleId = m_dom->createElement(strNumAbsId);
    newabsEleId.setAttribute(QStringLiteral("w:val"), absNumId);
    newNumEle.appendChild(newabsEleId);

    return newNumEle;
}
QString NumberingPart::listBulletId() const
{
    return m_listBulletId;
}

void NumberingPart::setListBulletId(const QString &listBulletId)
{
    m_listBulletId = listBulletId;
}

QString NumberingPart::listNumberId() const
{
    return m_listNumberId;
}

void NumberingPart::setListNumberId(const QString &listNumberId)
{
    m_listNumberId = listNumberId;
}


}

