#include "areagraph.h"

#include <qwt/qwt_legend.h>

namespace ComDataGui
{

AreaGraph::AreaGraph(int type, QWidget *parent) :
    Graph(type, parent)
{
}

void AreaGraph::initialize()
{
    switch (type()) {
    case Area:
        setTitle(tr("Area"));
        m_property.setTitleText(tr("Area"));
        break;

    case StackArea:
        setTitle(tr("Stack Area"));
        m_property.setTitleText(tr("Stack Area"));
        break;

    case FillArea:
        setTitle(tr("Fill Area"));
        m_property.setTitleText(tr("Fill Area"));
        break;
    }

    dataUpdated();
}

/*!
 * \reimp
 */
Graph* AreaGraph::clone()
{
    return cloneGraph<AreaGraph>(this, m_type, m_columnData);
}

/*!
 * \reimp
 */
Graph* AreaGraph::rawClone()
{
    return cloneRawGraph<AreaGraph>(this, m_type);
}

/*!
 * \reimp
 */
void AreaGraph::dataUpdated()
{
    if (m_columnData.isEmpty())
        return;

    Graph::dataUpdated();
    clear();

    QMap<int, double> xColumnIndexValueMap;
    m_hasXColumn = false;
    qreal min = ULLONG_MAX;

    QVector<double> xErrors;
    QVector<double> yErrors;

    // get x & y errors
    for (int i = 0; i < m_columnData.size(); ++i) {
        QSharedPointer<ComData::ColumnBase> colData = m_columnData.at(i);
        if (colData) {

            if (colData->significance() == ComData::ColumnBase::XError) {
                for (int i = 0; i < colData->size(); ++i)
                    xErrors.append(colData->cell(i).toDouble());
            } else if (colData->significance() == ComData::ColumnBase::YError) {
                for (int i = 0; i < colData->size(); ++i)
                    yErrors.append(colData->cell(i).toDouble());
            }
        }
    }

    // extract real data
    for (int i = 0; i < m_columnData.size(); ++i) {

        if (m_columnData.at(i)->type() == ComData::ColumnBase::String ||
            m_columnData.at(i)->type() == ComData::ColumnBase::DateTime)
            continue;

        QVector<QwtIntervalSample> samples;
        QSharedPointer<ComData::ColumnBase> colData = m_columnData.at(i);

        if (colData) {
            if (colData->significance() == ComData::ColumnBase::XError ||
                colData->significance() == ComData::ColumnBase::YError)
                continue;

                for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex) {
                    double value = colData->cell(dataIndex).toDouble();

                    if (min > value)
                        min = value;

                    if (m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
                        m_hasXColumn = true;
                        xColumnIndexValueMap.insert(dataIndex, value);
                        continue;
                    } else {
                        if (!m_hasXColumn) {
                            QwtIntervalSample sample(double(dataIndex), value > 0 ? QwtInterval(0, value) : QwtInterval(value, 0));
                            samples.append(sample);
                        } else {
                            QwtIntervalSample sample(xColumnIndexValueMap.value(dataIndex), value > 0 ? QwtInterval(0, value) : QwtInterval(value, 0));
                            samples.append(sample);
                        }
                    } // else
                } // for
            } // else

        if (samples.isEmpty())
            continue;

        QwtPlotIntervalCurve* curve = createIntervalCurve(tr("%1").arg(m_columnData.at(i)->title()),
                                                          randomColor());

        if (m_type == FillArea)
            curve->setStyle(QwtPlotIntervalCurve::Tube);
        curve->setSamples(samples);
        curve->attach(this);

        if (!xErrors.isEmpty())
            curve->setXErrorValues(xErrors);
        if (!yErrors.isEmpty())
            curve->setYErrorValues(yErrors);

        setPrecisionByMinValue(min);
    }

    replot();
}

/*!
 * \reimp
 */
void AreaGraph::clear()
{
    QwtPlotItemList curveList = itemList(QwtPlotItem::Rtti_PlotIntervalCurve);
    qDeleteAll(curveList);
    curveList.clear();
}

/*!
 * \brief 创建区间曲线对象.
 * \param title 标题
 * \param clr 颜色
 * \return 曲线对象
 */
QwtPlotIntervalCurve* AreaGraph::createIntervalCurve(const QString& title, const QColor& clr)
{
    QwtPlotIntervalCurve* intervalCurve = new QwtPlotIntervalCurve();
    intervalCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    intervalCurve->setPen( Qt::white );

    QColor bg(clr);
    bg.setAlpha(150);
    intervalCurve->setBrush(QBrush(bg));
    intervalCurve->setStyle(QwtPlotIntervalCurve::Tube);
    intervalCurve->attach(this);
    intervalCurve->setTitle(title);
    return intervalCurve;
}

} // namespace VizCore
