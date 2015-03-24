#include "curvegraph.h"

#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_picker.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_interval_symbol.h>
#include <qwt/qwt_curve_fitter.h>

#include <comdata/column.h>

namespace ComDataGui
{

CurveGraph::CurveGraph(int type, QWidget *parent) :
    Graph(type, parent)
{
}

/*!
 * \reimp
 */
Graph* CurveGraph::clone()
{
    return cloneGraph<CurveGraph>(this, m_type, m_columnData);
}

/*!
 * \reimp
 */
Graph* CurveGraph::rawClone()
{
    return cloneRawGraph<CurveGraph>(this, m_type);
}

/*!
 * \reimp
 */
void CurveGraph::initialize()
{
    resize(QSize(600,400));
    setAttribute(Qt::WA_DeleteOnClose);

    switch (type()) {
    case Line:
        setTitle(tr("Curve"));
        m_property.setTitleText(tr("Curve"));
        break;

    case HorizontalStep:
        setTitle(tr("Horizontal Step"));
        m_property.setTitleText(tr("Horizontal Step"));
        break;

    case VerticalStep:
        setTitle(tr("Vertical Step"));
        m_property.setTitleText(tr("Vertical Step"));
        break;

    case Stick:
        setTitle(tr("Stick"));
        m_property.setTitleText(tr("Stick"));
        break;

    case SplineConnected:
        setTitle(tr("Connected Spline"));
        m_property.setTitleText(tr("Connected Spline"));
        break;
    }

    dataUpdated();
}

/*!
 * \reimp
 */
void CurveGraph::dataUpdated()
{
    if (m_columnData.isEmpty())
        return;

    Graph::dataUpdated();
    clear();

    const int revSize = m_columnData.first()->size();
    QVector<double> x;
    x.reserve(revSize);
    m_plotItem2ColumnMap.clear();
    m_hasXColumn = false;
    qreal min = LLONG_MAX;

    for (int i = 0; i < m_columnData.size(); ++i) {

        if (m_columnData.at(i)->type() == ComData::ColumnBase::DateTime ||
            m_columnData.at(i)->type() == ComData::ColumnBase::String)
            continue;

        QwtPlotCurve* curve = new QwtPlotCurve(m_columnData.at(i)->title());

        if (property().isAntiAliasingCurve())
            curve->setRenderHint(QwtPlotItem::RenderAntialiased);

        QwtPlotCurve::CurveStyle cs;
        switch (m_type) {
        case Line:
            cs = QwtPlotCurve::Lines;
            curve->setStyle(QwtPlotCurve::Lines);
            break;

        case HorizontalStep:
            cs = QwtPlotCurve::Steps;
            curve->setStyle(QwtPlotCurve::Steps);
            break;

        case VerticalStep:
            cs = QwtPlotCurve::Steps;
            curve->setStyle(QwtPlotCurve::Steps);
            break;

        case Stick:
            cs = QwtPlotCurve::Sticks;
            curve->setStyle(QwtPlotCurve::Sticks);
            break;
        }

        if (m_type == HorizontalStep)
            curve->setCurveAttribute(QwtPlotCurve::Inverted);
        else if (m_type == SplineConnected) {
            QwtSplineCurveFitter *curveFitter = new QwtSplineCurveFitter();
            curveFitter->setSplineSize(150);
            curve->setCurveFitter(curveFitter);
            curve->setCurveAttribute(QwtPlotCurve::Fitted, true);
        }

        QPen pen(randomColor(), 1);
        curve->setPen(pen);

        if (!createCurve(curve, i, x, min))
            continue;

        QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotCurve);
        for (QwtPlotItem* item : items) {
            QwtPlotSeriesItem* si = static_cast<QwtPlotSeriesItem*>(item);
            if (!m_plotItem2ColumnMap.contains(si)) {
                PlotItemData pid;
                pid.column = m_columnData.at(i);
                pid.isAntialiased = si->testRenderHint(QwtPlotItem::RenderAntialiased);
                pid.pen = pen;
                pid.curveStyle = cs;
                pid.curve = curve;
                m_plotItem2ColumnMap.insert(si, pid);
            }
        }
    }

