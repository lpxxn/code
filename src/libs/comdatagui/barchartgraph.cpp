#include <qwt/qwt_plot_barchart.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_column_symbol.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_textlabel.h>
#include <qwt/qwt_plot_layout.h>

#include "barchartgraph.h"

namespace ComDataGui
{

class BarChart: public QwtPlotBarChart
{
public:
    BarChart(Graph* g): QwtPlotBarChart( "Page Hits" ), m_graph(g), m_useGroupColors(false)
    {
        setLegendMode( QwtPlotBarChart::LegendBarTitles );
        setLegendIconSize( QSize( 10, 14 ) );
    }

    void addItem( const QString &distro, const QColor &color )
    {
        m_colors += color;
        m_distros += distro;
    }

    void setUseGroupColors(bool use)
    {
        m_useGroupColors = use;
    }

    void setColors(const QList<QColor>& clrs)
    {
        m_colors = clrs;
    }

    virtual QwtColumnSymbol *specialSymbol(
        int index, const QPointF& ) const
    {
        // we want to have individual colors for each bar
        QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
        symbol->setLineWidth( 2 );
        symbol->setFrameStyle( plot()->barChartStyle() );

        QColor c( Qt::white );
        if ( m_useGroupColors && !m_colors.isEmpty())
            c = m_colors[ index % m_colors.size()];
        else
            c = m_graph->randomColor();

        symbol->setPalette( c );

        return symbol;
    }

    virtual QwtText barTitle( int sampleIndex ) const
    {
        QwtText title;
        if ( sampleIndex >= 0 && sampleIndex < m_distros.size() )
            title = m_distros[ sampleIndex ];

        return title;
    }

    void clear()
    {
        m_colors.clear();
        m_distros.clear();
    }

private:
    QList<QColor> m_colors;
    QList<QString> m_distros;
    Graph* m_graph;
    bool m_useGroupColors;
};

BarChartGraph::BarChartGraph(int type, QWidget *parent) :
    Graph(type, parent)
{
}

/*!
 * \reimp
 */
Graph* BarChartGraph::clone()
{
    return cloneGraph<BarChartGraph>(this, m_type, m_columnData);
}

/*!
 * \reimp
 */
Graph* BarChartGraph::rawClone()
{
    return cloneRawGraph<BarChartGraph>(this, m_type);
}

/*!
 * \reimp
 */
void BarChartGraph::initialize()
{
    switch (type()) {
    case Column:
        setTitle(tr("Column"));
        m_property.setTitleText(tr("Column"));
        break;

    case ColumnPlusLabel:
        setTitle(tr("Column Plus Label"));
        m_property.setTitleText(tr("Column Plus Label"));
        break;

    case Bar:
        setTitle(tr("Bar"));
        m_property.setTitleText(tr("Bar"));
        break;

    case BarPlusLabel:
        setTitle(tr("Bar Plus Label"));
        m_property.setTitleText(tr("Bar Plus Label"));
        break;
    }

    m_chartItem = new BarChart(this);

    switch (m_type) {
    case Bar:
        m_chartItem->setOrientation(Qt::Horizontal);
        break;

    case BarPlusLabel:
        m_chartItem->setOrientation(Qt::Horizontal);
        break;

    case Column:
        m_chartItem->setOrientation(Qt::Vertical);
        break;

    case ColumnPlusLabel:
        m_chartItem->setLegendMode(QwtPlotBarChart::LegendBarTitles);
        break;
    }

    dataUpdated();
    m_chartItem->attach(this);
}

/*!
 * \reimp
 */
void BarChartGraph::dataUpdated()
{
    if (m_columnData.isEmpty())
        return;

    Graph::dataUpdated();
    QVector<double> samples;
    const int revSize = m_columnData.first()->size();
    samples.reserve(revSize);
    QVector<double> x;
    x.reserve(revSize);
    QVector<QPointF> samplesPoint;
    QVector<double> xErrVals;
    xErrVals.reserve(revSize);
    QVector<double> yErrVals;
    yErrVals.reserve(revSize);
    m_hasXColumn = false;
    m_chartItem->clear();
    qreal min = ULLONG_MAX;

    for (int i = 0; i < m_columnData.size(); ++i) {

        if (m_columnData.at(i)->type() == ComData::ColumnBase::String ||
            m_columnData.at(i)->type() == ComData::ColumnBase::DateTime)
            continue;

        QSharedPointer<ComData::ColumnBase> colData = m_columnData.at(i);

        if (colData != nullptr) {
            if (colData->significance() == ComData::ColumnBase::X) {
                m_hasXColumn = true;
                for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex) {
                    double data = colData->cell(dataIndex).toDouble();
                    x.push_back(data);
                }
                continue;
            } else if (colData->significance() == ComData::ColumnBase::XError) {
                for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex)
                    xErrVals.append(colData->cell(dataIndex).toDouble());
            } else if (colData->significance() == ComData::ColumnBase::YError) {
                for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex)
                    yErrVals.append(colData->cell(dataIndex).toDouble());
            } else {
                Q_ASSERT(x.size() == m_columnData.at(i)->size());
                if (m_hasXColumn) {
                    for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex) {
                        double data = colData->cell(dataIndex).toDouble();
                        if (min > data)
                            min = data;

                        m_chartItem->addItem(tr("%1").arg(data), randomColor());
                        samplesPoint.append(QPointF(x.at(dataIndex), data));
                    }
                } else {
                    for (int dataIndex = 0; dataIndex < m_columnData.at(i)->size(); ++dataIndex) {
                        double data = colData->cell(dataIndex).toDouble();
                        if (min > data)
                            min = data;

                        m_chartItem->addItem(tr("%1").arg(data), randomColor());
                        samples.append(data);
                    }
                }
            }
        }
    }

    if (m_hasXColumn)
        m_chartItem->setSamples(samplesPoint);
    else
        m_chartItem->setSamples(samples);

    if (!xErrVals.isEmpty())
        m_chartItem->setXErrorValues(xErrVals);
    if (!yErrVals.isEmpty())
        m_chartItem->setYErrorValues(yErrVals);

    setPrecisionByMinValue(min);

    replot();
}

