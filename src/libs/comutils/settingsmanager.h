#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "comutils_global.h"
#include "id.h"

#include <QObject>
#include <QHash>

class QSettings;

namespace CommonUtils {

class UTILS_EXPORT SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager* instance();

    ~SettingsManager();
    QSettings* localSettings(Id softId);
    QSettings* localSettings();
    QSettings* generalSettings() const;
    QString configPath() const;
    void saveSettings();
    void setBaseSoftId(Id softId) { m_baseSoftId = softId; }
    Id baseSoftId() const { return m_baseSoftId; }

signals:
    void saveSettingsRequested();
    void optionsDialogRequested();

private:
    SettingsManager(QObject *parent = nullptr);

private:
    QHash<Id, QSettings*> m_settings;
    Id  m_baseSoftId;
};

}


#endif // SETTINGSMANAGER_H
