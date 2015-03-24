#ifndef MULTIBARCHARTGRAPH_H
#define MULTIBARCHARTGRAPH_H

#include "graph.h"

class QwtPlotMultiBarChart;

namespace ComDataGui
{

/*!
 * \class 多柱状图类.
 */
class COMDATAGUI_EXPORT MultiBarChartGraph : public Graph
{
    Q_OBJECT
public:
    enum Type
    {
        StackColumn,
        StackBar,
        FloatingColumn,
        FloatingBar
    };

    explicit MultiBarChartGraph(int type, QWidget *parent = 0);

    Graph* clone();
    Graph* rawClone();
    void initialize();
    void dataUpdated();

private:
    QwtPlotMultiBarChart *m_chartItem = nullptr;
};

} // namespace VizCore
#endif // MULTIBARCHARTGRAPH_H
