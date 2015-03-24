#ifndef GRAPH_H
#define GRAPH_H

#include "comdatagui_global.h"
#include "graphproperty.h"

#include <comdata/column.h>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_seriesitem.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_symbol.h>

class QwtPlotGrid;

namespace ComData
{
    class Table;
}

namespace ComDataGui
{

class GraphProperty;
class GridData;
class LegendData;
class SymbolData;
class DroplineData;
class CurveLabelData;
class BarChartLabelData;
class BubbleData;
class PieData;
class CurveData;
class ErrorBarData;

enum GraphType
{
    GT_Area,
    GT_StackArea,
    GT_FillArea,
    GT_Column,
    GT_ColumnPlusLabel,
    GT_Bar,
    GT_BarPlusLabel,
    GT_StackColumn,
    GT_StackBar,
    GT_FloatingColumn,
    GT_FloatingBar,
    GT_Bubble,
    GT_Line,
    GT_HorizontalStep,
    GT_VerticalStep,
    GT_Stick,
    GT_SplineConnected,
//    GT_Histogram,
    GT_Pie,
    GT_UserType = 99,
};

class COMDATAGUI_EXPORT PlotItemData
{
public:
    PlotItemData();
    PlotItemData(const PlotItemData& other);
    PlotItemData& operator=(const PlotItemData& other);
    void reset();
    void print() const;

    QSharedPointer<ComData::ColumnBase> column;
    bool isAntialiased;
    QPen pen;
    QwtPlotCurve::CurveStyle curveStyle;
    QwtPlotCurve* curve = nullptr;
};

/*!
 * \class 图形类, 所有图形类都派生自此类.
 */
class COMDATAGUI_EXPORT Graph : public QwtPlot
{
    Q_OBJECT
public:

    enum Position { Window, Title, Legend, Axis, PlotItem };
    enum AxisTitleType { LongName, ShortName, Unit, LongNamePlusUnit, ShortNamePlusUnit };

    explicit Graph(int type, QWidget *parent = nullptr);
    explicit Graph(QWidget* parent = nullptr);

    void setData(const QList<QSharedPointer<ComData::ColumnBase> > &columnData);
    void addColumnData(const QSharedPointer<ComData::ColumnBase>& column);

    void exportImage();

    void setProperty(const GraphProperty& property);
    const GraphProperty& property() const;

    PlotItemData column(QwtPlotSeriesItem* item) const;

    /*!
     * \brief 创建图形的克隆.(附带数据)
     * \return 新图形对象
     */
    virtual Graph* clone() = 0;

    /*!
     * \brief 创建图形的克隆.(不附带数据)
     * \return 新图形对象
     */
    virtual Graph* rawClone() = 0; // without data copying

    /*!
     * \brief 初始化图形.
     */
    virtual void initialize() = 0;

    /*!
     * \brief 当数据发生改变时调用此函数.
     */
    virtual void dataUpdated();

    /*!
     * \brief 清空数据.
     */
    virtual void clear();

    /*!
     * \brief 定制图形, 具体图形需要处理的操作放在这个函数中.
     */
    virtual void customize();

    virtual void addColumn(const PlotItemData& column);
    virtual int type() const;
    QColor randomColor();

    static QString graphName();
    static bool isGraph(const QWidget* widget);

    // throw in the GraphType value as type parameter
    static Graph* create(int type, const QList<QSharedPointer<ComData::ColumnBase> >& columnData);
    static bool snapshot(ComData::Table* table, const QList<int>& columns,
                         const QString& output, const QString& title = QString(), GraphType type = GT_Line, const QSize& size = QSize(120, 80));

public slots:
    virtual void replot();

    void setAxisTitleType(AxisTitleType type);
    AxisTitleType axisTitleType() const;

    void setLabelVisible(bool visible);
    bool isLabelVisible() const;

    void setLabelPosition(LabelPosition lp);
    LabelPosition labelPosition() const;

    void setLabelOffsetX(int x);
    int labelOffsetX() const;

    void setLabelOffsetY(int y);
    int labelOffsetY() const;

    void setLabelFont(const QFont& f);
    QFont labelFont() const;

    void setLabelOpacity(float opacity);
    float labelOpacity() const;

    void setLabelColor(const QColor& clr);
    QColor labelColor() const;

    void setGridVisible(bool visible);
    bool isGridVisible() const;

    void setMajorGridVisible(bool visible);
    bool isMajorGridVisible() const;

