#ifndef GRAPHWINDOWSETTINGS_H
#define GRAPHWINDOWSETTINGS_H

#include <QWidget>

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class GraphWindowSettings;
}

class COMDATAGUI_EXPORT GraphWindowSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT

public:
    explicit GraphWindowSettings(QObject *parent = 0);
    ~GraphWindowSettings();

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

protected:
    void initConns();
    void updateUi();

private slots:
    void onActivate();
    void onComboBoxCurrentIndexChanged(int index);
    void onCheckBoxStateChanged(int state);
    void onColorChanged();
    void onSliderValueChanged(int value);
    void updateUiByGradientFillMode(int mode);

private:
    Ui::GraphWindowSettings *m_ui = nullptr;
};

#endif // GRAPHWINDOWSETTINGS_H
