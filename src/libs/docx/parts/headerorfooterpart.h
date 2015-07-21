#ifndef HEADERSANDFOOTERS_H
#define HEADERSANDFOOTERS_H
#include "../opc/part.h"
#include "../enums/enumtext.h"

#include <QDomDocument>

namespace Docx {

class DocumentPart;
class Paragraph;

class HeaderOrFooterPart  : public Part
{
public:
    HeaderOrFooterPart(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package);

    QByteArray blob() const;
    virtual QDomElement element() const = 0;
    void checkDocument();
    ~HeaderOrFooterPart();

public:
    Paragraph* addParagraph(const QString &text = QString(), const QString &style = QString());
    void showNumber(PageNumberType numberType);

protected:
    QDomDocument *m_dom;    
    DocumentPart *m_docPart;
    Paragraph* m_currentPara = nullptr;
};

class HeaderPart : public HeaderOrFooterPart
{
public:
    HeaderPart(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package);
    QDomElement element() const;
    void afterUnmarshal();
    static HeaderPart *load(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package);
    ~HeaderPart();

private:

};

class FooterPart : public HeaderOrFooterPart
{
public:
    FooterPart(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package);
    QDomElement element() const;
    void afterUnmarshal();
    static FooterPart *load(const PackURI &partName, const QString &contentType, const QByteArray &blob, Package *package);
    ~FooterPart();

private:

};

}
#endif // HEADERSANDFOOTERS_H
