#ifndef GRAPHPROPERTY_H
#define GRAPHPROPERTY_H

#include <QObject>
#include <QColor>
#include <QFont>

#include <qwt/qwt_column_symbol.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_abstract_scale_draw.h>

#include "comdatagui_global.h"
#include "utils.h"

namespace ComDataGui
{

class Graph;
/*!
 * \class 图形属性类
 */
class COMDATAGUI_EXPORT GraphProperty : public QObject
{
    Q_OBJECT
public:
    GraphProperty(Graph* g = nullptr);
    GraphProperty(const GraphProperty& other);
    GraphProperty& operator=(const GraphProperty& other);

public slots:
    void apply();

    void readSettings();
    void writeSettings();

    void setGraph(Graph* g);
    Graph* graph() const;

    // bg colors
    void setBackgroundStartColor(const QColor& clr) { m_bgStartColor = clr; }
    QColor backgroundStartColor() const { return m_bgStartColor; }

    void setBackgroundStopColor(const QColor& clr) { m_bgStopColor = clr; }
    QColor backgroundStopColor() const { return m_bgStopColor; }

    void setBackgroundLightness(int lightness) { m_bgLightness = lightness; }
    int backgroundLightness() const { return m_bgLightness; }

    void setGradientFillMode(int mode) { m_bgGradientFillMode = mode; }
    int gradientFillMode() const { return m_bgGradientFillMode; }

    void setBackgroundGradientFill(int gt) { m_bgGradientFill = static_cast<ComDataGui::Utils::GradientType>(gt); }
    int backgroundGradientFill() const { return m_bgGradientFill; }

    void setCanvasGradientFillMode(int mode) { m_canvasGradientFillMode = mode; }
    int canvasGradientFillMode() const { return m_canvasGradientFillMode; }

    void setCanvasStartColor(const QColor& clr) { m_canvasStartColor = clr; }
    QColor canvasStartColor() const { return m_canvasStartColor; }

    void setCanvasStopColor(const QColor& clr) { m_canvasStopColor = clr; }
    QColor canvasStopColor() const { return m_canvasStopColor; }

    void setCanvasLightness(int lightness) { m_canvasLightness = lightness; }
    int canvasLightness() const { return m_canvasLightness; }

    void setCanvasGradientFill(int gt) { m_canvasGradientFill = static_cast<ComDataGui::Utils::GradientType>(gt); }
    int canvasGradientFill() const { return m_canvasGradientFill; }

    void setCanvasBorderRadius(int radius) { m_canvasBorderRadius = radius; }
    int canvasBorderRadius() const { return m_canvasBorderRadius; }

    // visibilities
    void setXAxisVisible(bool visible) { m_isXAxisVisible = visible; }
    bool isXAxisVisible() const { return m_isXAxisVisible; }

    void setYAxisVisible(bool visible) { m_isYAxisVisible = visible; }
    bool isYAxisVisible() const { return m_isYAxisVisible; }

    void setHorizontalTitleVisible(bool visible) { m_isHTitleVisible = visible; }
    bool isHorizontalTitleVisible() const { return m_isHTitleVisible; }

    void setVerticalTitleVisible(bool visible) { m_isVTitleVisible = visible; }
    bool isVerticalTitleVisible() const { return m_isVTitleVisible; }

    void setTitleVisible(bool visible) { m_isTitleVisible = visible; }
    bool isTitleVisible() const { return m_isTitleVisible; }

    void setDataVisible(bool visible) { m_isDataVisible = visible; }
    bool isDataVisible() const { return m_isDataVisible; }

    // title
    void setTitleFont(const QFont& f) { m_titleFont = f; }
    QFont titleFont() const { return m_titleFont; }

    void setTitleSize(int size) { m_titleFont.setPointSize(size); }
    int titleSize() const { return m_titleFont.pointSize(); }

    void setTitleOpacity(double opacity) { m_titleOpacity = opacity; }
    double titleOpacity() const { return m_titleOpacity; }

    void setTitleColor(const QColor& clr) { m_titleColor = clr; }
    QColor titleColor() const { return m_titleColor; }

    void setTitleBold(bool bold) { m_titleFont.setBold(bold); }
    bool isTitleBold() const { return m_titleFont.bold(); }

    void setTitleItalic(bool italic) { m_titleFont.setItalic(italic); }
    bool isTitleItalic() const { return m_titleFont.italic(); }