    setPrecisionByMinValue(min);
    replot();
}

/*!
 * \reimp
 */
void CurveGraph::clear()
{
    // here cannot use blockSignals to prevent the itemAttached from emitting
    // cause we need to update the legend items
    m_isInternalRemove = true;
    QwtPlotItemList curveList = itemList(QwtPlotItem::Rtti_PlotCurve);
    foreach (QwtPlotItem* item, curveList)
        delete item;
    m_isInternalRemove = false;
}

#if 0
void CurveGraph::addColumn(const PlotItemData& pid)
{
    if (pid.column.isNull())
        return;

    Graph::addColumn(pid);
    QVector<double> x;
    QVector<double> y;
    if (m_hasXColumn) {
        for (int i = 0; i < m_columnData.size(); ++i) {
                for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex)
                    if (m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
                        x.append(m_columnData.at(i)->cell(dataIndex).toDouble());
            }
        }
    }

    for (int i = 0; i < pid.column->size(); ++i) {
        if (!m_hasXColumn)
            x.append(i);
        y.append(pid.column->cell(i).toDouble());
    }

    // for resolving cloned graph doesn't have curves
    QwtPlotCurve* curve = new QwtPlotCurve(pid.column->title());
    if (pid.curve)
        curve->setCurveFitter(pid.curve->curveFitter());

    curve->setRenderHint(QwtPlotItem::RenderAntialiased, pid.isAntialiased);

    curve->setPen(pid.pen);
    curve->setSamples(x, y);
    curve->setStyle(pid.curveStyle);
    curve->attach(this);
}
#endif

void CurveGraph::onItemAttached(QwtPlotItem *plotItem, bool on)
{
    if (!on && !m_isInternalRemove) {
        QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);
        if (curves.isEmpty())
            emit destroy();

        // remove associated column data in case of cloning the graph
        QwtPlotSeriesItem* sitem = static_cast<QwtPlotSeriesItem*>(plotItem);
        PlotItemData& pid = m_plotItem2ColumnMap[sitem];
        m_columnData.removeAll(pid.column);
    }
}

bool CurveGraph::createCurve(QwtPlotCurve* curve, int i, QVector<double>& x, qreal& min)
{
    const int revSize = m_columnData.first()->size();

    QVector<double> y;
    y.reserve(revSize);
    QVector<double> xErrVals;
    xErrVals.reserve(revSize);
    QVector<double> yErrVals;
    yErrVals.reserve(revSize);

    for (int index = 0; index < m_columnData.at(i)->size(); ++index) {
        if (m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
            x.append(m_columnData.at(i)->cell(index).toDouble());
            m_hasXColumn = true;
            continue;
        } else if (m_columnData.at(i)->significance() == ComData::ColumnBase::XError) {
            delete curve;
            curve = nullptr;
            xErrVals.append(m_columnData.at(i)->cell(index).toDouble());
        } else if (m_columnData.at(i)->significance() == ComData::ColumnBase::YError) {
            delete curve;
            curve = nullptr;
            yErrVals.append(m_columnData.at(i)->cell(index).toDouble());
        } else if (m_columnData.at(i)->significance() == ComData::ColumnBase::Y) {
            qreal data = m_columnData.at(i)->cell(index).toDouble();
            y.append(data);

            if (min > data)
                min = data;
        }

        if (!m_hasXColumn)
            x.append(index);
    }

    // ignore the curve only with x values
    if (y.isEmpty() && xErrVals.isEmpty() && yErrVals.isEmpty()) {
        delete curve;
        curve = nullptr;
        return false;
    }

    if (!xErrVals.isEmpty() && !m_curves.isEmpty())
        m_curves.last()->setXErrorValues(xErrVals);
    if (!yErrVals.isEmpty() && !m_curves.isEmpty())
        m_curves.last()->setYErrorValues(yErrVals);

    if (curve) {
        curve->setSamples(x, y);
        curve->attach(this);
        m_curves.append(curve);
    }
    return true;
}

} // namespace VizCore
