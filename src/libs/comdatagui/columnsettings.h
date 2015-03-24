#ifndef COLUMNSETTINGS_H
#define COLUMNSETTINGS_H

#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class ColumnSettings;
}

class COMDATAGUI_EXPORT ColumnSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit ColumnSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private slots:
    void onActivate();
    void onSliderValueChanged(int value);
    void onComboBoxIndexChanged(int index);
    void onFontSelected(const QFont& f);
    void onColorChanged();
    void onSpinBoxValueChanged(int value);
    void onGroupBoxClicked(bool check);

protected:
    void initConns();
    void updateUi();

private:
    void setBarChartFillPattern(int pattern);
    void setBarChartLabelPosition(int position);

private:
    Ui::ColumnSettings* m_ui = nullptr;

};

#endif // COLUMNSETTINGS_H
