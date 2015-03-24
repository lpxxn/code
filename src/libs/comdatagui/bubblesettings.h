#ifndef BUBBLESETTINGS_H
#define BUBBLESETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class BubbleSettings;
}

class COMDATAGUI_EXPORT BubbleSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit BubbleSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onComboBoxTextChanged(const QString& text);
    void onSliderValueChanged(int value);

private:
    void updateUi();
    void initConns();

private:
    Ui::BubbleSettings *m_ui = nullptr;
};

#endif // BUBBLESETTINGS_H
