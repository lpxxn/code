#ifndef OPTIONVALUECOLLECT_H
#define OPTIONVALUECOLLECT_H

#include <QString>
#include <QVariant>

namespace CommonUtils {

class OptionValueCollect
{
public:
    typedef QPair<QString, QVariant> SettingParameter;

    OptionValueCollect();
    ~OptionValueCollect();
    void addSettingPage(const QString& category, const QString& page,
                        const QList<SettingParameter>& settingPage);
    QList<SettingParameter> settingPage(const QString& category, const QString& page);

private:
    class OptionValueCollectPrivate* d;
};

}

#endif // OPTIONVALUECOLLECT_H
