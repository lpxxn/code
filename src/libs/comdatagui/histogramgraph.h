#ifndef HISTOGRAMGRAPH_H
#define HISTOGRAMGRAPH_H

#include "graph.h"

namespace ComDataGui
{

/*!
 * \class 柱状图类.
 */
class COMDATAGUI_EXPORT HistogramGraph : public Graph
{
    Q_OBJECT
public:
    explicit HistogramGraph(QWidget *parent = 0);

    Graph* clone();
    Graph* rawClone();
    void initialize();
    void dataUpdated();

};

} // namespace VizCore
#endif // HISTOGRAMGRAPH_H
