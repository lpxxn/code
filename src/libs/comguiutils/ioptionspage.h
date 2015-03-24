#ifndef IOPTIONSPAGE_H
#define IOPTIONSPAGE_H

#include <comutils/id.h>

#include <QIcon>
#include <QObject>

#include "comguiutils_global.h"

using namespace CommonUtils;

namespace GuiUtils {

class COMGUIUTILS_EXPORT IOptionsPage: public QObject
{
    Q_OBJECT
public:
    IOptionsPage(QObject *parent = nullptr) : QObject(parent) {}

    Id id() const { return m_id; }
    QString displayName() const { return m_displayName; }
    Id category() const { return m_category; }
    QString displayCategory() const { return m_displayCategory; }
    QIcon categoryIcon() const { return QIcon(m_categoryIcon); }

    virtual bool matches(const QString & /* searchKeyWord*/) const { return false; }
    virtual QWidget *createPage(QWidget *parent) = 0;
    virtual void apply() = 0;
    virtual void finish() = 0;

signals:
    /*!
     * \brief 在切换到当前页面时发射.
     */
    void activate();

protected:
    void setId(Id id) { m_id = id; }
    void setDisplayName(const QString &displayName) { m_displayName = displayName; }
    void setCategory(Id category) { m_category = category; }
    void setDisplayCategory(const QString &displayCategory) { m_displayCategory = displayCategory; }
    void setCategoryIcon(const QString &categoryIcon) { m_categoryIcon = categoryIcon; }

    Id m_id;
    Id m_category;
    QString m_displayName;
    QString m_displayCategory;
    QString m_categoryIcon;
};

}

#endif // IOPTIONSPAGE_H
