#ifndef LICENSEMANAGERGUI_H
#define LICENSEMANAGERGUI_H

#include "comguiutils_global.h"

#include <comutils/licensemanager.h>
#include <comutils/id.h>

namespace GuiUtils {

class COMGUIUTILS_EXPORT LicenseManagerGui : public CommonUtils::LicenseManager
{
    Q_OBJECT
public:
    static LicenseManagerGui* instance();
    bool verifyLicense(const char* featureName, QWidget* parent = nullptr);
    void licenseObtain(CommonUtils::Id app);

public slots:
    void licenseInfor();

private slots:
    void reConnect(const QString &feature, int pass, int total_attempts, int interval);
    void reConnectDone(const QString &feature, int tries);
    void exitCall(const QString &feature);

private:
    explicit LicenseManagerGui(QObject *parent = nullptr);

private:
    static LicenseManagerGui *st_guiInstance;
};

}

#endif // LICENSEMANAGERGUI_H
