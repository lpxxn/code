#include <qwt/qwt_plot_histogram.h>
#include <qwt/qwt_legend.h>

#include "histogramgraph.h"

namespace ComDataGui
{

HistogramGraph::HistogramGraph(QWidget *parent) :
    Graph(parent)
{
}

/*!
 * \reimp
 */
Graph* HistogramGraph::clone()
{
    return cloneGraph<HistogramGraph>(this, m_columnData);
}

/*!
 * \reimp
 */
Graph* HistogramGraph::rawClone()
{
    return cloneRawGraph<HistogramGraph>(this);
}

/*!
 * \reimp
 */
void HistogramGraph::initialize()
{
    setTitle("Histogram");
    m_property.setTitleText(tr("Histogram"));
    canvas()->setPalette( QColor( "LemonChiffon" ) );
    setAxisTitle( QwtPlot::yLeft, "x values" );
    setAxisTitle( QwtPlot::xBottom, "y values" );
    resize(QSize(600,400));
    setAttribute(Qt::WA_DeleteOnClose);

    QwtPlotHistogram *barchart = new QwtPlotHistogram("Bar Chart");
    barchart->setStyle(QwtPlotHistogram::Lines);
    barchart->setSymbol(new QwtColumnSymbol(QwtColumnSymbol::Box));
    barchart->setBrush(QColor(233,12,54));

    //    barchart->setLayoutPolicy( QwtPlotMultiBarChart::AutoAdjustSamples );
    //    barchart->setSpacing( 20 );
    //    barchart->setMargin( 3 );

    //    QwtPlotBarChart* chart = new QwtPlotBarChart();
    //    chart
    //    QwtPlotCurve* curve = new QwtPlotCurve("Samples");
    //    curve->setStyle(QwtPlotCurve::Lines);
    static const char *colors[] = { "DarkOrchid", "SteelBlue", "Gold" };

    const int numSamples = 5;
    const int numBars = sizeof( colors ) / sizeof( colors[0] );

    QList<QwtText> titles;
    for ( int i = 0; i < numBars; i++ )
    {
        QString title("Bar %1");
        titles += title.arg( i );
    }
    //    barchart->setBarTitles( titles );
    barchart->setLegendIconSize( QSize( 10, 14 ) );

    QVector< QwtIntervalSample > series;
    for ( int i = 0; i < 100; i++ )
    {
        QwtIntervalSample sample;
        sample.value = rand() % 100;
        series.append(sample);
    }

    barchart->setSamples(series);

#if 0
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( this->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::darkGreen ) );
    zoomer->setTrackerMode( QwtPlotPicker::AlwaysOn );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
                             Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
                             Qt::RightButton );

    QwtPlotPanner *panner = new QwtPlotPanner( this->canvas() );
    panner->setMouseButton( Qt::MidButton );
#endif

    //    barchart->setStyle(QwtPlotMultiBarChart::Grouped);
    barchart->setOrientation(Qt::Vertical);
    barchart->attach(this);

    //    curve->attach(this);

    //    GraphItemPicker::instance()->setPlot(this);

    qDebug() << "histogram canvas: " << this->canvas();
    //    new CanvasPicker(this);

    QwtPlotItemList items = this->itemList();
    qDebug() << "item count: " << items.size();
    for (int i = 0; i < items.size(); ++i) {

    }
}

/*!
 * \reimp
 */
void HistogramGraph::dataUpdated()
{
}
} // namespace VizCore
