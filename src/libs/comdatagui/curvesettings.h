#ifndef CURVESETTINGS_H
#define CURVESETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class CurveSettings;
}

class COMDATAGUI_EXPORT CurveSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit CurveSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onComboBoxTextChanged(const QString& text);
    void onComboBoxIndexChanged(int index);
    void onSliderValueChanged(int value);
    void onSpinBoxValueChanged(int value);
    void onColorChanged();
    void onCheckBoxStateChanged(int state);
    void onGroupBoxClicked(bool clicked);
    void onFontSelected(const QFont& f);

protected:
    void initConns();
    void updateUi();

private:
    void populateSymbolCombo();
    void populateSizeCombo();
    void populateLabelPosCombo();

private:
    Ui::CurveSettings *m_ui = nullptr;


};

#endif // CURVESETTINGS_H