    void setMinorGridVisible(bool visible);
    bool isMinorGridVisible() const;

    void setGridPenStyle(Qt::PenStyle ps);
    Qt::PenStyle gridPenStyle() const;

    void setGridPenColor(const QColor& clr);
    QColor gridPenColor() const;

    void setGridPenWidth(int width);
    int gridPenWidth() const;

    void setHorizontalGridVisible(bool visible);
    bool isHorizontalGridVisible() const;

    void setVerticalGridVisible(bool visible);
    bool isVerticalGridVisible() const;

    // Legend
    void setLegendFont(const QFont& f);
    QFont legendFont() const;

    void setLegendPosition(int pos);
    int legendPosition() const;

    void setLegendFontColor(const QColor& clr);
    QColor legendFontColor() const;

    void setLegendVisible(bool visible);
    bool isLegendVisible() const;

    // Legend Item
    void setLegendItemVisible(bool visible);
    bool isLegendItemVisible() const;

    void setLegendItemFont(const QFont& f);
    QFont legendItemFont() const;

    void setLegendItemBgColor(const QColor& clr);
    QColor legendItemBgColor() const;

    void setLegendItemFontColor(const QColor& clr);
    QColor legendItemFontColor() const;

    void setLegendItemBorderColor(const QColor& clr);
    QColor legendItemBorderColor() const;

    void setLegendItemColumns(int cols);
    int legendItemColumns() const ;

    void setLegendItemHAlign(int align);
    int legendItemHAlign() const;

    void setLegendItemVAlign(int align);
    int legendItemVAlign() const;

    void setLegendItemBgMode(int mode);
    int legendItemBgMode() const;

    // Curve symbol
    void setSymbol(QwtSymbol::Style style);
    QwtSymbol::Style symbol() const;

    void setSymbolSize(int size);
    int symbolSize() const;

    void setSymbolWidth(int width);
    int symbolWidth() const;

    void setSymbolOpacity(float opacity);
    float symbolOpacity() const;

    void setSymbolColor(const QColor& clr);
    QColor symbolColor() const;

    void setSymbolEdgeColor(const QColor& clr);
    QColor symbolEdgeColor() const;

    // Dropline
    void setHorizontalDroplineVisible(bool visible);
    bool isHorizontalDroplineVisible() const;

    void setVerticalDroplineVisible(bool visible);
    bool isVerticalDroplineVisible() const;

    void setDroplinePen(const QPen& pen);
    QPen droplinePen() const;

    void setDroplineColor(const QColor& clr);
    QColor droplineColor() const;

    void setDroplineOpacity(float opacity);
    float droplineOpacity() const;

    void setUseGroupLineColor(bool use);
    bool useGroupLineColor() const;

    void setUseGroupPatternColor(bool use);
    bool useGroupPatternColor() const;

    void setUseGroupPenStyle(bool use);
    bool useGroupPenStyle() const;

    void setUseGroupBubbleStyle(bool use);
    bool useGroupBubbleStyle() const;

    void setGroupLineColors(const QList<QColor>& clrs);
    QList<QColor> groupLineColors() const;

    void setGroupPatternColors(const QList<QColor>& clrs);
    QList<QColor> groupPatternColors() const;

    void setGroupPenStyle(const QList<Qt::PenStyle> styles);
    QList<Qt::PenStyle> groupPenStyle() const;

    void setGroupBubbleStyle(const QList<QwtSymbol::Style>& styles);
    QList<QwtSymbol::Style> groupBubbleStyle() const;

    // label
    void setCurveLabelVisible(bool visible);
    bool isCurveLabelVisible() const;

    void setCurveLabelFont(const QFont& f);
    QFont curveLabelFont() const;

    void setCurveLabelOffsetX(int offset);
    int curveLabelOffsetX() const;

    void setCurveLabelOffsetY(int offset);
    int curveLabelOffsetY() const;

    void setCurveLabelPosition(QwtPlotCurve::LabelPosition pos);
    QwtPlotCurve::LabelPosition curveLabelPosition() const;

    void setCurveLabelOpacity(float opacity);
    float curveLabelOpacity() const;

    void setCurveLabelColor(const QColor& clr);
    QColor curveLabelColor() const;

    // bubble section
    void setBubbleSize(int size);
    int bubbleSize() const;

    void setBubbleEdgeWidth(int width);
    int bubbleEdgeWidth() const;

