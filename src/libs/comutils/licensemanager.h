/*! \file
*   \brief License管理功能
*   \author 谭立方
*   \date 2011-2014
*   \version 2.0
*   \copyright 2011-2014 PERA Global
*
*   应用程序中所有模块公共使用的License管理功能
*/

#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H

#include "comutils_global.h"

#ifdef USELOG
#include <log4qt/logger.h>
#endif

#include <QtCore/qglobal.h>
#include <QObject>
#include <QString>

namespace CommonUtils {

class UTILS_EXPORT LicenseManager : public QObject
{
    Q_OBJECT
#ifdef USELOG
    LOG4QT_DECLARE_QCLASS_LOGGER
#endif
public:
    ~LicenseManager();

    static LicenseManager* instance();

#ifdef Q_OS_WIN
    void setPromptForFile(bool prompt);
    void setCheckBadDate(bool check);
#endif
    bool setDefaultLicensePath(const QString &path);

    bool isOK() const { return m_isOK; }
    QString error() const { return m_error; }
    QString featureVersion() const { return m_featureVersion; }
    bool tamperResistant();
    bool checkout(const QString &featureName, const QString &licenseVersion);
    void checkin(const QString &featureName) const;
    bool isDemo(const char *featureName);
    bool isAny(const char *featureName);
    int expireDays(const char *featureName) const;
    QStringList userList(const char *featureName) const;
    QString featureInfo(const char *feature, int type, int *status) const;
    QStringList licenseFiles() const;

    virtual bool verifyLicense(const char* featureName, QWidget* parent=nullptr);
    void setCoreFeatureName(const QString& featureName);
    const QString& coreFeatureName();

signals:
    void reConnect(const QString &feature, int pass, int total_attempts, int interval);
    void reConnectDone(const QString &feature, int tries, int total_attempts, int interval);
    void exitCall(const QString &feature);

protected:
    LicenseManager(QObject *parent = nullptr);

private:
    bool tamperResistantHelper(const QString &fileName, const QString &hashString);
    static int reConnect(const char *feature, int pass, int total_attempts, int interval);
    static int reConnectDone(const char *feature, int tries, int total_attempts, int interval);
    static void exitCall(const char *feature);
    void reConnectServer(const QString &feature, int pass, int total_attempts, int interval);
    void reConnectServerDone(const QString &feature, int tries, int total_attempts, int interval);
    void exitApplicationCall(const QString &feature);

protected:
    static LicenseManager *st_instance;
private:
    QString m_featureVersion;
    QString m_error;
    QString m_coreFeature;
    bool m_isOK;
};

} // namespace Utils


#endif // LICENSEMANAGER_H