    void setTitleUnderline(bool underline) { m_titleFont.setUnderline(underline); }
    bool isTitleUnderline() const { return m_titleFont.underline(); }

    void setTitleText(const QString& text);
    QString titleText() const { return m_title; }

    // legend
    void setLegendPosition(int pos) { m_legendPosition = pos; }
    int legendPosition() const { return m_legendPosition; }

    void setLegendFont(const QFont& f) { m_legendFont = f; }
    QFont legendFont() const { return m_legendFont; }

    void setLegendColor(const QColor& clr) { m_legendColor = clr; }
    QColor legendColor() const { return m_legendColor; }

    void setLegendFontSize(int size) { m_legendFont.setPointSize(size); }
    int legendFontSize() const { return m_legendFont.pointSize(); }

    void setLegendVisible(bool visible) { m_isLegendVisible = visible; }
    bool isLegendVisible() const { return m_isLegendVisible; }

    // Legend Item
    void setLegendItemVisible(bool visible) { m_isLegendItemVisible = visible; }
    bool isLegendItemVisible() const { return m_isLegendItemVisible; }

    void setLegendItemFont(const QFont& f) { m_legendItemFont = f; }
    QFont legendItemFont() const { return m_legendItemFont; }

    void setLegendItemBgColor(const QColor& clr) { m_legendItemBgColor = clr; }
    QColor legendItemBgColor() const { return m_legendItemBgColor; }

    void setLegendItemColor(const QColor& clr) { m_legendItemFontColor = clr; }
    QColor legendItemColor() const { return m_legendItemFontColor; }

    void setLegendItemBorderColor(const QColor& clr) { m_legendItemBorderColor = clr; }
    QColor legendItemBorderColor() const { return m_legendItemBorderColor; }

    void setLegendItemColumns(int cols) { m_legendItemColumns = cols; }
    int legendItemColumns() const { return m_legendItemColumns; }

    void setLegendItemHAlign(int align) { m_legendItemHAlign = align; }
    int legendItemHAlign() const { return m_legendItemHAlign; }

    void setLegendItemVAlign(int align) { m_legendItemVAlign = align; }
    int legendItemVAlign() const { return m_legendItemVAlign; }

    void setLegendItemBgMode(int mode) { m_legendItemBgMode = mode; }
    int legendItemBgMode() const { return m_legendItemBgMode; }

    // Axis connect
    void setConnectAxisBreak(bool connect) { m_connectAxisBreak = connect; }
    bool isConnectAxisBreak() const { return m_connectAxisBreak; }

    void setConnectMissingData(bool connect) { m_connectMissingData = connect; }
    bool isConnectMissingData() const { return m_connectMissingData; }

    void setGraphWindowGradientFill(int fill) { m_graphWindowGradientFill = fill; }
    int graphWindowGradientFill() const { return m_graphWindowGradientFill; }

    void setAxisLabelFont(const QFont& f) { m_axisLabelFont = f; }
    QFont axisLabelFont() const { return m_axisLabelFont; }

    void setAxisTickColor(const QColor& clr) { m_axisTickColor = clr; }
    QColor axisTickColor() const { return m_axisTickColor; }

    void setAxisLabelColor(const QColor& clr) { m_axisLabelColor = clr; }
    QColor axisLabelColor() const { return m_axisLabelColor; }

    void setAxisTickWidth(int width) { m_axisTickWidth = width; }
    int axisTickWidth() const { return m_axisTickWidth; }

    void setAxisTitleType(int type) { m_axisTitleType = type; }
    int axisTitleType() const { return m_axisTitleType; }

    void setMajorLabelsVisible(bool visible) { m_axisMajorLabelsVisible = visible; }
    bool isMajorLabelsVisible() const { return m_axisMajorLabelsVisible; }

    void setMinorLabelsVisible(bool visible) { m_axisMinorLabelsVisible = visible; }
    bool isMinorLabelsVisible() const { return m_axisMinorLabelsVisible; }

    void setPlusSignVisible(bool visible) { m_axisPlusSignVisible = visible; }
    bool isPlusSignVisible() const { return m_axisPlusSignVisible; }

    void setMinusSignVisible(bool visible) { m_axisMinusSignVisible = visible; }
    bool isMinusSignVisible() const { return m_axisMinusSignVisible; }

    void setPrefix(const QString& prefix) { m_axisLabelPrefix = prefix; }
    QString prefix() const { return m_axisLabelPrefix; }

