#ifndef WATERFALLSETTINGS_H
#define WATERFALLSETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class WaterfallSettings;
}

class COMDATAGUI_EXPORT WaterfallSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit WaterfallSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();

private:
    Ui::WaterfallSettings* m_ui = nullptr;
};

#endif // WATERFALLSETTINGS_H
