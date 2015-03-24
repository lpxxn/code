#ifndef ERRORBARSETTINGS_H
#define ERRORBARSETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class ErrorBarSettings;
}

class COMDATAGUI_EXPORT ErrorBarSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit ErrorBarSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

protected:
    virtual void updateUi();
    virtual void initConns();

protected slots:
    virtual void onComboBoxIndexChanged(int index);
    virtual void onCheckBoxStateChanged(int state);
    virtual void onSliderValueChanged(int value);

private slots:
    void onActivate();
    void onColorChanged();

private:
    Ui::ErrorBarSettings* m_ui = nullptr;
};

#endif // ERRORBARSETTINGS_H