    void setSuffix(const QString& suffix) { m_axisLabelSuffix = suffix; }
    QString suffix() const { return m_axisLabelSuffix; }

    void setDecimalWidth(int width) { m_axisDecimalWidth = width; }
    int decimalWidth() const { return m_axisDecimalWidth; }

    void setAxisNotation(QwtAbstractScaleDraw::Notation notation) { m_axisNotation = notation; }
    QwtAbstractScaleDraw::Notation axisNotation() const { return m_axisNotation; }

    // Bar
    void setBarChartFillPattern(Qt::BrushStyle bs) { m_barChartFillPattern = bs; }
    Qt::BrushStyle barChartFillPattern() const { return m_barChartFillPattern; }

    void setBarChartFillStyle(QwtColumnSymbol::FrameStyle fs) { m_barChartFillStyle = fs; }
    QwtColumnSymbol::FrameStyle barChartFillStyle() const { return m_barChartFillStyle; }

    void setBarChartFillOpacity(float opacity) { m_barChartFillOpacity = opacity; }
    float barChartFillOpacity() const { return m_barChartFillOpacity; }

    void setBarChartLineWidth(int width) { m_barChartLineWidth = width; }
    int barChartLineWidth() const { return m_barChartLineWidth; }

    void setBarChartBorderPen(const QPen& pen) { m_barChartBorderPen = pen; }
    QPen barChartBorderPen() const { return m_barChartBorderPen; }

    // Curve
    void setCurveLineWidth(int width) { m_curveLineWidth = width; }
    int curveLineWidth() const { return m_curveLineWidth; }

    void setCurveLineOpacity(float opacity) { m_curveLineOpacity = opacity; }
    float curveLineOpacity() const { return m_curveLineOpacity; }

    void setMaxCurveDataCount(int count) { m_maxCurveDataCount = count; }
    int maxCurveDataCount() const { return m_maxCurveDataCount; }

    // bar label
    void setBarLabelVisible(bool visible) { m_isBarLabelVisible = visible; }
    bool isBarLabelVisible() const { return m_isBarLabelVisible; }

    void setBarLabelOpacity(float opacity) { m_barLabelOpacity = opacity; }
    float barLabelOpacity() const { return m_barLabelOpacity; }

    void setBarLabelFont(const QFont& f) { m_barLabelFont = f; }
    QFont barLabelFont() const { return m_barLabelFont; }

    void setBarLabelColor(const QColor& clr) { m_barLabelColor = clr; }
    QColor barLabelColor() const { return m_barLabelColor; }

    void setBarLabelPosition(QwtPlot::LabelPosition lp) { m_barLabelPosition = lp; }
    QwtPlot::LabelPosition barLabelPosition() const { return m_barLabelPosition; }

    void setBarLabelOffsetX(int x) { m_barLabelOffsetX = x; }
    int barLabelOffsetX() const { return m_barLabelOffsetX; }

    void setBarLabelOffsetY(int y) { m_barLabelOffsetY = y; }
    int barLabelOffsetY() const { return m_barLabelOffsetY; }

    // Grid section
    void setGridVisible(bool visible) { m_isGridVisible = visible; }
    bool isGridVisible() const { return m_isGridVisible; }

    void setMajorGridVisible(bool visible) { m_isMajorGridVisible = visible; }
    bool isMajorGridVisible() const { return m_isMajorGridVisible; }

    void setMinorGridVisible(bool visible) { m_isMinorGridVisible = visible; }
    bool isMinorGridVisible() const { return m_isMinorGridVisible; }

    void setGridPenStyle(Qt::PenStyle ps) { m_gridPen.setStyle(ps); }
    Qt::PenStyle gridPenStyle() const { return m_gridPen.style(); }

    void setGridPenColor(const QColor& clr) { m_gridPen.setColor(clr); }
    QColor gridPenColor() const { return m_gridPen.color(); }

    void setGridPenWidth(int width) { if (width < 0) qDebug() << "negative pen width"; m_gridPen.setWidth(width); }
    int gridPenWidth() const { return m_gridPen.width(); }

    void setHorizontalGridVisible(bool visible) { m_isGridHorizontalVisible = visible; }
    bool isHorizontalGridVisible() const { return m_isGridHorizontalVisible; }

    void setVerticalGridVisible(bool visible) { m_isGridVerticalVisible = visible; }
    bool isVerticalGridVisible() const { return m_isGridVerticalVisible; }

