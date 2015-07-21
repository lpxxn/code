#ifndef STYLESPART_H
#define STYLESPART_H

#include "../docx_global.h"
#include "../opc/part.h"

#include <QPair>
#include <QDomDocument>
#include <QByteArray>

namespace Docx {

class Styles;
class NumberingPart;

class StylesPart : public Part
{
public:
    StylesPart(const PackURI &partName, const QString &contentType, const QByteArray &blob = QByteArray());
    QDomDocument * element() const;
    QByteArray blob() const;
    static StylesPart *load(const PackURI &partName, const QString &contentType, const QByteArray &blob = QByteArray());
    QString titleStyleId() const;
    QString headingStyleId(int headIndex) const;
    QString tableGridStyleId() const;
    void checkNumberingStyle(const NumberingPart *numPart);
    virtual ~StylesPart();

private:
    QDomDocument *m_dom;
    Styles *m_styles;
};

}

#endif // STYLESPART_H
