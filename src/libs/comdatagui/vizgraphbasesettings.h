#ifndef VIZGRAPHBASESETTINGS_H
#define VIZGRAPHBASESETTINGS_H

#include <QMap>
#include <QComboBox>

#include "comdatagui_global.h"

namespace ComDataGui
{
    class CurveGraph;
    class BarChartGraph;
    class Pie2DGraph;
    class BubbleGraph;
    class GraphProperty;
}

class QComboBox;
class QListWidget;
class QWidget;
class QStackedWidget;

class VizGraphBaseSettings
{
public:
    VizGraphBaseSettings();

    static void apply();
    static void applyToBubble();
    static void applyToCurve();
    static void applyToBarChart();
    static void applyToPie();

    static ComDataGui::CurveGraph* curve();
    static ComDataGui::BarChartGraph* barChart();
    static ComDataGui::Pie2DGraph* pie();
    static ComDataGui::BubbleGraph* bubble();

    static void populateColorComboBox(QComboBox* cb);
    static void populateFontComboBox(QComboBox* cb);
    static void populateFontSizeComboBox(QComboBox* cb);
    static void populateColorList(QListWidget* lw);

    void setComboBoxIndexByValue(QComboBox* cb, int value);

    template <typename T>
    void setComboBoxIndexByData(QComboBox* cb, T value)
    {
        for (int i = 0; i < cb->count(); ++i) {
            if (qvariant_cast<T>(cb->itemData(i)) == value) {
                cb->setCurrentIndex(i);
                return;
            }
        }
    }

    static void init();
    static void finalize();
    static void release();
    static void showCurve();
    static void showPie();
    static void showBarChart();
    static void showBubble();

protected:
    static void readSettings();
    static void writeSettings();
    static void makeDirty();

    static ComDataGui::CurveGraph* sCurve;
    static ComDataGui::BarChartGraph* sBarChart;
    static ComDataGui::Pie2DGraph* sPie2D;
    static ComDataGui::BubbleGraph* sBubble;
    static ComDataGui::GraphProperty* sGraphProperty;
    static QStackedWidget* sDemoWidget;
    static bool sIsDirty;
    static bool sIsWritten;

    QWidget* m_widget = nullptr;
};

#endif // VIZGRAPHBASESETTINGS_H
