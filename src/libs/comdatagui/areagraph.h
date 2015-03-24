#ifndef AREAGRAPH_H
#define AREAGRAPH_H

#include <qwt/qwt_plot_intervalcurve.h>

#include "graph.h"

namespace ComDataGui
{

/*!
 * \class 面积图类
 */
class COMDATAGUI_EXPORT AreaGraph : public Graph
{
    Q_OBJECT
public:
    enum Type { Area, StackArea, FillArea };

    explicit AreaGraph(int type, QWidget *parent = 0);

    void initialize();
    Graph* clone();
    Graph* rawClone();
    void dataUpdated();
    void clear();

private:
    QwtPlotIntervalCurve* createIntervalCurve(const QString& title, const QColor& clr);
};

}
#endif // AREAGRAPH_H
