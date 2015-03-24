#ifndef CURVEGRAPH_H
#define CURVEGRAPH_H

#include "graph.h"

namespace ComDataGui
{

/*!
 * \class 曲线图类
 */
class COMDATAGUI_EXPORT CurveGraph : public Graph
{
    Q_OBJECT
public:
    enum Type
    {
        Line,
        HorizontalStep,
        VerticalStep,
        Stick,
        SplineConnected
    };

    explicit CurveGraph(int type, QWidget *parent = 0);

    Graph* clone();
    Graph* rawClone();
    void initialize();
    void dataUpdated();
    void clear();
//    void addColumn(const PlotItemData& pid);

protected slots:
    virtual void onItemAttached(QwtPlotItem *plotItem, bool on);

private:
    bool createCurve(QwtPlotCurve* curve, int i, QVector<double>& x, qreal& min);

private:
    QList<QwtPlotCurve* > m_curves;

};

} // namespace VizCore
#endif // CURVEGRAPH_H
