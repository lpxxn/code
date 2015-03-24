#ifndef PIE2DGRAPH_H
#define PIE2DGRAPH_H

#include "graph.h"
#include "comdatagui_global.h"

namespace ComDataGui
{

/*!
 * \class 二维饼图类.
 */
class COMDATAGUI_EXPORT Pie2DGraph : public Graph
{
    Q_OBJECT
public:
    explicit Pie2DGraph(QWidget* parent = nullptr);

    Graph* clone();
    Graph* rawClone(); // without data copying
    void initialize();
    void dataUpdated();
    void clear();
    void addColumn(const PlotItemData& pid);
    void customize();
    int type() const;
    void replot();

protected:
    void resizeEvent(QResizeEvent *e);

private:
    class Pie2DGraphPrivate;
    Pie2DGraphPrivate* m_data;
};

}
#endif // PIE2DGRAPH_H
