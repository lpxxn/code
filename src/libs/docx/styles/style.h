#ifndef STYLE_H
#define STYLE_H

#include "../enums/enumtext.h"

#include <QString>
#include <QDomElement>
#include <QList>

namespace Docx {

class BaseStyle : public QDomElement
{
public:
    BaseStyle(QDomDocument *dom, const QDomElement &ele);
    virtual ~BaseStyle();    

    QString styleId() const;
    void setStyleId(const QString &styleId);

    WD_STYLE_TYPE type() const;
    void setType(const WD_STYLE_TYPE &type);

    QString name() const;
    void setName(const QString &name);

    QDomElement element() const;

protected:
    QDomDocument *m_dom;
    QDomElement m_ele;
    QString m_styleId;
    WD_STYLE_TYPE m_type;
    QString m_name;
};

class ParagraphStyle : public BaseStyle
{
public:
    ParagraphStyle(QDomDocument *dom, const QDomElement &ele);
    virtual ~ParagraphStyle();

    QString basedOn() const;
    void setBasedOn(const QString &basedOn);

private:
    QString m_basedOn;
};


class ParagraphStyles
{
public:
    ParagraphStyles(QDomDocument *dom);
    void addParagraphStyle(const QDomElement &ele);
    void fillParagraphStyle(const ParagraphStyles &tempstyles);
    ParagraphStyle *normalStyle() const;
    ParagraphStyle *titleStyle() const;
    QList<ParagraphStyle*> headingStyles() const;
    QList<ParagraphStyle*> otherParaStyles() const;
    bool isContentListNumber() const;
    bool isContentListBullet() const;
    ~ParagraphStyles();

private:
    QDomDocument *m_dom;
    ParagraphStyle *m_normal = nullptr;
    ParagraphStyle *m_title = nullptr;
    QList<ParagraphStyle*> m_headingStyles;
    QList<ParagraphStyle*> m_paraStyles;
};


class TableStyle : public BaseStyle
{
public:
    TableStyle(QDomDocument *dom, const QDomElement &ele);
    virtual ~TableStyle();

    QString basedOn() const;
    void setBasedOn(const QString &basedOn);

private:
    QString m_basedOn;
};

class TableStyles
{
public:
    TableStyles(QDomDocument *dom);
    void addTableStyles(const QDomElement &ele);
    void fillTableStyle(const TableStyles &tempstyles);
    TableStyle *tableGridStyle() const;
    ~TableStyles();

private:
    QDomDocument *m_dom;
    TableStyle *m_normal = nullptr;
    TableStyle *m_tableGrid = nullptr;
    QList<TableStyle*> m_tableStyles;
};

class NumberingStyle : public BaseStyle
{
public:
    NumberingStyle(QDomDocument *dom, const QDomElement &ele);
    ~NumberingStyle();

    QString basedOn() const;
    void setBasedOn(const QString &basedOn);

private:
    QString m_basedOn;
};

}
#endif // STYLE_H
