#include "graph.h"
#include "areagraph.h"
#include "barchartgraph.h"
#include "bubblegraph.h"
#include "curvegraph.h"
#include "histogramgraph.h"
#include "multibarchartgraph.h"
#include "pie2dgraph.h"

#include <comdata/column.h>
#include <comdata/table.h>

#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_legenditem.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_barchart.h>
#include <qwt/qwt_plot_multi_barchart.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_plot_renderer.h>

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QFileDialog>

namespace ComDataGui
{
static int MinWidth = 300;
static int MinHeight = 300;
static const QString Separator = "@";
static const QString GraphName = "graph";
static const char *DefaultColors[] =
{
    "LightSalmon",
    "SteelBlue",
    "Yellow",
    "Fuchsia",
    "PaleGreen",
    "PaleTurquoise",
    "Cornsilk",
    "HotPink",
    "Peru",
    "Maroon"
};

PlotItemData::PlotItemData()
{

}

PlotItemData::PlotItemData(const PlotItemData &other)
{
    column = other.column;
    isAntialiased = other.isAntialiased;
    pen = other.pen;
    curveStyle = other.curveStyle;
    curve = other.curve;
}

PlotItemData& PlotItemData::operator=(const PlotItemData& other)
{
    column = other.column;
    isAntialiased = other.isAntialiased;
    pen = other.pen;
    curveStyle = other.curveStyle;
    curve = other.curve;
    return *this;
}

void PlotItemData::reset()
{
    column.clear();
}

void PlotItemData::print() const
{
    qDebug() << "column: " << column;
    qDebug() << "anti aliasing: " << isAntialiased;
    qDebug() << "pen: " << pen;
    qDebug() << "curve style: " << curveStyle;
}

class GridData
{
public:
    GridData(): gridPen(QPen(Qt::white, 1, Qt::SolidLine)), visible(false),
                majorVisible(true), minorVisible(false), horizontalVisible(true),
                verticalVisible(false) {}
    QPen gridPen;
    bool visible : 1;
    bool majorVisible : 1;
    bool minorVisible : 1;
    bool horizontalVisible : 1;
    bool verticalVisible : 1;
};

class LegendData
{
public:
    void makeDirty() { isDirty = true; }

