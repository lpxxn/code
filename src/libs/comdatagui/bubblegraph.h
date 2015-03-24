#ifndef BUBBLEGRAPH_H
#define BUBBLEGRAPH_H

#include "graph.h"

namespace ComDataGui
{

/*!
 * \class 冒泡图类
 */
class COMDATAGUI_EXPORT BubbleGraph : public Graph
{
    Q_OBJECT
public:
    explicit BubbleGraph(QWidget *parent = 0);

    void initialize();
    Graph* clone();
    Graph* rawClone();
    void dataUpdated();
    void clear();
    void customize();

public slots:
    void replot();

protected:
    QList<QwtPlotItem*> m_plotItems;
};

}
#endif // BUBBLEGRAPH_H
