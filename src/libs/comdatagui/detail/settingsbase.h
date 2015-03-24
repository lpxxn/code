#ifndef SETTINGSBASE_H
#define SETTINGSBASE_H

#include <comguiutils/ioptionspage.h>

class SettingsBase : public GuiUtils::IOptionsPage
{
    Q_OBJECT
public:
    explicit SettingsBase(QObject *parent = 0);

protected slots:
    virtual void onActivate() {}
    virtual void onComboBoxTextChanged(const QString& text) { Q_UNUSED(text) }
    virtual void onComboBoxIndexChanged(int index) { Q_UNUSED(index) }
    virtual void onCheckBoxStateChanged(int state) { Q_UNUSED(state) }
    virtual void onButtonClicked() {}
    virtual void onSliderValueChanged(int value) { Q_UNUSED(value) }
    virtual void onSpinBoxValueChanged(int value) { Q_UNUSED(value) }
    virtual void updateUi() {}
    virtual void initConns() {}
};

#endif // SETTINGSBASE_H
