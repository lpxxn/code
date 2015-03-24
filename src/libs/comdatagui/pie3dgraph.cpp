#include "pie3dgraph.h"

namespace ComDataGui
{

Pie3DGraph::Pie3DGraph(int type, QWidget *parent): Graph(type, parent)
{

}

Pie3DGraph::Pie3DGraph(QWidget *parent) :
    Graph(parent)
{
}

Graph* Pie3DGraph::clone()
{
    return cloneGraph<Pie3DGraph>(0, m_columnData);
}

Graph* Pie3DGraph::rawClone()
{
    return cloneRawGraph<Pie3DGraph>(0);
}

void Pie3DGraph::initialize()
{

}

void Pie3DGraph::dataUpdated()
{

}

void Pie3DGraph::clear()
{

}

void Pie3DGraph::addColumn(const PlotItemData& pid)
{

}

}
