#ifndef LAYERSETTINGS_H
#define LAYERSETTINGS_H

#include <QWidget>

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class LayerSettings;
}

class COMDATAGUI_EXPORT LayerSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT

public:
    explicit LayerSettings(QObject *parent = 0);
    ~LayerSettings();

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onComboBoxIndexChanged(int index);
    void onCheckBoxStateChanged(int state);
    void onColorChanged();
    void onSliderValueChanged(int value);
    void onSpinBoxValueChanged(int value);
    void onTextChanged(const QString& text);
    void onFontSelected(const QFont& f);
    void updateUiByGradientFillMode(int mode);

protected:
    void initConns();
    void updateUi();

private:
    Ui::LayerSettings *m_ui = nullptr;
};

#endif // LAYERSETTINGS_H
