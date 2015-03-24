#ifndef STACKLINESETTINGS_H
#define STACKLINESETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class StackLineSettings;
}

class COMDATAGUI_EXPORT StackLineSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit StackLineSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();

private:
    Ui::StackLineSettings* m_ui = nullptr;
};

#endif // STACKLINESETTINGS_H
