#include "stylespart.h"
#include "../styles/styles.h"


namespace Docx {

StylesPart::StylesPart(const PackURI &partName, const QString &contentType, const QByteArray &blob)
    : Part(partName, contentType, blob)
{
    m_dom = new QDomDocument();
    m_dom->setContent(blob);
    m_styles = new Styles(m_dom);
}

QDomDocument *StylesPart::element() const
{
    return m_dom;
}

QByteArray StylesPart::blob() const
{
    return m_dom->toByteArray();
}

StylesPart *StylesPart::load(const PackURI &partName, const QString &contentType, const QByteArray &blob)
{
    return new StylesPart(partName, contentType, blob);
}

QString StylesPart::titleStyleId() const
{
    return m_styles->titleHeadingId();
}

QString StylesPart::headingStyleId(int headIndex) const
{
    return m_styles->headingStyleId(headIndex);
}

QString StylesPart::tableGridStyleId() const
{
    return m_styles->talbeGridStyleId();
}

void StylesPart::checkNumberingStyle(const NumberingPart *numPart)
{
    m_styles->checkNumberingStyle(numPart);
}

StylesPart::~StylesPart()
{
    delete m_dom;
    delete m_styles;
}
}