    QwtPlotLegendItem* legendItem = nullptr;
    bool legendVisible;
    bool legendItemVisible;
    QFont legendFont;
    QColor legendColor;
    QFont legendItemFont;
    QColor legendItemColor;
    QColor legendItemBgColor;
    QColor legendItemBorderColor;
    int legendPosition = QwtPlot::RightLegend;
    int legendItemColumns;
    int legendItemHAlign = Qt::AlignLeft;
    int legendItemVAlign = Qt::AlignTop;
    int legendItemBgMode = QwtPlotLegendItem::LegendBackground;
    bool isDirty;
};

class SymbolData
{
public:
    QwtSymbol::Style style;
    int size;
    int width;
    QColor color;
    QColor edgeColor;
    float opacity;
};

class DroplineData
{
public:
    bool horizontalVisible;
    bool verticalVisible;
    QPen pen;
    float opacity;
};

class BaseLabelData
{
public:
    QColor color;
    bool isVisible;
    float opacity;
    int xOffset;
    int yOffset;
    QFont font;
};

class CurveLabelData : public BaseLabelData
{
public:
    QwtPlotCurve::LabelPosition pos;
};

class BarChartLabelData : public BaseLabelData
{
public:
    QwtPlot::LabelPosition pos;
};

class BubbleData
{
public:
    int size;
    int edgeWidth;
    float opacity;
};

class PieData
{
public:
    int thickness = 30;
    int startAngle = 90;
    int viewAngle = 45;
    float radiusRatio;
    float displacementRatio;
    bool isCounterClockwise;
};

class GraphLegend: public QwtLegend
{
protected:
    virtual QWidget *createWidget( const QwtLegendData &data ) const
    {
        QWidget *w = QwtLegend::createWidget( data );
//        if ( w )
//        {
//            w->setStyleSheet(
//                "border-radius: 5px;"
//                "padding: 2px;"
//                "background: LemonChiffon;"
//            );
//        }

        return w;
    }
};

class CurveData
{
public:
    bool isAntiAliasing;
};

class ErrorBarData
{
public:
    bool isPlusVisible;
    bool isMinusVisible;
    QColor color;
    int width;
    float opacity;
};

Graph::Graph(int type, QWidget *parent): QwtPlot(parent), m_type(type), m_colorIndex(0)
{
    init();
}

Graph::Graph(QWidget *parent): QwtPlot(parent), m_colorIndex(0)
{
    init();
}

Graph::~Graph()
{
    delete m_gridData;
    delete m_legendData;
    delete m_symbolData;
    delete m_droplineData;
    delete m_curveLabelData;
    delete m_barChartLabelData;
    delete m_bubbleData;
#ifdef RESERVE
    delete m_pieData;
#endif
    delete m_curveData;
}

void Graph::init()
{
    initSettings();
    initGrid();
    initLegend();
    initSymbol();
    initDropline();
    initCurveLabel();
    initBarChartLabel();
    initBubble();
    initPie();
    initCurve();
    initProperty();
}

/*!
 * \brief 初始化网格数据.
 */
void Graph::initGrid()
{
    m_gridData = new GridData;
    m_grid = new QwtPlotGrid;
    m_grid->enableXMin(m_gridData->majorVisible);
    m_grid->enableYMin(true);
    m_grid->setMajorPen(Qt::gray, 0, Qt::DotLine);
    m_grid->setMinorPen(Qt::darkGray, 0, Qt::DotLine);
}

/*!
 * \brief 初始化图例数据.
 */
void Graph::initLegend()
{
    m_legendData = new LegendData;
}

/*!
 * \brief 初始化符号数据.
 */
void Graph::initSymbol()
{
    m_symbolData = new SymbolData;
}

/*!
 * \brief 初始化垂线数据.
 */
void Graph::initDropline()
{
    m_droplineData = new DroplineData;
}

/*!
 * \brief 初始化曲线标签数据.
 */
void Graph::initCurveLabel()
{
    m_curveLabelData = new CurveLabelData;
}

/*!
 * \brief 初始化柱状图标签数据.
 */
void Graph::initBarChartLabel()
{
    m_barChartLabelData = new BarChartLabelData;
}

/*!
 * \brief 初始化冒泡图数据.
 */
void Graph::initBubble()
{
    m_bubbleData = new BubbleData;
}

/*!
 * \brief 初始化饼状图数据.
 */
void Graph::initPie()
{
#ifdef RESERVE
    m_pieData = new PieData;
#endif
}

/*!
 * \brief 初始化曲线数据.
 */
void Graph::initCurve()
{
    m_curveData = new CurveData;
}

/*!
 * \brief 初始化图形属性.
 */
void Graph::initProperty()
{
    GraphProperty gp;
    gp.setTitleText(title().text());
    gp.readSettings();
    gp.setGraph(this);
    m_property = gp;
}

void Graph::initSettings()
{
    setObjectName(graphName());    
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setFrameStyle(QFrame::Box | QFrame::Sunken);
    setCanvas(canvas);
    insertLegend(new GraphLegend());
    setAutoReplot(false);

    connect(this, SIGNAL(itemAttached(QwtPlotItem*,bool)), this, SLOT(onItemAttached(QwtPlotItem*,bool)));
}

void Graph::resetColorIndex()
{
    m_colorIndex = 0;
}

void Graph::setUseCustomSymbol(bool use)
{
    m_useCustomSymbol = use;
}

bool Graph::useCustomSymbol() const
{
    return m_useCustomSymbol;
}

void Graph::setUseOriginalCurveColor(bool use)
{
    m_useOriginalCurveColor = use;
}

bool Graph::isUseOriginalCurveColor() const
{
    return m_useOriginalCurveColor;
}

void Graph::configGrid()
{
    if (m_grid) {
        m_grid->enableX(m_gridData->verticalVisible && m_gridData->majorVisible);
        m_grid->enableY(m_gridData->horizontalVisible && m_gridData->majorVisible);
        m_grid->enableXMin(m_gridData->minorVisible);
        m_grid->enableYMin(m_gridData->minorVisible);
        m_grid->setPen(m_gridData->gridPen.color(), m_gridData->gridPen.width(),
                       m_gridData->gridPen.style());
    }
}

void Graph::configLegend()
{
    if (!m_legendData->legendVisible) {
        insertLegend(nullptr);
    } else {
        if (m_legendData->isDirty) {
            insertLegend(new GraphLegend(), QwtPlot::LegendPosition(m_legendData->legendPosition));
            legend()->setFont(m_legendData->legendFont);
            QPalette pal = legend()->palette();
            pal.setColor(QPalette::Active, QPalette::Text, m_legendData->legendColor);
            legend()->setPalette(pal);
            m_legendData->isDirty = false;
        }
    }

    if (m_legendData->legendItemVisible) {
        if (m_legendData->legendItem == nullptr) {
            m_legendData->legendItem = new QwtPlotLegendItem;
            m_legendData->legendItem->attach(this);
        }

        m_legendData->legendItem->show();
        m_legendData->legendItem->setMaxColumns(m_legendData->legendItemColumns);
        m_legendData->legendItem->setBackgroundMode(QwtPlotLegendItem::BackgroundMode(m_legendData->legendItemBgMode));
        m_legendData->legendItem->setFont(m_legendData->legendItemFont);
        m_legendData->legendItem->setBorderPen(m_legendData->legendItemBorderColor);
        m_legendData->legendItem->setBackgroundBrush(m_legendData->legendItemBgColor);
        m_legendData->legendItem->setTextPen(m_legendData->legendItemColor);
        m_legendData->legendItem->setAlignment(Qt::Alignment(m_legendData->legendItemHAlign | m_legendData->legendItemVAlign));
    } else {
        if (m_legendData->legendItem)
            m_legendData->legendItem->hide();
    }
}

void Graph::configSymbol()
{
    if (m_useCustomSymbol)
        return;

    QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);
    foreach (QwtPlotItem* plotItem, curves) {
        QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(plotItem);

        QwtSymbol* symbol = new QwtSymbol(m_symbolData->style);
        symbol->setSize(m_symbolData->size);
        symbol->setPen(m_symbolData->edgeColor, m_symbolData->width);
        symbol->setBrush(m_symbolData->color);
        symbol->setOpacity(m_symbolData->opacity);
        curve->setSymbol(symbol);
    }
}

void Graph::configDropline()
{
    QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);
    foreach (QwtPlotItem* plotItem, curves) {
        QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(plotItem);
        curve->setHorizontalDroplineVisible(m_droplineData->horizontalVisible);
        curve->setVerticalDroplineVisible(m_droplineData->verticalVisible);
        curve->setDroplinePen(m_droplineData->pen);
        curve->setDroplineOpacity(m_droplineData->opacity);
    }
}

void Graph::configGroupLineColors()
{
    if (m_useOriginalCurveColor)
        return;

    QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);
    if (!m_useGroupLineColor) {
        m_groupLineColors.clear();
        resetColorIndex();
        for (int i = 0; i < curves.size(); ++i) {
            QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(curves.at(i));
            QPen pen = curve->pen();
            pen.setColor(randomColor());
            curve->setPen(pen);
        }
    } else {
        if (m_groupLineColors.isEmpty())
            return;

        for (int i = 0; i < curves.size(); ++i) {
            QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(curves.at(i));
            QPen pen = curve->pen();
            pen.setColor(m_groupLineColors.at(i % m_groupLineColors.size()));
            curve->setPen(pen);
        }
    }
}

