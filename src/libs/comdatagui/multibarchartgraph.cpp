#include "multibarchartgraph.h"

#include <qwt/qwt_plot_multi_barchart.h>
#include <qwt/qwt_column_symbol.h>
#include <qwt/qwt_legend.h>

namespace ComDataGui
{

MultiBarChartGraph::MultiBarChartGraph(int type, QWidget *parent) :
    Graph(type, parent)
{
}

/*!
 * \reimp
 */
Graph* MultiBarChartGraph::clone()
{
    return cloneGraph<MultiBarChartGraph>(this, m_type, m_columnData);
}

/*!
 * \reimp
 */
Graph* MultiBarChartGraph::rawClone()
{
    return cloneRawGraph<MultiBarChartGraph>(this, m_type);
}

/*!
 * \reimp
 */
void MultiBarChartGraph::initialize()
{
    switch (type()) {
    case StackColumn:
        setTitle(tr("Stacked Columns"));
        m_property.setTitleText(tr("Stacked Columns"));
        break;

    case StackBar:
        setTitle(tr("Stacked Bars"));
        m_property.setTitleText(tr("Fill Area"));
        break;

    case FloatingColumn:
        setTitle(tr("Floating Columns"));
        m_property.setTitleText(tr("Floating Columns"));
        break;

    case FloatingBar:
        setTitle(tr("Floating Bars"));
        m_property.setTitleText(tr("Floating Bars"));
        break;
    }

    m_chartItem = new QwtPlotMultiBarChart;
    m_chartItem->setStyle(QwtPlotMultiBarChart::Stacked);
    m_chartItem->setLayoutPolicy(QwtPlotMultiBarChart::AutoAdjustSamples);
    m_chartItem->setSpacing(20);
    m_chartItem->setMargin(3);

    if (m_type == StackBar || m_type == FloatingBar)
        m_chartItem->setOrientation(Qt::Horizontal);

#if 1
    if (m_type == FloatingBar || m_type == FloatingColumn)
        m_chartItem->setFloatingBar(true);
#endif

    dataUpdated();

    m_chartItem->attach(this);
}

/*!
 * \reimp
 */
void MultiBarChartGraph::dataUpdated()
{
    if (m_columnData.isEmpty())
        return;

    Graph::dataUpdated();

    QList<QwtText> titles;
    bool first = true;
    for (int i = 0; i < m_columnData.count(); i++) {
        if (! m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
//            if (first && (m_type == FloatingBar || m_type == FloatingColumn)) {
//                first = false;
//                continue;
//            }
            titles += m_columnData.at(i)->title();
        }
    }

    m_chartItem->setBarTitles(titles);
    m_chartItem->setLegendIconSize(QSize(10, 14));

    m_hasXColumn = false;
    first = true;
    QVector<double> x;
    x.reserve(m_columnData.first()->size());
    qreal min = ULLONG_MAX;

    for (int i = 0; i < m_columnData.count(); i++) {

        QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
        symbol->setLineWidth(1);
        symbol->setFrameStyle(QwtColumnSymbol::Raised);
        symbol->setPalette(QPalette(randomColor()));
        m_chartItem->setSymbol(i, symbol);

        if (m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
            m_hasXColumn = true;
            for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex)
                x.append(m_columnData.at(i)->cell(dataIndex).toDouble());
            continue;
        }
    }

    int dataCount = 0;
    if (!m_columnData.isEmpty())
        dataCount = m_columnData.at(0)->size();

    if (!m_hasXColumn) {
        QVector< QVector<double> > series;
        for (int i = 0; i < dataCount; ++i) {
            QVector<double> values;
            for (int j = 0; j < m_columnData.size(); j++) {
                values += x.at(j);
                qreal data = m_columnData.at(j)->cell(i).toDouble();
                values += data;

                if (min > data)
                    min = data;
            }
            series += values;
        }

        m_chartItem->setSamples(series);
    } else {
        QVector<QwtSetSample> series;
        for (int dataIndex = 0; dataIndex < dataCount; ++dataIndex) {
            QwtSetSample sample;
            for (int i = 0; i < m_columnData.size(); ++i) {
                if (m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
                    sample.value = x.at(dataIndex);
                    continue;
                }

                qreal data = m_columnData.at(i)->cell(dataIndex).toDouble();
                sample.set.append(data);

                if (min > data)
                    min = data;
            }
            series.append(sample);
        }
        m_chartItem->setSamples(series);
    }

    setPrecisionByMinValue(min);
    replot();
}

} // namespace VizCore
