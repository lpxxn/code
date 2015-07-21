#ifndef SECTION_H
#define SECTION_H
#include "docx_global.h"

#include <QList>
#include <QDomDocument>

namespace Docx {

class DocumentPart;
class Header;
class Footer;
class Package;

class DOCX_EXPORT Section
{
public:
    Section(const QDomElement &ele, DocumentPart *docPart, Package *package);
    Header *defaultHeader();
    Footer *defaultFooter();
    void restartNum(bool restar = true);
    ~Section();

private:
    Header *createHeaderPart();
    Footer *createFooterPart();
    void setDefaultHeaderId(const QString &id);
    void setDefaultFooterId(const QString &id);

private:
    QDomElement m_ele;
    DocumentPart *m_docPart = nullptr;
    Header *m_header = nullptr;
    Footer *m_footer = nullptr;
    Package *m_package = nullptr;
};

class DOCX_EXPORT Sections
{
public:
    Sections(DocumentPart * docPart);
    Section *addSection(const QDomElement &ele);
    ~Sections();

private:
    QList<Section*> m_sections;
    DocumentPart *m_docPart = nullptr;
};

}
#endif // SECTION_H