void Graph::configGroupPenStyles()
{
    QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);

    if (m_useGroupPenStyle && !m_groupPenStyles.isEmpty()) {
        for (int i = 0; i < curves.size(); ++i) {
            QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(curves.at(i));
            QPen pen = curve->pen();
            pen.setStyle(m_groupPenStyles.at(i % m_groupPenStyles.size()));
            curve->setPen(pen);
        }
    } else {
        for (int i = 0; i < curves.size(); ++i) {
            QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(curves.at(i));
            QPen pen = curve->pen();
            pen.setStyle(Qt::SolidLine);
            curve->setPen(pen);
        }
    }
}

void Graph::configCurveLabel()
{
    QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);

    for (int i = 0; i < curves.size(); ++i) {
        QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(curves.at(i));
        curve->setLabelVisible(m_curveLabelData->isVisible);
        curve->setLabelColor(m_curveLabelData->color);
        curve->setLabelOpacity(m_curveLabelData->opacity);
        curve->setLabelOffsetX(m_curveLabelData->xOffset);
        curve->setLabelOffsetY(m_curveLabelData->yOffset);
        curve->setLabelFont(m_curveLabelData->font);
        curve->setLabelPosition(m_curveLabelData->pos);
    }
}

void Graph::configAxisTitles()
{
    m_axisTitleMap.clear();
    for (int i = 0; i < m_columnData.size(); ++i) {
        ComData::ColumnBase* col = m_columnData.at(i).data();
        if (col && col->significance() == ComData::ColumnBase::X)
            getAxisTitle(col, QwtPlot::xBottom);
        else if (col && col->significance() == ComData::ColumnBase::Y)
            getAxisTitle(col, QwtPlot::yLeft);
    }

    for (int i = 0; i < QwtPlot::axisCnt; ++i)
        axisWidget(i)->setTitle(m_axisTitleMap.value((QwtPlot::Axis)i));
}

void Graph::configCurve()
{
    QwtPlotItemList curves = itemList(QwtPlotItem::Rtti_PlotCurve);
    foreach (QwtPlotItem* item, curves) {
        QwtPlotCurve* curve = static_cast<QwtPlotCurve*>(item);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, m_curveData->isAntiAliasing);
    }
}

void Graph::getAxisTitle(const ComData::ColumnBase *column, QwtPlot::Axis axis)
{
    switch (m_axisTitleType) {
    case LongName:
        m_axisTitleMap.insert(axis, column->longName());
        break;

    case ShortName:
        m_axisTitleMap.insert(axis, column->name());
        break;

    case Unit:
        m_axisTitleMap.insert(axis, column->unit());
        break;

    case LongNamePlusUnit:
        m_axisTitleMap.insert(axis, QString("%1 %2").arg(column->longName())
                              .arg(column->unit()));
        break;

    case ShortNamePlusUnit:
        m_axisTitleMap.insert(axis, QString("%1 %2").arg(column->name())
                              .arg(column->unit()));
        break;
    }
}

void Graph::setPrecisionByMinValue(qreal min)
{
    const QString strMin = QString::number(min);
    if (strMin.contains(".")) {
        int precision = strMin.length() - strMin.lastIndexOf(".") - 1;
        // it seemed like qwt only supports precision up to 2
        if (precision > 2)
            precision = 2;
        setDecimalWidth(precision);
        m_property.setDecimalWidth(precision);
    }
}

QString Graph::graphName()
{
    return GraphName;
}

bool Graph::isGraph(const QWidget *widget)
{
    return widget->objectName() == GraphName;
}

Graph* Graph::create(int type, const QList<QSharedPointer<ComData::ColumnBase> > &columnData)
{
    Graph* g = nullptr;
    switch (type)
    {
    case GT_Area:
        g = new AreaGraph(AreaGraph::Area);
        break;

    case GT_StackArea:
        g = new AreaGraph(AreaGraph::StackArea);
        break;

    case GT_FillArea:
        g = new AreaGraph(AreaGraph::FillArea);
        break;

    case GT_Column:
        g = new BarChartGraph(BarChartGraph::Column);
        break;

    case GT_ColumnPlusLabel:
        g = new BarChartGraph(BarChartGraph::ColumnPlusLabel);
        break;

    case GT_Bar:
        g = new BarChartGraph(BarChartGraph::Bar);
        break;

    case GT_BarPlusLabel:
        g = new BarChartGraph(BarChartGraph::BarPlusLabel);
        break;

    case GT_StackColumn:
        g = new MultiBarChartGraph(MultiBarChartGraph::StackColumn);
        break;

    case GT_StackBar:
        g = new MultiBarChartGraph(MultiBarChartGraph::StackBar);
        break;

    case GT_FloatingColumn:
        g = new MultiBarChartGraph(MultiBarChartGraph::FloatingColumn);
        break;

    case GT_FloatingBar:
        g = new MultiBarChartGraph(MultiBarChartGraph::FloatingBar);
        break;

    case GT_Bubble:
        g = new BubbleGraph();
        break;

    case GT_Line:
        g = new CurveGraph(CurveGraph::Line);
        break;

    case GT_HorizontalStep:
        g = new CurveGraph(CurveGraph::HorizontalStep);
        break;

    case GT_VerticalStep:
        g = new CurveGraph(CurveGraph::VerticalStep);
        break;

    case GT_Stick:
        g = new CurveGraph(CurveGraph::Stick);
        break;

    case GT_SplineConnected:
        g = new CurveGraph(CurveGraph::SplineConnected);
        break;

//    case GT_Histogram:
//        g = new HistogramGraph();
//        break;

    case GT_Pie:
        g = new Pie2DGraph();
        break;
    }

    if (g) {
        g->setData(columnData);
        g->initialize();
    }

    return g;
}

