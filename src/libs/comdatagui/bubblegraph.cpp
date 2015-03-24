#include "bubblegraph.h"

#include <qwt/qwt_plot_seriesitem.h>
#include <qwt/qwt_series_store.h>
#include <qwt/qwt_point_mapper.h>
#include <qwt/qwt_painter.h>
#include <qwt/qwt_point_data.h>
#include <qwt/qwt_clipper.h>

namespace ComDataGui
{

static int qwtVerifyRange( int size, int &i1, int &i2 )
{
    if ( size < 1 )
        return 0;

    i1 = qBound( 0, i1, size - 1 );
    i2 = qBound( 0, i2, size - 1 );

    if ( i1 > i2 )
        qSwap( i1, i2 );

    return ( i2 - i1 + 1 );
}

class BubblePlot : public QwtPlotSeriesItem, public QwtSeriesStore<QPointF>
{
public:
    BubblePlot(const QString& title);
    virtual void drawSeries( QPainter *painter,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;
    virtual QwtGraphic legendIcon( int index, const QSizeF & ) const;

    void setSize(int size);
    int size() const;

    void setColor(const QColor& clr);
    QColor color() const;

    void setEdgeWidth(int width);
    int edgeWidth() const;

    void setEdgeColor(const QColor& clr);
    QColor edgeColor() const;

    void setOpacity(float opacity);
    float opacity() const;

    void setSymbol(QwtSymbol* symbol);
    const QwtSymbol *symbol() const;

    void setPen( const QPen & );
    const QPen &pen() const;

    int rtti() const;

    void setSamples( const QVector<double> &xData, const QVector<double> &yData );


private:
    void init();

    class PrivateData;
    PrivateData* d_data;

};

class BubblePlot::PrivateData
{
public:
    PrivateData(): color(Qt::yellow),
                   symbol(nullptr),
                   size(5),
                   opacity(1.0) {}
    QColor color;
    QwtSymbol *symbol;

    int size;
    float opacity;

