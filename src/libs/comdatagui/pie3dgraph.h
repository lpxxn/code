#ifndef PIE3DGRAPH_H
#define PIE3DGRAPH_H

#include "graph.h"
#include "comdatagui_global.h"

namespace ComDataGui
{

class COMDATAGUI_EXPORT Pie3DGraph : public Graph
{
    Q_OBJECT
public:
    explicit Pie3DGraph(int type, QWidget* parent = nullptr);
    explicit Pie3DGraph(QWidget *parent = nullptr);

    Graph* clone();
    Graph* rawClone(); // without data copying
    void initialize();
    void dataUpdated();
    void clear();
    void addColumn(const PlotItemData& pid);

};

}
#endif // PIE3DGRAPH_H