/*!
 * \brief 生成数据表的图形截图.
 * \param table 数据表
 * \param columns 列索引
 * \param output 输出文件名
 * \param type 图形类型
 * \param size 图形尺寸
 * \param format 输出文件类型, 支出pdf, svg, png等格式
 * \return 是否成功输出文件
 */
bool Graph::snapshot(ComData::Table* table, const QList<int>& columns,
                     const QString& output, const QString& title,
                     GraphType type, const QSize& size)
{
    if (!table)
        return false;

    QList<QSharedPointer<ComData::ColumnBase> > columnData;
    foreach (auto col, columns)
        columnData.append(table->at(col));

    Graph* g = Graph::create(type, columnData);
    if (!title.isEmpty())
        g->setTitle(title);
    QString format;
    QFileInfo fi(output);
    if (!fi.suffix().isEmpty()) {
        format = fi.suffix();
    } else {
        format = "png";
        fi.setFile(fi.absoluteFilePath() + ".png");
    }

    QwtPlotRenderer renderer;
    renderer.renderDocument(g, fi.absoluteFilePath(), format, size);
    delete g;
    return true;
}

/*!
 * \brief 重绘图形.
 */
void Graph::replot()
{
    configGrid();
    configLegend();
    configSymbol();
    configDropline();
    configGroupLineColors();
    configGroupPenStyles();
    configCurveLabel();
    configAxisTitles();
    configCurve();

    customize();
    QwtPlot::replot();
}

/*!
 * \brief 设置轴标题类型.
 * \param type 类型
 */
void Graph::setAxisTitleType(AxisTitleType type)
{
    m_axisTitleType = type;
}

/*!
 * \brief 获取轴标题类型.
 */
Graph::AxisTitleType Graph::axisTitleType() const
{
    return m_axisTitleType;
}

/*!
 * \brief 设置柱状图的标签是否可见.
 * \param visible 是否可见
 */
void Graph::setLabelVisible(bool visible)
{
    m_barChartLabelData->isVisible = visible;
}

/*!
 * \brief 获取柱状图的标签是否可见.
 * \return
 */
bool Graph::isLabelVisible() const
{
    return m_barChartLabelData->isVisible;
}

/*!
 * \brief 设置柱状图标签的位置.
 * \param lp 标签位置
 */
void Graph::setLabelPosition(LabelPosition lp)
{
    m_barChartLabelData->pos = lp;
}

/*!
 * \brief 获取标签位置.
 * \return 标签位置
 */
Graph::LabelPosition Graph::labelPosition() const
{
    return m_barChartLabelData->pos;
}

/*!
 * \brief 设置柱状图标签的X偏移量.
 * \param x 偏移量
 */
void Graph::setLabelOffsetX(int x)
{
    m_barChartLabelData->xOffset = x;
}

/*!
 * \brief 获取柱状图标签的X偏移量.
 * \return X偏移量
 */
int Graph::labelOffsetX() const
{
    return m_barChartLabelData->xOffset;
}

/*!
 * \brief 设置柱状图标签的Y偏移量.
 * \param y 偏移量
 */
void Graph::setLabelOffsetY(int y)
{
    m_barChartLabelData->yOffset = y;
}

/*!
 * \brief 获取柱状图标签的Y偏移量.
 * \return
 */
int Graph::labelOffsetY() const
{
    return m_barChartLabelData->yOffset;
}

/*!
 * \brief 设置柱状图的字体.
 * \param f 字体
 */
void Graph::setLabelFont(const QFont& f)
{
    m_barChartLabelData->font = f;
}

/*!
 * \brief 获取柱状图的字体.
 * \return 字体
 */
QFont Graph::labelFont() const
{
    return m_barChartLabelData->font;
}

/*!
 * \brief 设置柱状图标签的透明度.
 * \param opacity 透明度 (0.0 ~ 1.0)
 */
void Graph::setLabelOpacity(float opacity)
{
    m_barChartLabelData->opacity = opacity;
}

/*!
 * \brief 获取柱状图标签的透明度.
 * \return 透明度 (0.0 ~ 1.0)
 */
float Graph::labelOpacity() const
{
    return m_barChartLabelData->opacity;
}

/*!
 * \brief 设置柱状图标签的颜色.
 * \param clr 颜色
 */
void Graph::setLabelColor(const QColor& clr)
{
    m_barChartLabelData->color = clr;
}

/*!
 * \brief 获取柱状图标签颜色.
 * \return 颜色
 */
QColor Graph::labelColor() const
{
    return m_barChartLabelData->color;
}

/*!
 * \brief 设置网格是否可见.
 * \param visible 是否可见
 */
void Graph::setGridVisible(bool visible)
{
    if (!m_gridData)
        return;

    if (m_gridData->visible == visible)
        return;

    m_gridData->visible = visible;
    if (!visible && m_grid) {
        m_grid->detach();
    } else {
        if (m_grid) {
            m_grid->attach(this);
        } else {
            initGrid();
            m_grid->attach(this);
        }
    }
}

/*!
 * \brief 获取网格是否可见.
 * \return 是否可见
 */
bool Graph::isGridVisible() const
{
    if (!m_gridData)
        return false;

    return m_gridData->visible;
}

/*!
 * \brief 设置主要网格是否可见.
 * \param visible 是否可见
 */
void Graph::setMajorGridVisible(bool visible)
{
    if (!m_gridData)
        return;

    m_gridData->majorVisible = visible;
}

/*!
 * \brief 获取主要网格是否可见.
 * \return 是否可见
 */
bool Graph::isMajorGridVisible() const
{
    if (!m_gridData)
        return false;

    return m_gridData->majorVisible;
}

/*!
 * \brief 设置次要网格是否可见.
 * \param visible 是否可见
 */
void Graph::setMinorGridVisible(bool visible)
{
    if (!m_gridData)
        return;

    m_gridData->minorVisible = visible;
}

/*!
 * \brief 获取次要网格是否可见.
 */
bool Graph::isMinorGridVisible() const
{
    if (!m_gridData)
        return false;

    return m_gridData->minorVisible;
}

