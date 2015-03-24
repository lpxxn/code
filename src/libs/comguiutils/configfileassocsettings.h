#ifndef CONFIGFILEASSOCSETTINGS_H
#define CONFIGFILEASSOCSETTINGS_H

#include <QPointer>

#include "ioptionspage.h"
#include "comguiutils_global.h"

class ConfigFileAssocDialog;

namespace GuiUtils
{

class COMGUIUTILS_EXPORT ConfigFileAssocSettings : public GuiUtils::IOptionsPage
{
    Q_OBJECT
public:
    ConfigFileAssocSettings();
    ~ConfigFileAssocSettings();

    QWidget* createPage(QWidget *parent);
    void apply();
    void finish();
    virtual bool matches(const QString &s) const;
    void prepare(const QString& fileName);

private slots:
    void onSearchStringsChanged();
    void onAddFileName(const QString& fileName);
    void onAddFileExt(const QString& fileExt);

private:
    void addAppItem(const QString& group, const QString& itemName);

private:
    ConfigFileAssocDialog* m_page;
    QString m_searchKeywords;

public:
    static QString FileNameGroup;
    static QString ExtensionGroup;

};
}
#endif // CONFIGFILEASSOCSETTINGS_H