/*!
 * \reimp
 */
void BarChartGraph::replot()
{
    resetColorIndex();
    m_chartItem->setUseGroupColors(m_useGroupPatternColor);
    if (m_useGroupLineColor && !m_groupPatternColors.isEmpty())
        m_chartItem->setColors(m_groupPatternColors);

    Graph::replot();
}

/*!
 * \reimp
 */
void BarChartGraph::drawCanvas(QPainter *painter)
{
    QwtPlot::drawCanvas(painter);

    drawXErrorBars(painter);
    drawYErrorBars(painter);

    if (m_type == Bar || m_type == Column)
        return;

    painter->save();

    if (m_type == ColumnPlusLabel)
        drawColumnLabels(painter);
    else if (m_type == BarPlusLabel)
        drawBarLabels(painter);


    painter->restore();
}

/*!
 * \brief 绘制竖向柱状图的标签.
 */
void BarChartGraph::drawColumnLabels(QPainter* painter)
{
    if (!isLabelVisible())
        return;

    painter->save();
    painter->setFont(labelFont());
    painter->setOpacity(labelOpacity());
    painter->setPen(labelColor());

    QwtPlotItemList barList = itemList(QwtPlotItem::Rtti_PlotBarChart);
    QwtPlotBarChart* barChart = static_cast<QwtPlotBarChart*>(barList.at(0));
    int sampleCount = barChart->dataSize();
    for (int i = 0; i < sampleCount; ++i) {
        QString title = barChart->barTitle(i).text();
        QRect barRect = barChart->columnRect(i).toRect().toRect();
        qreal textHeight = fontMetrics().height();
        QRect titleRect = titleRectByLabelPosition(barRect, labelPosition(), textHeight);
        if (titleRect.y() < 0) {
            titleRect.setY(0);
            titleRect.setBottom(fontMetrics().height() * 2);
        }

        painter->drawText(titleRect.bottomLeft().x() + labelOffsetX(), titleRect.bottomLeft().y() + labelOffsetY(), title);
    }
    painter->restore();
}

/*!
 * \brief 绘制横向柱状图的标签.
 */
void BarChartGraph::drawBarLabels(QPainter* painter)
{
    if (!isLabelVisible())
        return;

    painter->save();
    painter->setFont(labelFont());
    painter->setOpacity(labelOpacity());
    painter->setPen(labelColor());

    QwtPlotItemList barList = itemList(QwtPlotItem::Rtti_PlotBarChart);
    QwtPlotBarChart* barChart = static_cast<QwtPlotBarChart*>(barList.at(0));
    QwtPlotLayout* layout = plotLayout();
    QRect canvasRect = layout->canvasRect().toRect();
    int sampleCount = barChart->dataSize();
    for (int i = 0; i < sampleCount; ++i) {
        QString title = barChart->barTitle(i).text();
        QRect barRect = barChart->columnRect(i).toRect().toRect();
        int titleLen = fontMetrics().width(title) * 2;
        qreal textHeight = fontMetrics().height();
        QRect titleRect = titleRectByLabelPosition(barRect, labelPosition(), textHeight);

        if ((titleRect.x() + titleLen )>= canvasRect.width()) {
            titleRect.setLeft(barRect.right() - titleLen);
            titleRect.setRight(barRect.right());
        }
        painter->drawText(titleRect.bottomLeft().x() + labelOffsetX(), titleRect.bottomLeft().y() + labelOffsetY(), title);
    }

    painter->restore();
}