/*!
 * \brief 设置网格线型
 * \param ps 线型
 */
void Graph::setGridPenStyle(Qt::PenStyle ps)
{
    if (!m_gridData)
        return;

    m_gridData->gridPen.setStyle(ps);
}

/*!
 * \brief 获取网格线型.
 */
Qt::PenStyle Graph::gridPenStyle() const
{
    if (!m_gridData)
        return Qt::SolidLine;

    return m_gridData->gridPen.style();
}

/*!
 * \brief 设置网格线颜色.
 * \param clr 颜色
 */
void Graph::setGridPenColor(const QColor& clr)
{
    if (!m_gridData)
        return;

    m_gridData->gridPen.setColor(clr);
}

/*!
 * \brief 获取网格线颜色.
 * \return 颜色
 */
QColor Graph::gridPenColor() const
{
    if (!m_gridData)
        return Qt::black;

    return m_gridData->gridPen.color();
}

/*!
 * \brief 设置网格线宽.
 * \param width 线宽
 */
void Graph::setGridPenWidth(int width)
{
    if (!m_gridData)
        return;

    m_gridData->gridPen.setWidth(width);
}

/*!
 * \brief 获取网格线宽.
 * \return 线宽
 */
int Graph::gridPenWidth() const
{
    if (!m_gridData)
        return 1;

    return m_gridData->gridPen.width();
}

/*!
 * \brief 设置水平网格线是否可见.
 * \param visible 是否可见
 */
void Graph::setHorizontalGridVisible(bool visible)
{
    if (!m_gridData)
        return;

    m_gridData->horizontalVisible = visible;
}

/*!
 * \brief 获取水平网格线是否可见.
 * \return 是否可见
 */
bool Graph::isHorizontalGridVisible() const
{
    if (!m_gridData)
        return true;

    return m_gridData->horizontalVisible;
}

/*!
 * \brief 设置竖直网格线是否可见.
 * \param visible 是否可见
 */
void Graph::setVerticalGridVisible(bool visible)
{
    if (!m_gridData)
        return;

    m_gridData->verticalVisible = visible;
}

/*!
 * \brief 获取竖直网格线是否可见.
 * \return 是否可见
 */
bool Graph::isVerticalGridVisible() const
{
    if (!m_gridData)
        return true;

    return m_gridData->verticalVisible;
}

/*!
 * \brief 设置图例字体.
 * \param f 字体
 */
void Graph::setLegendFont(const QFont& f)
{
    m_legendData->makeDirty();
    m_legendData->legendFont = f;
}

/*!
 * \brief 获取图例字体.
 * \return 字体
 */
QFont Graph::legendFont() const
{
    return m_legendData->legendFont;
}

/*!
 * \brief 设置图例位置.
 * \param pos 位置
 */
void Graph::setLegendPosition(int pos)
{
    m_legendData->makeDirty();
    m_legendData->legendPosition = pos;
}

/*!
 * \brief 获取图例位置.
 * \return 位置
 */
int Graph::legendPosition() const
{
    return m_legendData->legendPosition;
}

/*!
 * \brief 设置图例字体颜色.
 * \param clr 颜色
 */
void Graph::setLegendFontColor(const QColor& clr)
{
    m_legendData->makeDirty();
    m_legendData->legendColor = clr;
}

/*!
 * \brief 获取图例字体颜色
 * \return 颜色
 */
QColor Graph::legendFontColor() const
{
    return m_legendData->legendColor;
}

/*!
 * \brief 设置图例是否可见.
 * \param visible 是否可见
 */
void Graph::setLegendVisible(bool visible)
{
    m_legendData->legendVisible = visible;
}

/*!
 * \brief 获取图例是否可见.
 * \return 是否可见
 */
bool Graph::isLegendVisible() const
{
    return m_legendData->legendVisible;
}

// Legend Item
/*!
 * \brief 设置图例项是否可见, 图理项是位于图形区域的图例.
 * \param visible 是否可见
 */
void Graph::setLegendItemVisible(bool visible)
{
    m_legendData->legendItemVisible = visible;
}

/*!
 * \brief 获取图例项是否可见.
 * \return 是否可见
 */
bool Graph::isLegendItemVisible() const
{
    return m_legendData->legendItemVisible;
}

/*!
 * \brief 设置图例项字体.
 * \param f 字体
 */
void Graph::setLegendItemFont(const QFont& f)
{
    m_legendData->legendItemFont = f;
}

/*!
 * \brief 获取图例项字体.
 * \return 字体
 */
QFont Graph::legendItemFont() const
{
    return m_legendData->legendItemFont;
}

/*!
 * \brief 设置图例项背景颜色.
 * \param clr 背景颜色
 */
void Graph::setLegendItemBgColor(const QColor& clr)
{
    m_legendData->legendItemBgColor = clr;
}

/*!
 * \brief 获取图例项背景颜色.
 * \return 背景颜色
 */
QColor Graph::legendItemBgColor() const
{
    return m_legendData->legendItemBgColor;
}

/*!
 * \brief 设置图例项字体颜色.
 * \param clr 颜色
 */
void Graph::setLegendItemFontColor(const QColor& clr)
{
    m_legendData->legendItemColor = clr;
}

/*!
 * \brief 获取图立项背景字体颜色.
 * \return 颜色
 */
QColor Graph::legendItemFontColor() const
{
    return m_legendData->legendItemColor;
}

/*!
 * \brief 设置图例项边框颜色.
 * \param clr 边框颜色
 */
void Graph::setLegendItemBorderColor(const QColor& clr)
{
    m_legendData->legendItemBorderColor = clr;
}

/*!
 * \brief 获取图例项边框颜色.
 * \return 边框颜色
 */
QColor Graph::legendItemBorderColor() const
{
    return m_legendData->legendItemBorderColor;
}

