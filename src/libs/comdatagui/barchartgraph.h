#ifndef BARCHARTGRAPH_H
#define BARCHARTGRAPH_H

#include "graph.h"

class QwtPlotBarChart;
namespace ComDataGui
{

class BarChart;
/*!
 * \class 柱状图类
 */
class COMDATAGUI_EXPORT BarChartGraph : public Graph
{
    Q_OBJECT
public:
    enum Type
    {
        Column,
        ColumnPlusLabel,
        Bar,
        BarPlusLabel,
    };

    explicit BarChartGraph(int type, QWidget *parent = 0);

    Graph* clone();
    Graph* rawClone();
    void initialize();
    void dataUpdated();

public slots:
    void replot();

protected:
    void drawCanvas(QPainter* painter);
    void drawColumnLabels(QPainter* painter);
    void drawBarLabels(QPainter* painter);
    void drawXErrorBars(QPainter* painter);
    void drawYErrorBars(QPainter* painter);
    void drawXErrorBar_Column(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error);
    void drawXErrorBar_Bar(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error);
    void drawYErrorBar_Column(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error);
    void drawYErrorBar_Bar(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error);

    QRect titleRectByLabelPosition(const QRect& barRect, QwtPlot::LabelPosition lp, qreal textHeight);

private:
    BarChart* m_chartItem;
};

} // namespace VizCore
#endif // BARCHARTGRAPH_H
