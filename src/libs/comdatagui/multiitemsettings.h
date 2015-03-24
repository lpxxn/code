#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include "comdatagui_global.h"
#include "detail/settingsbase.h"
#include "vizgraphbasesettings.h"
#include "comdatagui_global.h"

namespace Ui
{
    class GeneralSettings;
}

class QTableWidget;

class COMDATAGUI_EXPORT MultiItemSettings : public SettingsBase, public VizGraphBaseSettings
{
    Q_OBJECT
public:
    explicit MultiItemSettings(QObject *parent = 0);

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

protected slots:
    void onActivate();
    void onCheckBoxStateChanged(int state);
    void onComboBoxIndexChanged(int index);

    void onLineColorTableEntered();
    void onPatternColorTableEntered();
    void onPenStyleTableEntered();
    void onBubbleStyleTableEntered();
    void onLineColorComboIndexChanged(int index);
    void onPatternColorComboIndexChanged(int index);
    void onLineStyleComboIndexChanged(int index);

protected:
    void initConns();
    void updateUi();

private:
    void populateLineColorTable();
    void populatePatternColorTable();
    void populateLineStyleTable();
    void populateBubbleStyleTable();
    void populateColorTable(QTableWidget* table, const char* slot = nullptr);
    void updateLineStyles();
    void updateLineColors();
    void updatePatternColors();
    void updateBubbleStyles();

private:
    Ui::GeneralSettings* m_ui = nullptr;
};

#endif // GENERALSETTINGS_H