/*!
 * \brief 设置图例项列数.
 * \param cols 列数
 */
void Graph::setLegendItemColumns(int cols)
{
    m_legendData->legendItemColumns = cols;
}

/*!
 * \brief 获取图例项列数.
 * \return 列数
 */
int Graph::legendItemColumns() const
{
    return m_legendData->legendItemColumns;
}

/*!
 * \brief 设置图例项横向对齐.
 * \param align 对齐方式
 */
void Graph::setLegendItemHAlign(int align)
{
    m_legendData->legendItemHAlign = align;
}

/*!
 * \brief 获取图例项横向对齐方式.
 * \return 对齐方式
 */
int Graph::legendItemHAlign() const
{
    return m_legendData->legendItemHAlign;
}

/*!
 * \brief 设置图例项竖直对齐方式.
 * \param align 对齐方式
 */
void Graph::setLegendItemVAlign(int align)
{
    m_legendData->legendItemVAlign = align;
}

/*!
 * \brief 获取图例项竖直对齐方式.
 * \return 对齐方式
 */
int Graph::legendItemVAlign() const
{
    return m_legendData->legendItemVAlign;
}

/*!
 * \brief 设置图例项背景模式.
 * \param mode 背景模式
 */
void Graph::setLegendItemBgMode(int mode)
{
    m_legendData->legendItemBgMode = mode;
}

/*!
 * \brief 设置图例项背景模式.
 * \return 背景模式
 */
int Graph::legendItemBgMode() const
{
    return m_legendData->legendItemBgMode;
}

/*!
 * \brief 设置曲线符号风格.
 * \param style 风格
 */
void Graph::setSymbol(QwtSymbol::Style style)
{
    m_symbolData->style = style;
}

/*!
 * \brief 获取曲线符号风格.
 * \return 符号风格
 */
QwtSymbol::Style Graph::symbol() const
{
    return m_symbolData->style;
}

/*!
 * \brief 设置曲线符号尺寸.
 * \param size 符号尺寸
 */
void Graph::setSymbolSize(int size)
{
    m_symbolData->size = size;
}

/*!
 * \brief 获取曲线符号尺寸.
 * \return 符号尺寸
 */
int Graph::symbolSize() const
{
    return m_symbolData->size;
}

/*!
 * \brief 设置曲线符号宽度.
 * \param width 宽度
 */
void Graph::setSymbolWidth(int width)
{
    m_symbolData->width = width;
}

/*!
 * \brief 获取曲线符号宽度.
 * \return 宽度
 */
int Graph::symbolWidth() const
{
    return m_symbolData->width;
}

/*!
 * \brief 设置曲线符号透明度.
 * \param opacity 透明度(0.0 ~ 1.0)
 */
void Graph::setSymbolOpacity(float opacity)
{
    m_symbolData->opacity = opacity;
}

/*!
 * \brief 获取曲线符号透明度.
 * \return 透明度
 */
float Graph::symbolOpacity() const
{
    return m_symbolData->opacity;
}

/*!
 * \brief 设置曲线符号颜色.
 * \param color 颜色
 */
void Graph::setSymbolColor(const QColor& color)
{
    m_symbolData->color = color;
}

/*!
 * \brief 获取曲线符号颜色.
 * \return 颜色
 */
QColor Graph::symbolColor() const
{
    return m_symbolData->color;
}

/*!
 * \brief 设置曲线符号边的颜色.
 * \param clr 颜色
 */
void Graph::setSymbolEdgeColor(const QColor& clr)
{
    m_symbolData->edgeColor = clr;
}

/*!
 * \brief 获取符号边颜色.
 * \return 颜色
 */
QColor Graph::symbolEdgeColor() const
{
    return m_symbolData->edgeColor;
}

/*!
 * \brief 设置水平垂线是否可见.
 * \param visible 是否可见
 */
void Graph::setHorizontalDroplineVisible(bool visible)
{
    m_droplineData->horizontalVisible = visible;
}

/*!
 * \brief 获取水平垂线是否可见.
 * \return 是否可见
 */
bool Graph::isHorizontalDroplineVisible() const
{
    return m_droplineData->horizontalVisible;
}

/*!
 * \brief 设置竖直垂线是否可见.
 * \param visible 是否可见
 */
void Graph::setVerticalDroplineVisible(bool visible)
{
    m_droplineData->verticalVisible = visible;
}

/*!
 * \brief 获取竖直垂线是否可见.
 * \return 是否可见
 */
bool Graph::isVerticalDroplineVisible() const
{
    return m_droplineData->verticalVisible;
}

/*!
 * \brief 设置垂线画笔.
 * \param pen 画笔
 */
void Graph::setDroplinePen(const QPen& pen)
{
    m_droplineData->pen = pen;
}

/*!
 * \brief 获取垂线画笔.
 * \return 画笔
 */
QPen Graph::droplinePen() const
{
    return m_droplineData->pen;
}

/*!
 * \brief 设置垂线颜色.
 * \param clr 颜色
 */
void Graph::setDroplineColor(const QColor& clr)
{
    m_droplineData->pen.setColor(clr);
}

/*!
 * \brief 获取垂线颜色.
 * \return 颜色
 */
QColor Graph::droplineColor() const
{
    return m_droplineData->pen.color();
}

/*!
 * \brief 设置垂线透明度.
 * \param opacity 透明度
 */
void Graph::setDroplineOpacity(float opacity)
{
    m_droplineData->opacity = opacity;
}

/*!
 * \brief 获取垂线透明度.
 * \return
 */
float Graph::droplineOpacity() const
{
    return m_droplineData->opacity;
}

/*!
 * \brief 设置是否使用多项线颜色.
 * \param use 是否使用
 */
void Graph::setUseGroupLineColor(bool use)
{
    m_useGroupLineColor = use;
    replot();
}

/*!
 * \brief 获取是否使用多项线颜色.
 */
bool Graph::useGroupLineColor() const
{
    return m_useGroupLineColor;
}