    // Curve symbol
    void setSymbol(QwtSymbol::Style style) { m_symbolStyle = style; }
    QwtSymbol::Style symbol() const { return m_symbolStyle; }

    void setSymbolSize(int size) { m_symbolSize = size; }
    int symbolSize() const { return m_symbolSize; }

    void setSymbolWidth(int width) { m_symbolWidth = width; }
    int symbolWidth() const { return m_symbolWidth; }

    void setSymbolOpacity(float opacity) { m_symbolOpacity = opacity; }
    float symbolOpacity() const { return m_symbolOpacity; }

    void setSymbolColor(const QColor& clr) { m_symbolColor = clr; }
    QColor symbolColor() const { return m_symbolColor; }

    void setSymbolEdgeColor(const QColor& clr) { m_symbolEdgeColor = clr; }
    QColor symbolEdgeColor() const { return m_symbolEdgeColor; }

    // Dropline
    void setHorizontalDroplineVisible(bool visible) { m_isHorizontalDroplineVisible = visible; }
    bool isHorizontalDroplineVisible() const { return m_isHorizontalDroplineVisible; }

    void setVerticalDroplineVisible(bool visible) { m_isVerticalDroplineVisible = visible; }
    bool isVerticalDroplineVisible() const { return m_isVerticalDroplineVisible; }

    void setAntiAliasingCurve(bool anti) { m_isAntiAliasingCurve = anti; }
    bool isAntiAliasingCurve() const { return m_isAntiAliasingCurve; }

    void setDroplinePen(const QPen& pen) { m_droplinePen = pen; }
    QPen droplinePen() const { return m_droplinePen; }

    void setDroplineColor(const QColor& clr) { m_droplinePen.setColor(clr); }
    QColor droplineColor() const { return m_droplinePen.color(); }

    void setDroplineOpacity(float opacity) { m_droplineOpacity = opacity; }
    float droplineOpacity() const { return m_droplineOpacity; }

    // Group related
    void setUseGroupLineColor(bool use) { m_useGroupLineColor = use; }
    bool useGroupLineColor() const { return m_useGroupLineColor; }

    void setUseGroupPatternColor(bool use) { m_useGroupPatternColor = use; }
    bool useGroupPatternColor() const { return m_useGroupPatternColor; }

    void setUseGroupPenStyle(bool use) { m_useGroupPenStyle = use; }
    bool useGroupPenStyle() const { return m_useGroupPenStyle; }

    void setUseGroupBubbleStyle(bool use) { m_useGroupBubbleStyle = use; }
    bool useGroupBubbleStyle() const { return m_useGroupBubbleStyle; }

    void setGroupLineColors(const QMap<QString, QVariant>& clrs) { m_groupLineColors = clrs; }
    QMap<QString, QVariant> groupLineColors() const { return m_groupLineColors; }

    void setGroupPatternColors(const QMap<QString, QVariant>& clrs) { m_groupPatternColors = clrs; }
    QMap<QString, QVariant> groupPatternColors() const { return m_groupPatternColors; }

    void setGroupPenStyles(const QMap<QString, QVariant>& styles) { m_groupPenStyles = styles; }
    QMap<QString, QVariant> groupPenStyle() const { return m_groupPenStyles; }

    void setGroupBubbleStyles(const QMap<QString, QVariant>& styles) { m_groupBubbleStyles = styles; }
    QMap<QString, QVariant> groupBubbleStyles() const { return m_groupBubbleStyles; }

    // label
    void setCurveLabelVisible(bool visible) { m_curveLabelData.isVisible = visible; m_curveLabelData.makeDirty(); }
    bool isCurveLabelVisible() const { return m_curveLabelData.isVisible; }

    void setCurveLabelFont(const QFont& f) { m_curveLabelData.font = f; m_curveLabelData.makeDirty();}
    QFont curveLabelFont() const { return m_curveLabelData.font; }

    void setCurveLabelOffsetX(int offset) { m_curveLabelData.xOffset = offset; m_curveLabelData.makeDirty(); }
    int curveLabelOffsetX() const { return m_curveLabelData.xOffset; }

    void setCurveLabelOffsetY(int offset) { m_curveLabelData.yOffset = offset; m_curveLabelData.makeDirty(); }
    int curveLabelOffsetY() const { return m_curveLabelData.yOffset; }