/*!
 * \brief 绘制X误差条.
 */
void BarChartGraph::drawXErrorBars(QPainter* painter)
{
    QwtPlotItemList barList = itemList(QwtPlotItem::Rtti_PlotBarChart);
    QwtPlotBarChart* barChart = static_cast<QwtPlotBarChart*>(barList.at(0));
    QVector<double> xErrVals = barChart->xErrorValues();
    if (xErrVals.isEmpty())
        return;

    int sampleCount = barChart->dataSize();
    Q_ASSERT(xErrVals.size() == sampleCount);
    if (m_type == Column || m_type == ColumnPlusLabel)
        drawXErrorBar_Column(painter, barChart, xErrVals);
    else
        drawXErrorBar_Bar(painter, barChart, xErrVals);
}

/*!
 * \brief 绘制Y误差条.
 */
void BarChartGraph::drawYErrorBars(QPainter* painter)
{
    QwtPlotItemList barList = itemList(QwtPlotItem::Rtti_PlotBarChart);
    QwtPlotBarChart* barChart = static_cast<QwtPlotBarChart*>(barList.at(0));
    QVector<double> yErrVals = barChart->yErrorValues();
    if (yErrVals.isEmpty())
        return;

    int sampleCount = barChart->dataSize();
    Q_ASSERT(yErrVals.size() == sampleCount);
    if (m_type == Column || m_type == ColumnPlusLabel)
        drawYErrorBar_Column(painter, barChart, yErrVals);
    else
        drawYErrorBar_Bar(painter, barChart, yErrVals);
}

// the 4 drawXXXErrorBar functions could be refatored more elegantly
// do it later
void BarChartGraph::drawXErrorBar_Column(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error)
{
    painter->save();
    painter->setPen(QPen(errorBarLineColor(), errorBarLineWidth()));
    painter->setOpacity(errorBarLineOpacity());
    const int handleWidth = errorBarHandleWidth();
    const int sampleCount = barChart->dataSize();

    // draw bar error bars
    for (int i = 0; i < sampleCount; ++i) {
        QString title = barChart->barTitle(i).text();
        qreal value = title.toDouble();
        QRect barRect = barChart->columnRect(i).toRect().toRect();
        qreal barHeight = barRect.height();
        qreal errorBarLen = (qreal)error.at(i) * barHeight / value;

        // draw column error bars
        QPoint middleTop(barRect.topLeft().x() + barRect.width() / 2, barRect.topLeft().y());
        if (isErrorBarPlusVisible()) {
            QPoint right(middleTop.x() + errorBarLen, middleTop.y());
            painter->drawLine(middleTop, right);

            QPoint top(right.x(), right.y() - handleWidth);
            QPoint bottom(right.x(), right.y() + handleWidth);
            painter->drawLine(top, bottom);
        }

        if (isErrorBarMinusVisible()) {
            QPoint left(middleTop.x() - errorBarLen, middleTop.y());
            painter->drawLine(middleTop, left);

            QPoint top(left.x(), left.y() - handleWidth);
            QPoint bottom(left.x(), left.y() + handleWidth);
            painter->drawLine(top, bottom);
        }
    }
    painter->restore();
}

void BarChartGraph::drawXErrorBar_Bar(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error)
{
    painter->save();
    painter->setPen(QPen(errorBarLineColor(), errorBarLineWidth()));
    painter->setOpacity(errorBarLineOpacity());
    const int handleWidth = errorBarHandleWidth();
    const int sampleCount = barChart->dataSize();

    // draw bar error bars
    for (int i = 0; i < sampleCount; ++i) {
        QString title = barChart->barTitle(i).text();
        qreal value = title.toDouble();
        QRect barRect = barChart->columnRect(i).toRect().toRect();
        qreal barWidth = barRect.width();
        qreal errorBarLen = (qreal)error.at(i) * barWidth/ value;

        // draw column error bars
        QPoint rightMiddle(barRect.topRight().x(), barRect.topRight().y() + barRect.height() / 2);
        if (isErrorBarPlusVisible()) {
            QPoint right(rightMiddle.x() + errorBarLen, rightMiddle.y());
            painter->drawLine(rightMiddle, right);

            QPoint top(right.x(), right.y() - handleWidth);
            QPoint bottom(right.x(), right.y() + handleWidth);
            painter->drawLine(top, bottom);
        }

        if (isErrorBarMinusVisible()) {
            QPoint left(rightMiddle.x() - errorBarLen, rightMiddle.y());
            painter->drawLine(rightMiddle, left);

            QPoint top(left.x(), left.y() - handleWidth);
            QPoint bottom(left.x(), left.y() + handleWidth);
            painter->drawLine(top, bottom);
        }
    }
    painter->restore();
}