/*!
 * \brief 设置是否使用多项填充色.
 * \param use 是否使用
 */
void Graph::setUseGroupPatternColor(bool use)
{
    m_useGroupPatternColor = use;
    replot();
}

/*!
 * \brief 获取是否使用多项填充色.
 */
bool Graph::useGroupPatternColor() const
{
    return m_useGroupPatternColor;
}

/*!
 * \brief 设置是否使用多项画笔.
 * \param use 是否使用
 */
void Graph::setUseGroupPenStyle(bool use)
{
    m_useGroupPenStyle = use;
    replot();
}

/*!
 * \brief 获取是否使用多项画笔.
 */
bool Graph::useGroupPenStyle() const
{
    return m_useGroupPenStyle;
}

/*!
 * \brief 设置冒泡图是否使用多项风格.
 * \param use 是否使用
 */
void Graph::setUseGroupBubbleStyle(bool use)
{
    m_useGroupBubbleStyle = use;

    if (!use)
        m_groupBubbleStyles.clear();
    replot();
}

/*!
 * \brief 获取冒泡图是否使用多项风格.
 */
bool Graph::useGroupBubbleStyle() const
{
    return m_useGroupBubbleStyle;
}

/*!
 * \brief 设置多项线颜色.
 * \param clrs 颜色列表
 */
void Graph::setGroupLineColors(const QList<QColor>& clrs)
{
    m_groupLineColors = clrs;
}

/*!
 * \brief 获取多项线颜色列表.
 */
QList<QColor> Graph::groupLineColors() const
{
    return m_groupLineColors;
}

/*!
 * \brief 设置多项填充色.
 * \param clrs 颜色列表
 */
void Graph::setGroupPatternColors(const QList<QColor>& clrs)
{
    m_groupPatternColors = clrs;
}

/*!
 * \brief 获取多项填充色列表.
 */
QList<QColor> Graph::groupPatternColors() const
{
    return m_groupPatternColors;
}

/*!
 * \brief 设置多项线型.
 * \param styles 线型列表.
 */
void Graph::setGroupPenStyle(const QList<Qt::PenStyle> styles)
{
    m_groupPenStyles = styles;
}

/*!
 * \brief 获取多项线型列表.
 */
QList<Qt::PenStyle> Graph::groupPenStyle() const
{
    return m_groupPenStyles;
}

/*!
 * \brief 设置冒泡图多项样式.
 * \param styles 样式列表
 */
void Graph::setGroupBubbleStyle(const QList<QwtSymbol::Style>& styles)
{
    m_groupBubbleStyles = styles;
}

/*!
 * \brief 设置冒泡图多项样式.
 */
QList<QwtSymbol::Style> Graph::groupBubbleStyle() const
{
    return m_groupBubbleStyles;
}

/*!
 * \brief 设置曲线标签是否可见.
 * \param visible 是否可见
 */
void Graph::setCurveLabelVisible(bool visible)
{
    m_curveLabelData->isVisible = visible;
}

/*!
 * \brief 获取曲线标签是否可见.
 * \return 是否可见
 */
bool Graph::isCurveLabelVisible() const
{
    return m_curveLabelData->isVisible;
}

/*!
 * \brief 设置曲线标签字体.
 * \param f 字体
 */
void Graph::setCurveLabelFont(const QFont& f)
{
    m_curveLabelData->font = f;
}

/*!
 * \brief 获取曲线标签字体.
 * \return 字体
 */
QFont Graph::curveLabelFont() const
{
    return m_curveLabelData->font;
}

/*!
 * \brief 获取曲线标签X偏移量.
 * \param offset 偏移量
 */
void Graph::setCurveLabelOffsetX(int offset)
{
    m_curveLabelData->xOffset = offset;
}

/*!
 * \brief 获取曲线标签X偏移量.
 * \return 偏移量
 */
int Graph::curveLabelOffsetX() const
{
    return m_curveLabelData->xOffset;
}

/*!
 * \brief 设置曲线标签Y偏移量.
 * \param offset 偏移量
 */
void Graph::setCurveLabelOffsetY(int offset)
{
    m_curveLabelData->yOffset = offset;
}

/*!
 * \brief 获取曲线标签Y偏移量.
 * \return 偏移量
 */
int Graph::curveLabelOffsetY() const
{
    return m_curveLabelData->yOffset;
}

/*!
 * \brief 设置曲线标签位置.
 * \param pos 位置
 */
void Graph::setCurveLabelPosition(QwtPlotCurve::LabelPosition pos)
{
    m_curveLabelData->pos = pos;
}

/*!
 * \brief 获取曲线标签位置.
 */
QwtPlotCurve::LabelPosition Graph::curveLabelPosition() const
{
    return m_curveLabelData->pos;
}

/*!
 * \brief 设置曲线标签透明度.
 * \param opacity 透明度
 */
void Graph::setCurveLabelOpacity(float opacity)
{
    m_curveLabelData->opacity = opacity;
}

/*!
 * \brief 获取曲线标签透明度.
 */
float Graph::curveLabelOpacity() const
{
    return m_curveLabelData->opacity;
}

/*!
 * \brief 设置曲线标签颜色.
 * \param clr 颜色
 */
void Graph::setCurveLabelColor(const QColor& clr)
{
    m_curveLabelData->color = clr;
}

/*!
 * \brief 获取曲线标签颜色.
 */
QColor Graph::curveLabelColor() const
{
    return m_curveLabelData->color;
}

/*!
 * \brief 设置冒泡图符号大小.
 * \param size 大小
 */
void Graph::setBubbleSize(int size)
{
    m_bubbleData->size = size;
}

/*!
 * \brief 设置冒泡图符号大小.
 */
int Graph::bubbleSize() const
{
    return m_bubbleData->size;
}

/*!
 * \brief 设置冒泡图边的宽度.
 * \param width 宽度
 */