    QPen pen;
};

BubblePlot::BubblePlot(const QString& title): QwtPlotSeriesItem(QwtText(title))
{
    init();
}

/*!
 * \reimp
 */
void BubblePlot::drawSeries(QPainter *painter, const QwtScaleMap &xMap,
                            const QwtScaleMap &yMap, const QRectF &canvasRect,
                            int from, int to) const
{
    const size_t numSamples = dataSize();

    if ( !painter || numSamples <= 0 )
        return;

    if ( to < 0 )
        to = numSamples - 1;

    painter->save();
    painter->setOpacity(d_data->opacity);

    if (d_data->symbol) {
        d_data->symbol->setSize(d_data->size);
        d_data->symbol->setPen(d_data->pen);
        d_data->symbol->setColor(Qt::transparent);
    }

    if ( qwtVerifyRange( numSamples, from, to ) > 0 ) {
        QRectF clipRect;
        qreal pw = qMax( qreal( 1.0 ), painter->pen().widthF());
        clipRect = canvasRect.adjusted(-pw, -pw, pw, pw);

        QwtPointMapper mapper;
        mapper.setFlag( QwtPointMapper::RoundPoints,
            QwtPainter::roundingAlignment( painter ) );
        mapper.setBoundingRect( canvasRect );

        const int chunkSize = 500;

        for (int i = from; i <= to; i += chunkSize) {
            const int n = qMin( chunkSize, to - i + 1 );
            const QPolygonF points = mapper.toPointsF( xMap, yMap,
                data(), i, i + n - 1 );

            if (points.size() && d_data->symbol)
                d_data->symbol->drawSymbols( painter, points );
        }
    }

    painter->restore();
}

void BubblePlot::init()
{
    setItemAttribute(QwtPlotItem::Legend);
    setItemAttribute(QwtPlotItem::AutoScale);
    setData(new QwtPointSeriesData());

    d_data = new PrivateData;
    d_data->symbol = new QwtSymbol(QwtSymbol::Triangle);
    d_data->symbol->setSize(20);

    setZ(20.0);
}

void BubblePlot::setSize(int size)
{
    d_data->size = size;
}

int BubblePlot::size() const
{
    return d_data->size;
}

void BubblePlot::setEdgeWidth(int width)
{
    d_data->pen.setWidth(width);
}

int BubblePlot::edgeWidth() const
{
    return d_data->pen.width();
}

void BubblePlot::setOpacity(float opacity)
{
    d_data->opacity = opacity;
}

float BubblePlot::opacity() const
{
    return d_data->opacity;
}

void BubblePlot::setSymbol(QwtSymbol* symbol)
{
    if (d_data->symbol != symbol) {
        delete d_data->symbol;
        d_data->symbol = symbol;
    }
}

const QwtSymbol *BubblePlot::symbol() const
{
    return d_data->symbol;
}

void BubblePlot::setPen( const QPen & p)
{
    if (p != d_data->pen)
        d_data->pen = p;
}

const QPen &BubblePlot::pen() const
{
    return d_data->pen;
}

/*!
 * \reimp
 */
int BubblePlot::rtti() const
{
    return QwtPlotItem::Rtti_PlotUserItem + 1;
}

void BubblePlot::setColor(const QColor &clr)
{
    d_data->color = clr;
}

QColor BubblePlot::color() const
{
    return d_data->color;
}

void BubblePlot::setEdgeColor(const QColor &clr)
{
    d_data->pen.setColor(clr);
}

QColor BubblePlot::edgeColor() const
{
    return d_data->pen.color();
}

void BubblePlot::setSamples(const QVector<double> &xData, const QVector<double> &yData)
{
    setData( new QwtPointArrayData( xData, yData ) );
}

/*!
 * \reimp
 */
QwtGraphic BubblePlot::legendIcon( int index,
    const QSizeF &size ) const
{
    Q_UNUSED(index);

    if (size.isEmpty())
        return QwtGraphic();

    QwtGraphic graphic;
    graphic.setDefaultSize(size);
    graphic.setRenderHint(QwtGraphic::RenderPensUnscaled, true);

    QPainter painter( &graphic );
    painter.setRenderHint(QPainter::Antialiasing, testRenderHint(QwtPlotItem::RenderAntialiased));

    QRectF r(0, 0, size.width(), size.height());
    painter.fillRect(r, QBrush(d_data->pen.color()));

    return graphic;
}

BubbleGraph::BubbleGraph(QWidget *parent) : Graph(parent)
{
}

/*!
 * \reimp
 */
void BubbleGraph::initialize()
{
    setTitle(tr("Bubble"));
    m_property.setTitleText(tr("Bubble"));
    dataUpdated();
}

/*!
 * \reimp
 */
Graph* BubbleGraph::clone()
{
    return cloneGraph<BubbleGraph>(this, m_columnData);
}

/*!
 * \reimp
 */
Graph* BubbleGraph::rawClone()
{
    return cloneRawGraph<BubbleGraph>(this);
}

/*!
 * \reimp
 */
void BubbleGraph::dataUpdated()
{
    if (m_columnData.isEmpty())
        return;

    Graph::dataUpdated();
    clear();

    m_hasXColumn = false;
    QVector<double> x;
    x.reserve(m_columnData.first()->size());
    qreal min = ULLONG_MAX;

    for (int i = 0; i < m_columnData.size(); ++i) {

        if (m_columnData.at(i)->type() == ComData::ColumnBase::DateTime ||
            m_columnData.at(i)->type() == ComData::ColumnBase::String)
            continue;

        QVector<double> y;
        y.reserve(m_columnData.first()->size());

        for (int index = 0; index < m_columnData.at(i)->size(); ++index) {
            if (m_columnData.at(i)->significance() == ComData::ColumnBase::X) {
                x.append(m_columnData.at(i)->cell(index).toDouble());
                m_hasXColumn = true;
                continue;
            }

            if (!m_hasXColumn)
                x.append(index);
            qreal data = m_columnData.at(i)->cell(index).toDouble();
            y.append(data);

            if (min > data)
                min = data;
        }

        if (y.isEmpty())
            continue;

        setPrecisionByMinValue(min);

        BubblePlot* plot = new BubblePlot(m_columnData.at(i)->title());
        m_plotItems.append(plot);
        plot->setSamples(x, y);
        plot->attach(this);

    }

    replot();
    updateLegend();

}

/*!
 * \reimp
 */
void BubbleGraph::clear()
{
    qDeleteAll(m_plotItems);
    m_plotItems.clear();
}

/*!
 * \reimp
 */
void BubbleGraph::customize()
{
    setGridVisible(false);
}

/*!
 * \reimp
 */
void BubbleGraph::replot()
{
    resetColorIndex();
    for (int i = 0; i < m_plotItems.size(); ++i) {
        BubblePlot* bubble = static_cast<BubblePlot*>(m_plotItems.at(i));

        if (bubble) {
            bubble->setSize(bubbleSize());
            bubble->setEdgeWidth(bubbleEdgeWidth());
            bubble->setOpacity(bubbleOpacity());

            if (m_useGroupPatternColor && !m_groupPatternColors.isEmpty()) {
                const QColor fillColor = m_groupPatternColors.at(i % m_groupPatternColors.size());
                bubble->setEdgeColor(fillColor);
                bubble->setColor(fillColor);
            } else {
                const QColor fillColor = randomColor();
                bubble->setEdgeColor(fillColor);
                bubble->setColor(fillColor);
            }

            if (m_useGroupBubbleStyle && !m_groupBubbleStyles.isEmpty()) {
                QwtSymbol* symbol = new QwtSymbol(m_groupBubbleStyles.at( i % m_groupBubbleStyles.size()));
                bubble->setSymbol(symbol);
            } else {
                QwtSymbol* symbol = new QwtSymbol(QwtSymbol::Rect);
                bubble->setSymbol(symbol);
            }
        }
    }

    Graph::replot();
}

}
