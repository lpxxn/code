#ifndef PIESETTINGS_H
#define PIESETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class PieSettings;
}

class COMDATAGUI_EXPORT PieSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit PieSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onSpinBoxValueChanged(int value);
    void onCheckBoxStateChanged(int state);

protected:
    void updateUi();
    void initConns();

private:
    Ui::PieSettings* m_ui = nullptr;
};

#endif // PIESETTINGS_H