    void setBubbleOpacity(float opacity);
    float bubbleOpacity() const;

    // pie section
    void setPieThickness(int thickness);
    int pieThickness() const;

    void setPieStartAngle(int startAngle);
    int pieStartAngle();

    void setPieCounterClockwise(bool counter);
    bool isPieCounterClockwise() const;

    void setPieViewAngle(int angle);
    int pieViewAngle() const;

    void setPieRadiusRatio(float ratio);
    float pieRadiusRatio() const;

    void setPieDisplacementRatio(float ratio);
    float pieDisplacementRatio() const;

    void setAxisTitle(int axisId, const QString &title);
    QString axisTitle(int axisId) const;

    void setAntiAliasingCurve(bool anti);
    bool isAntiAliasingCurve() const;

    void resetColorIndex();

    void setUseCustomSymbol(bool use);
    bool useCustomSymbol() const;

    void setUseOriginalCurveColor(bool use);
    bool isUseOriginalCurveColor() const;

signals:
    void destroy();

protected slots:
    void onColumnDataChanged();
    void onPropertyChanged();
    virtual void onItemAttached(QwtPlotItem *plotItem, bool on);

protected:
    virtual ~Graph();

    void init();
    void initGrid();
    void initLegend();
    void initSymbol();
    void initDropline();
    void initCurveLabel();
    void initBarChartLabel();
    void initBubble();
    void initPie();
    void initCurve();
    void initProperty();
    void initSettings();

    void configGrid();
    void configLegend();
    void configSymbol();
    void configDropline();
    void configGroupLineColors();
    void configGroupPenStyles();
    void configCurveLabel();
    void configAxisTitles();
    void configCurve();
    void getAxisTitle(const ComData::ColumnBase* column, QwtPlot::Axis axis);
    void setPrecisionByMinValue(qreal min);

protected:
    int m_type = -1;
    int m_colorIndex;
    AxisTitleType m_axisTitleType = ShortName;
    bool m_hasXColumn = false;
    bool m_useGroupLineColor;
    bool m_useGroupPatternColor;
    bool m_useGroupPenStyle;
    bool m_useGroupBubbleStyle;
    bool m_useCustomSymbol = false;
    bool m_useOriginalCurveColor = false;
    bool m_isInternalRemove = false;

    QList<QSharedPointer<ComData::ColumnBase>> m_columnData;
    GraphProperty m_property;

    QwtPlotGrid *m_grid = nullptr;
    GridData* m_gridData = nullptr;
    LegendData* m_legendData = nullptr;
    SymbolData* m_symbolData = nullptr;
    DroplineData* m_droplineData = nullptr;
    CurveLabelData* m_curveLabelData = nullptr;
    BarChartLabelData* m_barChartLabelData = nullptr;
    BubbleData* m_bubbleData = nullptr;
#ifdef RESERVE
    PieData* m_pieData = nullptr;
#endif
    CurveData* m_curveData = nullptr;

    QMap<QwtPlotSeriesItem*, PlotItemData> m_plotItem2ColumnMap;
    QList<QColor> m_groupLineColors;
    QList<QColor> m_groupPatternColors;
    QList<Qt::PenStyle> m_groupPenStyles;
    QList<QwtSymbol::Style> m_groupBubbleStyles;
    QMap<QwtPlot::Axis, QString> m_axisTitleMap;
};

template <class T>
inline T* cloneGraph(Graph* parent, int type, const QList<QSharedPointer<ComData::ColumnBase>>& columns)
{
    T* graph = new T(type);
    graph->setData(columns);
    graph->initialize();
    graph->setProperty(parent->property());
    return graph;
}

template <class T>
inline T* cloneGraph(Graph* parent, const QList<QSharedPointer<ComData::ColumnBase>>& columns)
{
    T* graph = new T();
    graph->setData(columns);
    graph->initialize();
    graph->setProperty(parent->property());
    return graph;
}

template <class T>
inline T* cloneRawGraph(Graph* parent, int type)
{
    T* graph = new T(type);
    graph->initialize();
    graph->resize(parent->size());
    graph->setProperty(parent->property());
    return graph;
}

template <class T>
inline T* cloneRawGraph(Graph* parent)
{
    T* graph = new T();
    graph->initialize();
    graph->resize(parent->size());
    graph->setProperty(parent->property());
    return graph;
}

} // namespace VizCore
#endif // GRAPH_H
