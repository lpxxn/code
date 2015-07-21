#ifndef STYLES_H
#define STYLES_H

#include <QString>
#include <QDomDocument>

namespace Docx {

class ParagraphStyles;
class TableStyles;
class NumberingPart;

class Styles
{
public:
    Styles(QDomDocument *dom);
    void classifyStyles();
    QString titleHeadingId() const;
    QString headingStyleId(int headIndex) const;
    QString talbeGridStyleId() const;
    void fillNoExistStyle();
    void checkNumberingStyle(const NumberingPart *numPart);
    ~Styles();

private:
    void modifyNumId(QDomElement &listStyle, int listType, const NumberingPart *numPart);

private:
    QDomDocument *m_dom;
    ParagraphStyles *m_paraStyles;
    TableStyles *m_tableStyles;

};
}
#endif // STYLES_H