    void setCurveLabelPosition(QwtPlotCurve::LabelPosition pos) { m_curveLabelData.pos = pos; m_curveLabelData.makeDirty(); }
    QwtPlotCurve::LabelPosition curveLabelPosition() const { return m_curveLabelData.pos; }

    void setCurveLabelOpacity(float opacity) { m_curveLabelData.opacity = opacity; m_curveLabelData.makeDirty(); }
    float curveLabelOpacity() const { return m_curveLabelData.opacity; }

    void setCurveLabelColor(const QColor& clr) { m_curveLabelData.color = clr; m_curveLabelData.makeDirty(); }
    QColor curveLabelColor() const { return m_curveLabelData.color; }

    // bubble
    void setBubbleSize(int size) { m_bubbleSize = size; }
    int bubbleSize() const { return m_bubbleSize; }

    void setBubbleEdgeWidth(int width) { m_bubbleEdgeWidth = width; }
    int bubbleEdgeWidth() const { return m_bubbleEdgeWidth; }

    void setBubbleOpacity(float opacity) { m_bubbleOpacity = opacity; }
    float bubbleOpacity() const { return m_bubbleOpacity; }

    // pie
    void setPieStartAngle(int startAngle) { m_pieStartAngle = startAngle; }
    int pieStartAngle() const { return m_pieStartAngle; }

    void setPieThickness(int thickness) { m_pieThickness = thickness; }
    int pieThickness() const { return m_pieThickness; }

    void setPieCounterClockwise(bool counter) { m_isCounterClockwise = counter; }
    bool isPieCounterClockwise() const { return m_isCounterClockwise; }

    void setPieViewAngle(int angle) { m_pieViewAngle = angle; }
    int pieViewAngle() const { return m_pieViewAngle; }

    void setPieRadiusRatio(float ratio) { m_pieRadiusRatio = ratio; }
    float pieRadiusRatio() const { return m_pieRadiusRatio; }

    void setPieDisplacementRatio(float ratio) { m_pieDisplacementRatio = ratio; }
    float pieDisplacementRatio() const { return m_pieDisplacementRatio; }

    // error bar
    void setErrorBarPlusVisible(bool visible) { m_isErrorBarPlusVisible = visible; }
    bool isErrorBarPlusVisible() const { return m_isErrorBarPlusVisible; }

    void setErrorBarMinusVisible(bool visible) { m_isErrorBarMinusVisible = visible; }
    bool isErrorBarMinusVisible() const { return m_isErrorBarMinusVisible; }

    void setErrorBarLineColor(const QColor& clr) { m_errorBarLineColor = clr; }
    QColor errorBarLineColor() const { return m_errorBarLineColor; }

    void setErrorBarLineWidth(int width) { m_errorBarLineWidth = width; }
    int errorBarLineWidth() const { return m_errorBarLineWidth; }

    void setErrorBarHandleWidth(int width) { m_errorBarHandleWidth = width; }
    int errorBarHandleWidth() const { return m_errorBarHandleWidth; }

    void setErrorBarLineOpacity(float opacity) { m_errorBarLineOpacity = opacity; }
    float errorBarLineOpacity() const { return m_errorBarLineOpacity; }

    void print() const;

signals:
    void changed();

protected:
    void initVars();
    void initFrom(const GraphProperty& other);

private:
    Graph* m_graph;

    struct BaseData
    {
        bool isDirty = false;
        void makeDirty() { isDirty = true; }
        void clean() { isDirty = false; }
    };

    bool m_isXAxisVisible : 1;
    bool m_isYAxisVisible : 1;
    bool m_isHTitleVisible : 1;
    bool m_isVTitleVisible : 1;
    bool m_isTitleVisible : 1;
    bool m_isDataVisible : 1;
    bool m_isLegendVisible : 1;
    bool m_isLegendItemVisible : 1;
    bool m_connectAxisBreak : 1;
    bool m_connectMissingData : 1;
    bool m_isAxisMajorMarkVisible : 1;
    bool m_isAxisMinorMarkVisible : 1;
    bool m_isBarLabelVisible : 1;
    bool m_isGridVisible : 1;
    bool m_isMajorGridVisible : 1;
    bool m_isMinorGridVisible : 1;
    bool m_isGridHorizontalVisible : 1;
    bool m_isGridVerticalVisible : 1;
    bool m_isHorizontalDroplineVisible : 1;
    bool m_isVerticalDroplineVisible : 1;
    bool m_useGroupLineColor : 1;
    bool m_useGroupPatternColor : 1;
    bool m_useGroupPenStyle : 1;
    bool m_useGroupBubbleStyle : 1;
    bool m_axisMajorLabelsVisible : 1;
    bool m_axisMinorLabelsVisible : 1;
    bool m_axisPlusSignVisible : 1;
    bool m_axisMinusSignVisible : 1;
    bool m_isCounterClockwise : 1;
    bool m_isAntiAliasingCurve : 1;
    bool m_isErrorBarPlusVisible : 1;
    bool m_isErrorBarMinusVisible : 1;

