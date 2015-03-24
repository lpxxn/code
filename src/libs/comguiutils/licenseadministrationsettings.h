#ifndef LICENSEADMINISTRATIONSETTINGS_H
#define LICENSEADMINISTRATIONSETTINGS_H

#include "comguiutils_global.h"
#include "ioptionspage.h"

#include <QPointer>

namespace GuiUtils {

namespace Ui {
    class LicenseAdministrationPage;
}

class COMGUIUTILS_EXPORT LicenseAdministrationSettings : public IOptionsPage
{
    Q_OBJECT
public:
    LicenseAdministrationSettings();
    QWidget* createPage(QWidget *parent);
    void apply();
    void finish();
    virtual bool matches(const QString &s) const;

private slots:
    void addNewLicenseFile();
    void upLicFile();
    void downLicFile();
    void delLicFile();

private:
    Ui::LicenseAdministrationPage* m_page;
    QPointer<QWidget> m_widget;
    QString m_searchKeywords;
};

}



#endif // LICENSEADMINISTRATIONSETTINGS_H
