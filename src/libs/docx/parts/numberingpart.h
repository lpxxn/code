#ifndef NUMBERINGPART_H
#define NUMBERINGPART_H
#include "../docx_global.h"
#include "../opc/part.h"

#include <QPair>
#include <QDomDocument>
#include <QByteArray>

namespace Docx {

class NumberingPart : public Part
{
public:
    NumberingPart(const PackURI &partName, const QString &contentType, const QByteArray &blob = QByteArray());
    ~NumberingPart();

    QByteArray blob() const;
    void afterUnmarshal();
    static NumberingPart *load(const PackURI &partName, const QString &contentType, const QByteArray &blob = QByteArray());

    void checkStyle();

    QString listNumberId() const;
    void setListNumberId(const QString &listNumberId);

    QString listBulletId() const;
    void setListBulletId(const QString &listBulletId);

private:
    void addNumberStyle(bool haveListNum, int absListNumId, bool haveListBullet, int absBulletId);
    void numberStyle(int absNumId, QString &listNumId, int numIndex, const QDomNodeList &domNums);
    QDomElement addNumStyle(int numId, int absNumId);

private:
    QDomDocument *m_dom;
    QString m_listNumberId;
    QString m_listBulletId;
};

}

#endif // NUMBERINGPART_H
