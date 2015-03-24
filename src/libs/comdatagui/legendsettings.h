#ifndef LEGENDSETTINGS_H
#define LEGENDSETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class LegendSettings;
}

class COMDATAGUI_EXPORT LegendSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit LegendSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onComboBoxCurrrentIndexChanged(int index);
    void onColorChanged();
    void onFontSelected(const QFont& f);
    void onGroupBoxClicked(bool clicked);
    void onSpinBoxValueChanged(int value);

protected:
    void initConns();
    void updateUi();

private:
    void populateModeCombo();
    void populateHAlignCombo();
    void populateVAlignCombo();
    void populateLegendPosCombo();

private:
    Ui::LegendSettings* m_ui = nullptr;
};

#endif // LEGENDSETTINGS_H
