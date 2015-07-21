#include "style.h"
#include "../shared.h"

#include <QRegularExpression>

namespace Docx {

const QString strStyleId(QStringLiteral("w:styleId"));

BaseStyle::BaseStyle(QDomDocument *dom, const QDomElement &ele)
    : m_dom(dom), m_ele(ele)
{
    m_styleId = m_ele.attribute(strStyleId);
    QDomNodeList nameEles = m_ele.elementsByTagName(QStringLiteral("w:name"));
    if (!nameEles.isEmpty()) {
        const QDomElement nameEle = nameEles.at(0).toElement();
        m_name = nameEle.attribute(QStringLiteral("w:val"));
    }
}

BaseStyle::~BaseStyle()
{

}



QString BaseStyle::styleId() const
{
    return m_styleId;
}

void BaseStyle::setStyleId(const QString &styleId)
{
    m_ele.setAttribute(strStyleId, styleId);
    m_styleId = styleId;
}

WD_STYLE_TYPE BaseStyle::type() const
{
    return m_type;
}

void BaseStyle::setType(const WD_STYLE_TYPE &type)
{
    m_type = type;
}

QString BaseStyle::name() const
{
    return m_name;
}

void BaseStyle::setName(const QString &name)
{
    QDomElement nameEle = addOrAssignElement(m_dom, &m_ele, QStringLiteral("w:name"));
    nameEle.setAttribute(QStringLiteral("w:val"), name);
    m_name = name;
}

QDomElement BaseStyle::element() const
{
    return m_ele;
}



// 段落样式

ParagraphStyle::ParagraphStyle(QDomDocument *dom, const QDomElement &ele)
    : BaseStyle(dom, ele)
{
    m_type = WD_STYLE_TYPE::PARAGRAPH;

    QDomNodeList baseOnEles = m_ele.elementsByTagName(QStringLiteral("w:basedOn"));
    if (!baseOnEles.isEmpty()) {
        const QDomElement nameEle = baseOnEles.at(0).toElement();
        m_basedOn = nameEle.attribute(QStringLiteral("w:val"));
    }
}

ParagraphStyle::~ParagraphStyle()
{

}

QString ParagraphStyle::basedOn() const
{
    return m_basedOn;
}

void ParagraphStyle::setBasedOn(const QString &basedOn)
{
    QDomElement baseOnEle = addOrAssignElement(m_dom, &m_ele, QStringLiteral("w:basedOn"));
    baseOnEle.setAttribute(QStringLiteral("w:val"), basedOn);
    m_basedOn = basedOn;
}


ParagraphStyles::ParagraphStyles(QDomDocument *dom)
    : m_dom(dom)
{
    
}

void ParagraphStyles::addParagraphStyle(const QDomElement &ele)
{
    const QRegularExpression regHeading(QStringLiteral("heading\\s\\d"));
    ParagraphStyle *newParaStyle = new ParagraphStyle(m_dom, ele);
    const QString strname = newParaStyle->name();
    const QRegularExpressionMatch pos = regHeading.match(strname.toLower());
    if (strname == QStringLiteral("Normal")) {
        m_normal = newParaStyle;
    } else if (strname == QStringLiteral("Title")) {
        m_title = newParaStyle;
    }else if (pos.hasMatch()) {
        m_headingStyles.append(newParaStyle);
    } else {
        m_paraStyles.append(newParaStyle);
    }
}

/*!
 * \brief 填充 没有Heading的样式
 */
void ParagraphStyles::fillParagraphStyle(const ParagraphStyles &tempstyles)
{    
    // 赋值
    if (!m_normal && tempstyles.m_normal) {
        m_dom->firstChildElement().appendChild(tempstyles.m_normal->element());
        m_normal = new ParagraphStyle(m_dom, tempstyles.m_normal->element());
    }

    if (!m_title && tempstyles.m_title) {
        m_dom->firstChildElement().appendChild(tempstyles.m_title->element());
        m_title = new ParagraphStyle(m_dom, tempstyles.m_title->element());
    }
    const int headCount = tempstyles.m_headingStyles.count();

    if (m_headingStyles.count() == headCount)
        return;

    int headIndex = m_headingStyles.count();
    QString strId;
    if (!m_headingStyles.isEmpty()) {
        ParagraphStyle *lastPara = m_headingStyles.last();
        strId = lastPara->styleId();
    } else {
        strId = QStringLiteral("Heading");
    }

    for (; headIndex < headCount; headIndex++) {
        QString currId(strId);
        currId.append(QString::number(headIndex));
        ParagraphStyle *currHeadStyle = tempstyles.m_headingStyles.at(headIndex);
        currHeadStyle->setStyleId(currId);
        QDomElement newHeadEle = currHeadStyle->element();
        m_dom->firstChildElement().appendChild(newHeadEle);
        ParagraphStyle *newPara = new ParagraphStyle(m_dom, newHeadEle);
        newPara->setBasedOn(m_normal->styleId());
        m_headingStyles.append(newPara);
    }
}

ParagraphStyle *ParagraphStyles::normalStyle() const
{
    return m_normal;
}

ParagraphStyle *ParagraphStyles::titleStyle() const
{
    return m_title;
}

QList<ParagraphStyle *> ParagraphStyles::headingStyles() const
{
    return m_headingStyles;
}

QList<ParagraphStyle *> ParagraphStyles::otherParaStyles() const
{
    return m_paraStyles;
}

bool ParagraphStyles::isContentListNumber() const
{
    for (const ParagraphStyle *p : m_paraStyles) {
        if (p->styleId() == QStringLiteral("ListNumber"))
            return true;
    }
    return false;
}

bool ParagraphStyles::isContentListBullet() const
{
    for (const ParagraphStyle *p : m_paraStyles) {
        if (p->styleId() == QStringLiteral("ListBullet"))
            return true;
    }
    return false;
}

ParagraphStyles::~ParagraphStyles()
{
    qDeleteAll(m_paraStyles);
    qDeleteAll(m_headingStyles);
    if (!m_title)
        delete m_title;

    if (!m_normal)
        delete m_normal;
}

// Table 表 样式
TableStyle::TableStyle(QDomDocument *dom, const QDomElement &ele)
    : BaseStyle(dom, ele)
{
    QDomNodeList baseOnEles = m_ele.elementsByTagName(QStringLiteral("w:basedOn"));
    if (!baseOnEles.isEmpty()) {
        const QDomElement nameEle = baseOnEles.at(0).toElement();
        m_basedOn = nameEle.attribute(QStringLiteral("w:val"));
    }
}

TableStyle::~TableStyle()
{

}

QString TableStyle::basedOn() const
{
    return m_basedOn;
}

void TableStyle::setBasedOn(const QString &basedOn)
{
    QDomElement baseOnEle = addOrAssignElement(m_dom, &m_ele, QStringLiteral("w:basedOn"));
    baseOnEle.setAttribute(QStringLiteral("w:val"), basedOn);
    m_basedOn = basedOn;
}

TableStyles::TableStyles(QDomDocument *dom)
    : m_dom(dom)
{

}

void TableStyles::addTableStyles(const QDomElement &ele)
{
    //
    TableStyle *tableStyle = new TableStyle(m_dom, ele);
    const QString strName = tableStyle->name();
    if (strName == QStringLiteral("Normal Table")) {
        m_normal = tableStyle;
    } else if (strName == QStringLiteral("Table Grid")) {
        m_tableGrid = tableStyle;
    } else {
        m_tableStyles.append(tableStyle);
    }
}

void TableStyles::fillTableStyle(const TableStyles &tempstyles)
{
    if (!m_normal && tempstyles.m_normal) {
        m_dom->firstChildElement().appendChild(tempstyles.m_normal->element());
        m_normal = new TableStyle(m_dom, tempstyles.m_normal->element());
    } else if (!m_tableGrid && tempstyles.m_tableGrid) {
        m_dom->firstChildElement().appendChild(tempstyles.m_tableGrid->element());
        m_tableGrid = new TableStyle(m_dom, tempstyles.m_tableGrid->element());
        m_tableGrid->setBasedOn(m_normal->styleId());
    }
}

TableStyle *TableStyles::tableGridStyle() const
{
    return m_tableGrid;
}

TableStyles::~TableStyles()
{
    qDeleteAll(m_tableStyles);
    if (!m_normal) delete m_normal;

    if (!m_tableGrid) delete m_tableGrid;
}


// 列表样式
NumberingStyle::NumberingStyle(QDomDocument *dom, const QDomElement &ele)
    : BaseStyle(dom, ele)
{
    QDomNodeList baseOnEles = m_ele.elementsByTagName(QStringLiteral("w:basedOn"));
    if (!baseOnEles.isEmpty()) {
        const QDomElement nameEle = baseOnEles.at(0).toElement();
        m_basedOn = nameEle.attribute(QStringLiteral("w:val"));
    }
}

NumberingStyle::~NumberingStyle()
{

}
QString NumberingStyle::basedOn() const
{
    return m_basedOn;
}

void NumberingStyle::setBasedOn(const QString &basedOn)
{
    QDomElement baseOnEle = addOrAssignElement(m_dom, &m_ele, QStringLiteral("w:basedOn"));
    baseOnEle.setAttribute(QStringLiteral("w:val"), basedOn);
    m_basedOn = basedOn;
}

}