void Graph::setBubbleEdgeWidth(int width)
{
    m_bubbleData->edgeWidth = width;
}

/*!
 * \brief 获取冒泡图边宽度.
 */
int Graph::bubbleEdgeWidth() const
{
    return m_bubbleData->edgeWidth;
}

/*!
 * \brief 设置冒泡图符号的透明度.
 * \param opacity 透明度
 */
void Graph::setBubbleOpacity(float opacity)
{
    m_bubbleData->opacity = opacity;
}

/*!
 * \brief 获取冒泡图符号透明度.
 */
float Graph::bubbleOpacity() const
{
    return m_bubbleData->opacity;
}

void Graph::setPieThickness(int thickness)
{
#ifdef RESERVE
    m_pieData->thickness = thickness;
#endif
}

int Graph::pieThickness() const
{
#ifdef RESERVE
    return m_pieData->thickness;
#else
    return 0;
#endif
}

void Graph::setPieStartAngle(int startAngle)
{
#ifdef RESERVE
    m_pieData->startAngle = startAngle;
#endif
}

int Graph::pieStartAngle()
{
#ifdef RESERVE
    return m_pieData->startAngle;
#else
    return 0;
#endif
}

void Graph::setPieCounterClockwise(bool counter)
{
#ifdef RESERVE
    m_pieData->isCounterClockwise = counter;
#endif
}

bool Graph::isPieCounterClockwise() const
{
#ifdef RESERVE
    return m_pieData->isCounterClockwise;
#else
    return false;
#endif
}

void Graph::setPieViewAngle(int angle)
{
#ifdef RESERVE
    m_pieData->viewAngle = angle;
#endif
}

int Graph::pieViewAngle() const
{
#ifdef RESERVE
    return m_pieData->viewAngle;
#else
    return 0;
#endif
}

void Graph::setPieRadiusRatio(float ratio)
{
#ifdef RESERVE
    m_pieData->radiusRatio = ratio;
#endif
}

float Graph::pieRadiusRatio() const
{
#ifdef RESERVE
    return m_pieData->radiusRatio;
#else
    return 0;
#endif
}

void Graph::setPieDisplacementRatio(float ratio)
{
#ifdef RESERVE
    m_pieData->displacementRatio = ratio;
#endif
}

float Graph::pieDisplacementRatio() const
{
#ifdef RESERVE
    return m_pieData->displacementRatio;
#else
    return 0;
#endif
}

/*!
 * \brief 设置轴标题.
 * \param axisId 轴的枚举值
 * \param title 标题
 */
void Graph::setAxisTitle(int axisId, const QString &title)
{
    m_axisTitleMap[(QwtPlot::Axis)axisId] = title;
}

/*!
 * \brief 获取轴的挑剔
 * \param axisId 轴的枚举值
 * \return 标题
 */
QString Graph::axisTitle(int axisId) const
{
    return m_axisTitleMap.value((QwtPlot::Axis)axisId);
}

/*!
 * \brief 设置曲线是否使用反锯齿.
 * \param anti 是否使用
 */
void Graph::setAntiAliasingCurve(bool anti)
{
    m_curveData->isAntiAliasing = anti;
}

/*!
 * \brief 获取曲线是否使用了反锯齿设置.
 */
bool Graph::isAntiAliasingCurve() const
{
    return m_curveData->isAntiAliasing;
}

void Graph::onColumnDataChanged()
{
    dataUpdated();
}

void Graph::onPropertyChanged()
{
    // now apply new settings
}

void Graph::onItemAttached(QwtPlotItem *plotItem, bool on)
{
    // noimpl
}

void Graph::setData(const QList<QSharedPointer<ComData::ColumnBase>> &columnData)
{
    m_columnData = columnData;
    for (int i = 0; i < columnData.size(); ++i)
        connect(columnData.at(i).data(), SIGNAL(dataChanged()), this, SLOT(onColumnDataChanged()));
}

void Graph::addColumnData(const QSharedPointer<ComData::ColumnBase> &column)
{
    m_columnData.append(column);
    dataUpdated();
    replot();
}

/*!
 * \brief 导出图形的截图.
 */
void Graph::exportImage()
{
    QString saveImage = QFileDialog::getSaveFileName(this, tr("Save as Image - %1")
                                                     .arg(QApplication::applicationDisplayName()), ".", "PNG Files(*.png)");
    if (saveImage.isEmpty())
        return;

    if (!saveImage.endsWith(".png"))
        saveImage += ".png";

    QPixmap pixmap = grab();
    pixmap.save(saveImage, "PNG");

}

void Graph::setProperty(const GraphProperty &property)
{
    m_property = property;
    m_property.setGraph(this);
}

const GraphProperty& Graph::property() const
{
    return m_property;
}

PlotItemData Graph::column(QwtPlotSeriesItem* item) const
{
    if (m_plotItem2ColumnMap.contains(item))
        qDebug() << __FUNCTION__ << "contains";
    else
        qDebug() << __FUNCTION__ << "not contains";

    PlotItemData pid = m_plotItem2ColumnMap.value(item);
//    QMap<QwtPlotSeriesItem*, PlotItemData>::const_iterator iter = m_plotItem2ColumnMap.begin();
//    while (iter != m_plotItem2ColumnMap.end()) {
//        iter.value().print();
//        ++iter;
//    }

    return pid;
}

void Graph::dataUpdated()
{
    resetColorIndex();
}

void Graph::clear()
{
    // no impl
}

void Graph::customize()
{
    // put some custom code in this method
}

void Graph::addColumn(const PlotItemData& column)
{
    m_columnData.append(column.column);
}

int Graph::type() const
{
    return m_type;
}

QColor Graph::randomColor()
{
    int colorCount = sizeof(DefaultColors) / sizeof(DefaultColors[0]);
    return QColor(DefaultColors[m_colorIndex++ % colorCount]);
}

} // namespace VizCore
