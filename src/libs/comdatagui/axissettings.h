#ifndef AXISSETTINGS_H
#define AXISSETTINGS_H

#include <QWidget>

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class AxisSettings;
}

class COMDATAGUI_EXPORT AxisSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT

public:
    explicit AxisSettings(QObject *parent = 0);
    ~AxisSettings();

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onColorChanged();
    void onComboBoxIndexChanged(int index);
    void onFontSelected(const QFont& f);
    void onCheckBoxStateChanged(int state);
    void onGroupBoxClicked(bool clicked);
    void onEditTextChanged(const QString& text);
    void onSpinBoxValueChanged(int value);

protected:
    void updateUi();
    void initConns();

private:
    void populateDisplayCombo();
    void populateTitleCombo();

private:
    Ui::AxisSettings *m_ui = nullptr;
};

#endif // AXISSETTINGS_H