    QFont m_axisLabelFont;
    QColor m_axisTickColor;
    QColor m_axisLabelColor;
    QString m_axisLabelPrefix;
    QString m_axisLabelSuffix;
    int m_axisDecimalWidth = 0;
    QwtAbstractScaleDraw::Notation m_axisNotation = QwtAbstractScaleDraw::Normal;

    QString m_title;
    QString m_verticalTitle;
    QString m_horizontalTitle;

    QColor m_bgStartColor;
    QColor m_bgStopColor;
    ComDataGui::Utils::GradientType m_bgGradientFill;

    QColor m_canvasStartColor;
    QColor m_canvasStopColor;
    ComDataGui::Utils::GradientType m_canvasGradientFill;

    QFont m_titleFont;
    QColor m_titleColor;

    int m_graphWindowGradientFill;
    int m_axisTickWidth;
    int m_barChartLineWidth;
    int m_curveLineWidth;
    int m_symbolSize;
    int m_symbolWidth;
    int m_maxCurveDataCount;
    int m_bgLightness;
    int m_canvasLightness;
    int m_bgGradientFillMode;
    int m_canvasGradientFillMode;
    int m_legendPosition;
    int m_legendItemColumns;
    int m_legendItemHAlign;
    int m_legendItemVAlign;
    int m_legendItemBgMode;
    int m_barLabelOffsetX;
    int m_barLabelOffsetY;
    int m_bubbleSize;
    int m_bubbleEdgeWidth;
    int m_canvasBorderRadius;
    int m_pieStartAngle = 90;
    int m_pieThickness = 30;
    int m_pieViewAngle;
    int m_axisTitleType;
    int m_errorBarLineWidth;
    int m_errorBarHandleWidth;

    float m_titleOpacity = 1.0;
    float m_barChartFillOpacity;
    float m_curveLineOpacity;
    float m_barLabelOpacity;
    float m_symbolOpacity;
    float m_droplineOpacity;
    float m_bubbleOpacity;
    float m_pieRadiusRatio;
    float m_pieDisplacementRatio;
    float m_errorBarLineOpacity;

    QFont m_legendFont;
    QColor m_legendColor;

    QFont m_legendItemFont;
    QColor m_legendItemBgColor;
    QColor m_legendItemFontColor;
    QColor m_legendItemBorderColor;

    Qt::BrushStyle m_barChartFillPattern;
    QwtColumnSymbol::FrameStyle m_barChartFillStyle;

    QFont m_barLabelFont;
    QwtPlot::LabelPosition m_barLabelPosition;
    QColor m_barLabelColor;

    QPen m_gridPen;
    QPen m_barChartBorderPen;
    QPen m_droplinePen;

    QColor m_symbolColor;
    QColor m_symbolEdgeColor;
    QwtSymbol::Style m_symbolStyle;

    QColor m_errorBarLineColor;

    QMap<QString, QVariant> m_groupLineColors;
    QMap<QString, QVariant> m_groupPatternColors;
    QMap<QString, QVariant> m_groupPenStyles;
    QMap<QString, QVariant> m_groupBubbleStyles;

    struct CurveLabelData : public BaseData
    {
        QColor color;
        bool isVisible;
        float opacity;
        int xOffset;
        int yOffset;
        QFont font;
        QwtPlotCurve::LabelPosition pos;

        CurveLabelData operator=(const CurveLabelData& other)
        {
            color = other.color;
            isVisible = other.isVisible;
            opacity = other.opacity;
            xOffset = other.xOffset;
            yOffset = other.yOffset;
            font = other.font;
            pos = other.pos;
            return *this;
        }
    };

    CurveLabelData m_curveLabelData;

    friend class DefaultProperty;
};

}
#endif // GRAPHPROPERTY_H