void BarChartGraph::drawYErrorBar_Column(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error)
{
    painter->save();
    painter->setPen(QPen(errorBarLineColor(), errorBarLineWidth()));
    painter->setOpacity(errorBarLineOpacity());
    const int handleWidth = errorBarHandleWidth();
    const int sampleCount = barChart->dataSize();

    // draw column error bars
    for (int i = 0; i < sampleCount; ++i) {
        QString title = barChart->barTitle(i).text();
        qreal value = title.toDouble();
        QRect barRect = barChart->columnRect(i).toRect().toRect();
        qreal barHeight = barRect.height();
        qreal errorBarLen = (qreal)error.at(i) * barHeight / value;

        // draw column error bars
        QPoint middleTop(barRect.topLeft().x() + barRect.width() / 2, barRect.topLeft().y());
        if (isErrorBarPlusVisible()) {
            QPointF top(middleTop.x(), middleTop.y() - errorBarLen);
            painter->drawLine(middleTop, top);

            // draw handle
            QPoint left(top.x() - handleWidth, top.y());
            QPoint right(top.x() + handleWidth, top.y());
            painter->drawLine(left, right);
        }

        if (isErrorBarMinusVisible()) {
            QPoint bottom(middleTop.x(), middleTop.y() + errorBarLen);
            painter->drawLine(middleTop, bottom);

            // draw handle
            QPoint left(bottom.x() - handleWidth, bottom.y());
            QPoint right(bottom.x() + handleWidth, bottom.y());
            painter->drawLine(left, right);
        }
    }
    painter->restore();
}

void BarChartGraph::drawYErrorBar_Bar(QPainter* painter, QwtPlotBarChart* barChart, const QVector<double>& error)
{
    painter->save();
    painter->setPen(QPen(errorBarLineColor(), errorBarLineWidth()));
    painter->setOpacity(errorBarLineOpacity());
    const int handleWidth = errorBarHandleWidth();
    const int sampleCount = barChart->dataSize();

    // draw column error bars
    for (int i = 0; i < sampleCount; ++i) {
        QString title = barChart->barTitle(i).text();
        qreal value = title.toDouble();
        QRect barRect = barChart->columnRect(i).toRect().toRect();
        qreal barWidth = barRect.width();
        qreal errorBarLen = (qreal)error.at(i) * barWidth / value;

        // draw column error bars
        QPoint rightMiddle(barRect.topRight().x(), barRect.topRight().y() + barRect.height() / 2);
        if (isErrorBarPlusVisible()) {
            QPointF top(rightMiddle.x(), rightMiddle.y() - errorBarLen);
            painter->drawLine(rightMiddle, top);

            // draw handle
            QPoint left(top.x() - handleWidth, top.y());
            QPoint right(top.x() + handleWidth, top.y());
            painter->drawLine(left, right);
        }

        if (isErrorBarMinusVisible()) {
            QPoint bottom(rightMiddle.x(), rightMiddle.y() + errorBarLen);
            painter->drawLine(rightMiddle, bottom);

            // draw handle
            QPoint left(bottom.x() - handleWidth, bottom.y());
            QPoint right(bottom.x() + handleWidth, bottom.y());
            painter->drawLine(left, right);
        }
    }
    painter->restore();
}

/*!
 * \brief 计算标签的位置矩形.
 * \param barRect 柱状条的矩形
 * \param lp 标签位置
 * \param textHeight 文本高度
 * \return 位置矩形
 */
QRect BarChartGraph::titleRectByLabelPosition(const QRect &barRect, QwtPlot::LabelPosition lp, qreal textHeight)
{
    const qreal halfHeight = (qreal)textHeight / 2;
    QRect titleRect;
    switch (lp) {
    case QwtPlot::Center:
        titleRect = QRect(QPoint(barRect.topLeft().x(), barRect.bottom() - barRect.height() / 2 - halfHeight),
                          QPoint(barRect.topRight().x(), barRect.bottom() - barRect.height() / 2 + halfHeight));
        break;

    case QwtPlot::InsideBase:
        titleRect = QRect(QPoint(barRect.bottomLeft().x(), barRect.bottomLeft().y() - fontMetrics().height()),
                          QPoint(barRect.bottomRight().x(), barRect.bottomRight().y()));
        break;

    case QwtPlot::InsideEnd:
        titleRect = QRect(QPoint(barRect.topLeft().x(), barRect.topLeft().y() + fontMetrics().height()),
                          QPoint(barRect.topRight().x(), barRect.topRight().y() + 2 * fontMetrics().height()));
        break;

    case QwtPlot::OutsideEnd:
        titleRect = QRect(QPoint(barRect.left(), barRect.top() - fontMetrics().height() * 2), barRect.topRight());
        break;
    }

    return titleRect;
}

} // namespace VizCore
