#include "graphproperty.h"
#include "graph.h"
#include "settingkeys.h"

#include <comutils/settingsmanager.h>
#include <comguiutils/colorcombobox.h>

#include <qwt/qwt_text_label.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_plot_legenditem.h>
#include <qwt/qwt_plot_canvas.h>

#include <QSettings>

#define DEBUG(VAR) qDebug() << #VAR << " = " << VAR;

namespace ComDataGui
{

class DefaultProperty
{
public:
    static bool isXAxisVisible() { return true; }
    static bool isYAxisVisible() { return true; }
    static bool isHTitleVisible() { return true; }
    static bool isVTitleVisible() { return true; }
    static bool isTitleVisible() { return true; }
    static bool isDataVisible() { return true; }
    static bool isLegendVisible() { return true; }
    static bool isLegendItemVisible() { return true; }
    static bool isConnectAxisBreak() { return false; }
    static bool isConnectMissingData() { return false; }
    static bool isAxisMajorMarkVisible() { return true; }
    static bool isAxisMinorMarkVisible() { return false; }
    static bool isBarLabelVisible() { return false; }
    static bool isGridVisible() { return false; }
    static bool isMajorGridVisible() { return true; }
    static bool isMinorGridVisible() { return false; }
    static bool isHorizontalGridVisible() { return true; }
    static bool isVerticalGridVisible() { return true; }
    static bool isHorizontalDroplineVisible() { return false; }
    static bool isVerticalDroplineVisible() { return false; }
    static bool useGroupLineColor() { return true; }
    static bool useGroupPatternColor() { return true; }
    static bool useGroupPenStyle() { return true; }
    static bool useGroupBubbleStyle() { return true; }
    static bool isAxisMajorLabelsVisible() { return true; }
    static bool isAxisMinorLabelsVisible() { return false; }
    static bool isAxisPlusSignVisible() { return false; }
    static bool isAxisMinusSignVisible() { return true; }
    static bool isCounterClockwise() { return false; }
    static bool isAntiAliasingCurve() { return true; }

    static QFont axisLabelFont() { QFont f; return f; }
    static QColor axisTickColor() { return Qt::black; }
    static QColor axisLabelColor() { return Qt::black; }
    static QString axisLabelPrefix() { return QString(); }
    static QString axisLabelSuffix() { return QString(); }
    static int axisDecimalWidth() { return 0; }
    static QwtAbstractScaleDraw::Notation axisNotation() { return QwtAbstractScaleDraw::Normal; }

    static QString title() { return QObject::tr("Default Title"); }
    static QString verticalTitle() { return QObject::tr("Default Vertical Title"); }
    static QString horizontalTitle() { return QObject::tr("Default Horizontal Title"); }
    static QColor backgroundStartColor() { return Qt::white; }
    static QColor backgroundStopColor() { return Qt::white; }
    static ComDataGui::Utils::GradientType backgroundGradientType() { return ComDataGui::Utils::TopBottom; }
    static QFont titleFont() { QFont f; f.setBold(true); f.setPointSize(20); return f; }
    static QColor titleColor() { return Qt::black; }

    static int graphWindowGradientFill() { return 0; }
    static int axisTickWidth() { return 1; }
    static int axisTitleType() { return ComDataGui::Graph::ShortName; }
    static int barChartLineWidth() { return 5; }
    static int curveLineWidth() { return 1; }
    static int symbolSize() { return 12; }
    static int symbolWidth() { return 1; }
    static int maxCurveDataCount() { return 1000; }
    static int backgroundLightness() { return 50; }
    static int backgroundGradientFill() { return 0; }

    static QColor canvasStartColor() { return QColor(115, 178, 255).lighter(); }
    static QColor canvasStopColor() { return QColor(115, 178, 255); }
    static int canvasLightness() { return 50; }
    static int canvasGradientFill() { return 2; }

    static int legendPosition() { return QwtPlot::RightLegend; }
    static int legendItemColumns() { return 1; }
    static int legendItemHAlign() { return Qt::AlignLeft; }
    static int legendItemVAlign() { return Qt::AlignTop; }
    static int legendItemBackgroundMode() { return QwtPlotLegendItem::LegendBackground; }

    static int barLabelOffsetX() { return 0; }
    static int barLabelOffsetY() { return 0; }
    static int bubbleSize() { return 15; }
    static int bubbleEdgeWidth() { return 1; }
    static int canvasBorderRadius() { return 17; }
    static int pieStartAngle() { return 90; }
    static int pieThickness() { return 20; }
    static int pieViewAngle() { return 60; }

    static float titleOpacity() { return 1.0; }
    static float barChartFillOpacity() { return 1.0; }
    static float curveLineOpacity() { return 1.0; }
    static float barLabelOpacity() { return 1.0; }
    static float symbolOpacity() { return 1.0; }
    static float droplineOpacity() { return 1.0; }
    static float bubbleOpacity() { return 1.0; }
    static float pieRadiusRatio() { return 1.0; }
    static float pieDisplacementRatio() { return 0.2; }

    static QFont legendFont() { QFont f; return f; }
    static QColor legendColor() { return Qt::black; }

    static QFont legendItemFont() { QFont f; return f; }
    static QColor legendItemBackgroundColor() { return Qt::white; }
    static QColor legendItemFontColor() { return Qt::black; }
    static QColor legendItemBorderColor() { return Qt::black; }

    static Qt::BrushStyle barChartFillPattern() { return Qt::SolidPattern; }
    static QwtColumnSymbol::FrameStyle barChartFillStyle() { return QwtColumnSymbol::Raised; }

    static QFont barLabelFont() { QFont f; return f; }
    static QwtPlot::LabelPosition barLabelPosition() { return QwtPlot::OutsideEnd; }
    static QColor barLabelColor() { return Qt::black; }

    static QPen gridPen() { QPen p(Qt::yellow, 1, Qt::DotLine); return p; }
    static QPen barChartBorderPen() { QPen p(Qt::black); return p; }
    static QPen droplinePen() { QPen p(Qt::green, 1, Qt::DashLine); return p; }

    static QColor symbolColor() { return QColor(255, 170, 0); }
    static QColor symbolEdgeColor() { return QColor(255, 170, 0); }
    static QwtSymbol::Style symbolStyle() { return QwtSymbol::Diamond; }

    static bool isErrorBarPlusVisible() { return true; }
    static bool isErrorBarMinusVisible() { return true; }
    static QColor errorBarLineColor() { return Qt::black; }
    static int errorBarLineWidth() { return 1; }
    static int errorBarHandleWidth() { return 3; }
    static float errorBarLineOpacity() { return 1.0; }

    static GraphProperty::CurveLabelData curveLabelData()
    {
        GraphProperty::CurveLabelData cld;
        cld.color = Qt::yellow;
        cld.isVisible = false;
        cld.opacity = 1.0;
        cld.xOffset = 0;
        cld.yOffset = 0;
        cld.font = QFont();
        cld.pos = QwtPlotCurve::Above;
        return cld;
    }

    static QMap<QString, QVariant> groupLineColors()
    {
        QMap<QString, QVariant> colors;
        colors.insert("0", QColor(Qt::black));
        colors.insert("1", QColor(Qt::red));
        colors.insert("2", QColor(Qt::green));
        colors.insert("3", QColor(Qt::blue));
        colors.insert("4", QColor(Qt::cyan));
        colors.insert("5", QColor(Qt::magenta));
        colors.insert("6", QColor(Qt::yellow));
        colors.insert("7", QColor(Qt::darkYellow));
        colors.insert("8", QColor(Qt::darkBlue));
        colors.insert("9", QColor(Qt::darkMagenta));
        colors.insert("10", QColor(Qt::darkRed));
        colors.insert("11", QColor(Qt::darkGreen));
        colors.insert("12", QColor(Qt::darkCyan));
        colors.insert("13", QColor("#0000A0"));
        colors.insert("14", QColor("#FF8000"));
        colors.insert("15", QColor("#8000FF"));
        colors.insert("16", QColor("#FF0080"));
        colors.insert("17", QColor(Qt::white));
        colors.insert("18", QColor(Qt::lightGray));
        colors.insert("19", QColor(Qt::gray));
        colors.insert("20", QColor("#FFFF80"));
        colors.insert("21", QColor("#80FFFF"));
        colors.insert("22", QColor("#FF80FF"));
        colors.insert("23", QColor(Qt::darkGray));
        return colors;
    }

    static QMap<QString, QVariant> groupPatternColors()
    {
        QMap<QString, QVariant> colors;
        colors.insert("0", QColor(Qt::black));
        colors.insert("1", QColor(Qt::red));
        colors.insert("2", QColor(Qt::green));
        colors.insert("3", QColor(Qt::blue));
        colors.insert("4", QColor(Qt::cyan));
        colors.insert("5", QColor(Qt::magenta));
        colors.insert("6", QColor(Qt::yellow));
        colors.insert("7", QColor(Qt::darkYellow));
        colors.insert("8", QColor(Qt::darkBlue));
        colors.insert("9", QColor(Qt::darkMagenta));
        colors.insert("10", QColor(Qt::darkRed));
        colors.insert("11", QColor(Qt::darkGreen));
        colors.insert("12", QColor(Qt::darkCyan));
        colors.insert("13", QColor("#0000A0"));
        colors.insert("14", QColor("#FF8000"));
        colors.insert("15", QColor("#8000FF"));
        colors.insert("16", QColor("#FF0080"));
        colors.insert("17", QColor(Qt::white));
        colors.insert("18", QColor(Qt::lightGray));
        colors.insert("19", QColor(Qt::gray));
        colors.insert("20", QColor("#FFFF80"));
        colors.insert("21", QColor("#80FFFF"));
        colors.insert("22", QColor("#FF80FF"));
        colors.insert("23", QColor(Qt::darkGray));
        return colors;
    }

    static QMap<QString, QVariant> groupPenStyles()
    {
        QMap<QString, QVariant> styles;
        styles.insert("0", (int)Qt::SolidLine);
        styles.insert("1", (int)Qt::DashLine);
        styles.insert("2", (int)Qt::DotLine);
        styles.insert("3", (int)Qt::DashDotLine);
        styles.insert("4", (int)Qt::DashDotDotLine);
        styles.insert("5", (int)Qt::SolidLine);
        styles.insert("6", (int)Qt::DashLine);
        styles.insert("7", (int)Qt::DotLine);
        styles.insert("8", (int)Qt::DashDotLine);
        styles.insert("9", (int)Qt::DashDotDotLine);
        styles.insert("10", (int)Qt::SolidLine);
        styles.insert("11", (int)Qt::DashLine);
        styles.insert("12", (int)Qt::DotLine);
        styles.insert("13", (int)Qt::DashDotLine);
        styles.insert("14", (int)Qt::DashDotDotLine);
        styles.insert("15", (int)Qt::SolidLine);
        styles.insert("16", (int)Qt::DashLine);
        styles.insert("17", (int)Qt::DotLine);
        styles.insert("18", (int)Qt::DashDotLine);
        styles.insert("19", (int)Qt::DashDotDotLine);
        styles.insert("20", (int)Qt::SolidLine);
        styles.insert("21", (int)Qt::DashLine);
        styles.insert("22", (int)Qt::DotLine);
        styles.insert("23", (int)Qt::DashDotLine);
        return styles;
    }

    static QMap<QString, QVariant> groupBubbleStyles()
    {
        QMap<QString, QVariant> styles;
        styles.insert("0", QwtSymbol::Ellipse);
        styles.insert("1", QwtSymbol::Rect);
        styles.insert("2", QwtSymbol::Diamond);
        styles.insert("3", QwtSymbol::Triangle);
        styles.insert("4", QwtSymbol::DTriangle);
        styles.insert("5", QwtSymbol::UTriangle);
        styles.insert("6", QwtSymbol::LTriangle);
        styles.insert("7", QwtSymbol::RTriangle);
        styles.insert("8", QwtSymbol::Cross);
        styles.insert("9", QwtSymbol::XCross);
        styles.insert("10", QwtSymbol::HLine);
        styles.insert("11", QwtSymbol::VLine);
        styles.insert("12", QwtSymbol::Star1);
        styles.insert("13", QwtSymbol::Star2);
        styles.insert("14", QwtSymbol::Hexagon);
        styles.insert("15", QwtSymbol::Ellipse);
        styles.insert("16", QwtSymbol::Rect);
        styles.insert("17", QwtSymbol::Diamond);
        styles.insert("18", QwtSymbol::Triangle);
        styles.insert("19", QwtSymbol::DTriangle);
        styles.insert("20", QwtSymbol::UTriangle);
        styles.insert("21", QwtSymbol::LTriangle);
        styles.insert("22", QwtSymbol::RTriangle);
        styles.insert("23", QwtSymbol::Cross);
        return styles;
    }

};

GraphProperty::GraphProperty(Graph* g): m_graph(g), m_isXAxisVisible(true),
                                        m_isYAxisVisible(true)
{
    initVars();
}

GraphProperty::GraphProperty(const GraphProperty &other)
{
    initFrom(other);
}

GraphProperty& GraphProperty::operator =(const GraphProperty& other)
{
    initFrom(other);
    return *this;
}

void GraphProperty::apply()
{
    if (!m_graph)
        return;

    // canvas
    QLinearGradient canvasGradient;
    if (m_canvasStopColor.isValid()) {
        canvasGradient = ComDataGui::Utils::createGradientByTypeEx(m_canvasGradientFill, m_graph->canvas()->frameGeometry(), m_canvasStartColor, m_canvasStopColor);
    } else {
        if (m_canvasGradientFillMode == 0)
            canvasGradient = ComDataGui::Utils::createGradientByType(ComDataGui::Utils::None, m_graph->canvas()->frameGeometry(), m_canvasStartColor);
        else
            canvasGradient = ComDataGui::Utils::createGradientByType(m_canvasGradientFill, m_graph->canvas()->frameGeometry(), m_canvasStartColor);
    }
    m_graph->setCanvasBackground(QBrush(canvasGradient));

    // graph window background
    QPalette palette(m_graph->palette());
    QLinearGradient bgGradient;
    if (m_bgStopColor.isValid()) {
        bgGradient = ComDataGui::Utils::createGradientByTypeEx(m_bgGradientFill, m_graph->frameGeometry(), m_bgStartColor, m_bgStopColor);
    } else {
        if (m_bgGradientFillMode == 0)
            bgGradient = ComDataGui::Utils::createGradientByType(ComDataGui::Utils::None, m_graph->frameGeometry(), m_bgStartColor);
        else
            bgGradient = ComDataGui::Utils::createGradientByType(m_bgGradientFill, m_graph->frameGeometry(), m_bgStartColor);
    }

    palette.setBrush(QPalette::Background, QBrush(bgGradient));
    m_graph->setAutoFillBackground(true);
    m_graph->setPalette(palette);
    m_graph->setAntiAliasingCurve(m_isAntiAliasingCurve);

    // visibility settings
    m_graph->axisWidget(QwtPlot::xBottom)->setTitleVisible(m_isHTitleVisible);
    m_graph->axisWidget(QwtPlot::yLeft)->setTitleVisible(m_isVTitleVisible);

    m_graph->axisWidget(QwtPlot::yLeft)->setVisible(m_isYAxisVisible);
    m_graph->axisWidget(QwtPlot::xBottom)->setVisible(m_isXAxisVisible);
    m_graph->canvas()->setVisible(m_isDataVisible);

    if (m_graph->type() != 1001) { // for pie the type is 1001
        QwtPlotCanvas* canvas = static_cast<QwtPlotCanvas*>(m_graph->canvas());
        if (canvas)
            canvas->setBorderRadius(m_canvasBorderRadius);
    }

    m_graph->setTitle(m_title);
    m_graph->setTitleVisible(m_isTitleVisible);
    m_graph->titleLabel()->setFont(m_titleFont);
    QPalette titlePalette(m_graph->titleLabel()->palette());
    titlePalette.setColor(QPalette::Active, QPalette::Text, m_titleColor);
    m_graph->titleLabel()->setPalette(titlePalette);

    m_graph->titleLabel()->setOpacity(m_titleOpacity);

    // legend & item settings
    m_graph->setLegendFont(m_legendFont);
    m_graph->setLegendPosition(m_legendPosition);
    m_graph->setLegendFontColor(m_legendColor);
    m_graph->setLegendVisible(m_isLegendVisible);
    m_graph->setLegendItemVisible(m_isLegendItemVisible);
    m_graph->setLegendItemFont(m_legendItemFont);
    m_graph->setLegendItemBgColor(m_legendItemBgColor);
    m_graph->setLegendItemFontColor(m_legendItemFontColor);
    m_graph->setLegendItemBorderColor(m_legendItemBorderColor);
    m_graph->setLegendItemColumns(m_legendItemColumns);
    m_graph->setLegendItemHAlign(m_legendItemHAlign);
    m_graph->setLegendItemVAlign(m_legendItemVAlign);
    m_graph->setLegendItemBgMode(m_legendItemBgMode);

    // axis settings
    m_graph->setAxisTickColor(m_axisTickColor);
    m_graph->setAxisLabelColor(m_axisLabelColor);
    m_graph->setAxisTickWidth(m_axisTickWidth);
    m_graph->setAxisLabelFont(m_axisLabelFont);
    m_graph->setMajorLabelsVisible(m_axisMajorLabelsVisible);
    m_graph->setMinorLabelsVisible(m_axisMinorLabelsVisible);
    m_graph->setPlusSignVisible(m_axisPlusSignVisible);
    m_graph->setMinusSignVisible(m_axisMinusSignVisible);
    m_graph->setPrefix(m_axisLabelPrefix);
    m_graph->setSuffix(m_axisLabelSuffix);
    m_graph->setDecimalWidth(m_axisDecimalWidth);
    m_graph->setAxisNotation(m_axisNotation);
    m_graph->setAxisTitleType((Graph::AxisTitleType) m_axisTitleType);

    // barchart settings
    m_graph->setBarChartFillStyle(m_barChartFillPattern);
    m_graph->setBarChartLineWidth(m_barChartLineWidth);
    m_graph->setBarChartOpacity(m_barChartFillOpacity);
    m_graph->setBarChartStyle(m_barChartFillStyle);
    m_graph->setBarChartBorderPen(m_barChartBorderPen);

    QwtPlotItemList legendItems = m_graph->itemList(QwtPlotItem::Rtti_PlotLegend);
    foreach (QwtPlotItem* item, legendItems) {
        QwtPlotLegendItem* legendItem = static_cast<QwtPlotLegendItem*>(item);
        legendItem->setFont(m_legendFont);
        legendItem->setBackgroundBrush(QBrush(m_legendColor));
    }

    m_graph->setLineWidth(m_curveLineWidth);
    m_graph->setLineOpacity(m_curveLineOpacity);

    // bar label
    m_graph->setLabelVisible(m_isBarLabelVisible);
    m_graph->setLabelColor(m_barLabelColor);
    m_graph->setLabelFont(m_barLabelFont);
    m_graph->setLabelOpacity(m_barLabelOpacity);
    m_graph->setLabelPosition(m_barLabelPosition);
    m_graph->setLabelOffsetX(m_barLabelOffsetX);
    m_graph->setLabelOffsetY(m_barLabelOffsetY);

    // grid
    m_graph->setGridVisible(m_isGridVisible);
    m_graph->setMajorGridVisible(m_isMajorGridVisible);
    m_graph->setMinorGridVisible(m_isMinorGridVisible);
    m_graph->setGridPenColor(m_gridPen.color());
    m_graph->setGridPenStyle(m_gridPen.style());
    m_graph->setGridPenWidth(m_gridPen.width());
    m_graph->setHorizontalGridVisible(m_isGridHorizontalVisible);
    m_graph->setVerticalGridVisible(m_isGridVerticalVisible);

    // symbol
    m_graph->setSymbol(m_symbolStyle);
    m_graph->setSymbolColor(m_symbolColor);
    m_graph->setSymbolEdgeColor(m_symbolEdgeColor);
    m_graph->setSymbolOpacity(m_symbolOpacity);
    m_graph->setSymbolSize(m_symbolSize);
    m_graph->setSymbolWidth(m_symbolWidth);

    // dropline
    m_graph->setHorizontalDroplineVisible(m_isHorizontalDroplineVisible);
    m_graph->setVerticalDroplineVisible(m_isVerticalDroplineVisible);
    m_graph->setDroplinePen(m_droplinePen);
    m_graph->setDroplineOpacity(m_droplineOpacity);

    // group
    const int penCount = GuiUtils::ColorComboBox::defaultColors().size();
    QList<QColor> lineClrs;
    for (int i = 0; i < penCount; ++i)
        lineClrs.push_back(qvariant_cast<QColor>(m_groupLineColors.value(QString("%1").arg(i))));
    m_graph->setGroupLineColors(lineClrs);

    QList<QColor> patternClrs;
    for (int i = 0; i < penCount; ++i)
        patternClrs.push_back(qvariant_cast<QColor>(m_groupPatternColors.value(QString("%1").arg(i))));
    m_graph->setGroupPatternColors(patternClrs);

    QList<Qt::PenStyle> penStyles;
    for (int i = 0; i < penCount; ++i)
        penStyles.push_back(static_cast<Qt::PenStyle>(m_groupPenStyles.value(QString("%1").arg(i)).toInt()));
    m_graph->setGroupPenStyle(penStyles);

    QList<QwtSymbol::Style> bubbleStyles;
    for (int i = 0; i < penCount; ++i)
        bubbleStyles.push_back(static_cast<QwtSymbol::Style>(m_groupBubbleStyles.value(QString("%1").arg(i)).toInt()));
    m_graph->setGroupBubbleStyle(bubbleStyles);

    m_graph->setUseGroupLineColor(m_useGroupLineColor);
    m_graph->setUseGroupPatternColor(m_useGroupPatternColor);
    m_graph->setUseGroupPenStyle(m_useGroupPenStyle);
    m_graph->setUseGroupBubbleStyle(m_useGroupBubbleStyle);

    // curve labels
    m_graph->setCurveLabelColor(m_curveLabelData.color);
    m_graph->setCurveLabelVisible(m_curveLabelData.isVisible);
    m_graph->setCurveLabelOpacity(m_curveLabelData.opacity);
    m_graph->setCurveLabelOffsetX(m_curveLabelData.xOffset);
    m_graph->setCurveLabelOffsetY(m_curveLabelData.yOffset);
    m_graph->setCurveLabelFont(m_curveLabelData.font);
    m_graph->setCurveLabelPosition(m_curveLabelData.pos);

    // bubble
    m_graph->setBubbleSize(m_bubbleSize);
    m_graph->setBubbleEdgeWidth(m_bubbleEdgeWidth);
    m_graph->setBubbleOpacity(m_bubbleOpacity);

    // pie
    m_graph->setPieStartAngle(m_pieStartAngle);
    m_graph->setPieThickness(m_pieThickness);
    m_graph->setPieCounterClockwise(m_isCounterClockwise);
    m_graph->setPieViewAngle(m_pieViewAngle);
    m_graph->setPieRadiusRatio(m_pieRadiusRatio);
    m_graph->setPieDisplacementRatio(m_pieDisplacementRatio);

    // error bar
    m_graph->setErrorBarPlusVisible(m_isErrorBarPlusVisible);
    m_graph->setErrorBarMinusVisible(m_isErrorBarMinusVisible);
    m_graph->setErrorBarLineColor(m_errorBarLineColor);
    m_graph->setErrorBarLineWidth(m_errorBarLineWidth);
    m_graph->setErrorBarHandleWidth(m_errorBarHandleWidth);
    m_graph->setErrorBarLineOpacity(m_errorBarLineOpacity);

    // update
    m_graph->update();
    m_graph->replot();

    emit changed();
}

/*!
 * \brief 从配置文件中读取配置信息.
 */
void GraphProperty::readSettings()
{
    QSettings* settings = CommonUtils::SettingsManager::instance()->generalSettings();

    // beginning
    settings->beginGroup(ComDataGui::Group_VizGraph);

    // Axis section
    settings->beginGroup(ComDataGui::Group_Axis);
    setAxisLabelColor(qvariant_cast<QColor>(settings->value(ComDataGui::Axis_LabelColor, DefaultProperty::axisLabelColor())));
    setAxisTickColor(qvariant_cast<QColor>(settings->value(ComDataGui::Axis_TickColor, DefaultProperty::axisTickColor())));
    setAxisTickWidth((settings->value(ComDataGui::Axis_TickWidth, DefaultProperty::axisTickWidth())).toInt());
    setAxisTitleType((settings->value(ComDataGui::Axis_TitleType, DefaultProperty::axisTitleType())).toInt());
    setAxisLabelFont(qvariant_cast<QFont>(settings->value(ComDataGui::Axis_LabelFont, DefaultProperty::axisLabelFont())));
    setGridVisible(settings->value(ComDataGui::Axis_ShowGrid, DefaultProperty::isGridVisible()).toBool());
    setHorizontalGridVisible(settings->value(ComDataGui::Axis_ShowHorizontalGrid, DefaultProperty::isHorizontalGridVisible()).toBool());
    setVerticalGridVisible(settings->value(ComDataGui::Axis_ShowVerticalGrid, DefaultProperty::isVerticalGridVisible()).toBool());
    setMajorGridVisible(settings->value(ComDataGui::Axis_ShowMajorGrid, DefaultProperty::isMajorGridVisible()).toBool());
    setMinorGridVisible(settings->value(ComDataGui::Axis_ShowMinorGrid, DefaultProperty::isMinorGridVisible()).toBool());
    setGridPenColor(qvariant_cast<QColor>(settings->value(ComDataGui::Axis_GridColor, DefaultProperty::gridPen().color())));
    setGridPenWidth(settings->value(ComDataGui::Axis_GridWidth, DefaultProperty::gridPen().width()).toInt());
    setGridPenStyle(static_cast<Qt::PenStyle>(settings->value(ComDataGui::Axis_GridStyle, (int)DefaultProperty::gridPen().style()).toInt()));
    setMajorLabelsVisible(settings->value(ComDataGui::Axis_ShowMajorLabels, DefaultProperty::isAxisMajorLabelsVisible()).toBool());
    setMinorLabelsVisible(settings->value(ComDataGui::Axis_ShowMinorLabels, DefaultProperty::isAxisMinorLabelsVisible()).toBool());
    setDecimalWidth(settings->value(ComDataGui::Axis_DecimalWidth, DefaultProperty::axisDecimalWidth()).toInt());
    setAxisNotation((QwtAbstractScaleDraw::Notation)settings->value(ComDataGui::Axis_Notation, DefaultProperty::axisNotation()).toInt());
    setPrefix(settings->value(ComDataGui::Axis_Prefix, DefaultProperty::axisLabelPrefix()).toString());
    setSuffix(settings->value(ComDataGui::Axis_Suffix, DefaultProperty::axisLabelSuffix()).toString());
    setPlusSignVisible(settings->value(ComDataGui::Axis_ShowPlusSign, DefaultProperty::isAxisPlusSignVisible()).toBool());
    setMinusSignVisible(settings->value(ComDataGui::Axis_ShowMinusSign, DefaultProperty::isAxisMinusSignVisible()).toBool());
    settings->endGroup();

    // Bubble section
    settings->beginGroup(ComDataGui::Group_Bubbles);
    setBubbleSize(settings->value(ComDataGui::Bubble_Size, DefaultProperty::bubbleSize()).toInt());
    setBubbleEdgeWidth(settings->value(ComDataGui::Bubble_EdgeWidth, DefaultProperty::bubbleEdgeWidth()).toInt());
    setBubbleOpacity(settings->value(ComDataGui::Bubble_Opacity, DefaultProperty::bubbleOpacity()).toDouble());
    settings->endGroup();

    // Pie section
    settings->beginGroup(ComDataGui::Group_Pies);
    setPieViewAngle(settings->value(ComDataGui::Pie_ViewAngle, DefaultProperty::pieViewAngle()).toInt());
    setPieStartAngle(settings->value(ComDataGui::Pie_StartAngle, DefaultProperty::pieStartAngle()).toInt());
    setPieThickness(settings->value(ComDataGui::Pie_Thickness, DefaultProperty::pieThickness()).toInt());
    setPieCounterClockwise(settings->value(ComDataGui::Pie_CounterClockwise, DefaultProperty::isCounterClockwise()).toBool());
    setPieRadiusRatio(settings->value(ComDataGui::Pie_RadiusRatio, DefaultProperty::pieRadiusRatio()).toDouble());
    setPieDisplacementRatio(settings->value(ComDataGui::Pie_DisplacementRatio, DefaultProperty::pieDisplacementRatio()).toDouble());
    settings->endGroup();

    // Legend section
    settings->beginGroup(ComDataGui::Group_Legend);
    setLegendVisible(settings->value(ComDataGui::Legend_ShowLegend).toBool());
    setLegendFont(qvariant_cast<QFont>(settings->value(ComDataGui::Legend_Font, DefaultProperty::legendFont())));
    setLegendColor(qvariant_cast<QColor>(settings->value(ComDataGui::Legend_Color, DefaultProperty::legendColor())));
    setLegendPosition(settings->value(ComDataGui::Legend_Position, DefaultProperty::legendPosition()).toInt());
    setLegendItemVisible(settings->value(ComDataGui::Legend_ShowItem, DefaultProperty::isLegendItemVisible()).toBool());
    setLegendItemFont(qvariant_cast<QFont>(settings->value(ComDataGui::Legend_ItemFont, DefaultProperty::legendItemFont())));
    setLegendItemBgColor(qvariant_cast<QColor>(settings->value(ComDataGui::Legend_ItemBgColor, DefaultProperty::legendItemBackgroundColor())));
    setLegendItemColor(qvariant_cast<QColor>(settings->value(ComDataGui::Legend_ItemColor, DefaultProperty::legendItemFontColor())));
    setLegendItemBorderColor(qvariant_cast<QColor>(settings->value(ComDataGui::Legend_BorderColor, DefaultProperty::legendItemBorderColor())));
    setLegendItemColumns(settings->value(ComDataGui::Legend_Columns, DefaultProperty::legendItemColumns()).toInt());
    setLegendItemHAlign(settings->value(ComDataGui::Legend_HorizontalAlignment, DefaultProperty::legendItemHAlign()).toInt());
    setLegendItemVAlign(settings->value(ComDataGui::Legend_VerticalAlignment, DefaultProperty::legendItemVAlign()).toInt());
    setLegendItemBgMode(settings->value(ComDataGui::Legend_ItemMode, DefaultProperty::legendItemBackgroundMode()).toInt());
    settings->endGroup();

    // ErrorBar section
    settings->beginGroup(ComDataGui::Group_ErrorBar);
    setErrorBarPlusVisible(settings->value(ComDataGui::ErrorBar_PlusVisible, DefaultProperty::isErrorBarPlusVisible()).toBool());
    setErrorBarMinusVisible(settings->value(ComDataGui::ErrorBar_MinusVisible, DefaultProperty::isErrorBarMinusVisible()).toBool());
    setErrorBarLineColor(qvariant_cast<QColor>(settings->value(ComDataGui::ErrorBar_Color, DefaultProperty::errorBarLineColor())));
    setErrorBarLineWidth(settings->value(ComDataGui::ErrorBar_LineWidth, DefaultProperty::errorBarLineWidth()).toInt());
    setErrorBarHandleWidth(settings->value(ComDataGui::ErrorBar_HandleWidth, DefaultProperty::errorBarHandleWidth()).toInt());
    setErrorBarLineOpacity(settings->value(ComDataGui::ErrorBar_LineOpacity, DefaultProperty::errorBarLineOpacity()).toFloat());
    settings->endGroup();

    // BarChart section
    settings->beginGroup(ComDataGui::Group_BarChart);
    setBarChartFillOpacity(settings->value(ComDataGui::BarChart_FillOpacity, DefaultProperty::barChartFillOpacity()).toDouble());
    setBarChartFillPattern(static_cast<Qt::BrushStyle>(settings->value(ComDataGui::BarChart_FillPattern, (int)DefaultProperty::barChartFillPattern()).toInt()));
    setBarChartLineWidth(settings->value(ComDataGui::BarChart_LineWidth, DefaultProperty::barChartLineWidth()).toInt());
    setBarChartFillStyle(static_cast<QwtColumnSymbol::FrameStyle>(settings->value(ComDataGui::BarChart_FillStyle, DefaultProperty::barChartFillStyle()).toInt()));
    setBarLabelVisible(settings->value(ComDataGui::BarChart_LabelVisible, DefaultProperty::isBarLabelVisible()).toBool());
    setBarLabelFont(qvariant_cast<QFont>(settings->value(ComDataGui::BarChart_LabelFont, DefaultProperty::barLabelFont())));
    setBarLabelColor(qvariant_cast<QColor>(settings->value(ComDataGui::BarChart_LabelColor, DefaultProperty::barLabelColor())));
    setBarLabelOpacity(settings->value(ComDataGui::BarChart_LabelOpacity, DefaultProperty::barLabelOpacity()).toDouble());
    setBarLabelOffsetX(settings->value(ComDataGui::BarChart_LabelOffsetX, DefaultProperty::barLabelOffsetX()).toInt());
    setBarLabelOffsetY(settings->value(ComDataGui::BarChart_LabelOffsetY, DefaultProperty::barLabelOffsetY()).toInt());
    setBarLabelPosition(static_cast<QwtPlot::LabelPosition>(settings->value(ComDataGui::BarChart_LabelPosition, DefaultProperty::barLabelPosition()).toInt()));
    setBarChartBorderPen(qvariant_cast<QPen>(settings->value(ComDataGui::BarChart_BorderPen, DefaultProperty::barChartBorderPen())));
    settings->endGroup();

    // Curve section
    settings->beginGroup(ComDataGui::Group_Curves);
    setCurveLineWidth(settings->value(ComDataGui::Curve_LineWidth, DefaultProperty::curveLineWidth()).toInt());
    setCurveLineOpacity(settings->value(ComDataGui::Curve_LineOpacity, DefaultProperty::curveLineOpacity()).toDouble());
    setAntiAliasingCurve(settings->value(ComDataGui::Curve_AntiAliasing, DefaultProperty::isAntiAliasingCurve()).toBool());
    setSymbol(static_cast<QwtSymbol::Style>(settings->value(ComDataGui::Curve_Symbol, DefaultProperty::symbolStyle()).toInt()));
    setSymbolColor(qvariant_cast<QColor>(settings->value(ComDataGui::Curve_SymbolColor, DefaultProperty::symbolColor())));
    setSymbolOpacity(settings->value(ComDataGui::Curve_SymbolOpacity, DefaultProperty::symbolOpacity()).toDouble());
    setSymbolSize(settings->value(ComDataGui::Curve_SymbolSize, DefaultProperty::symbolSize()).toInt());
    setSymbolWidth(settings->value(ComDataGui::Curve_SymbolEdgeWidth, DefaultProperty::symbolWidth()).toInt());
    setSymbolEdgeColor(qvariant_cast<QColor>(settings->value(ComDataGui::Curve_SymbolEdgeColor, DefaultProperty::symbolEdgeColor())));
    setHorizontalDroplineVisible(settings->value(ComDataGui::Curve_DroplineHorizontalVisible, DefaultProperty::isHorizontalDroplineVisible()).toBool());
    setVerticalDroplineVisible(settings->value(ComDataGui::Curve_DroplineVerticalVisible, DefaultProperty::isVerticalDroplineVisible()).toBool());
    setDroplinePen(qvariant_cast<QPen>(settings->value(ComDataGui::Curve_DroplinePen, DefaultProperty::droplinePen())));
    setDroplineOpacity(settings->value(ComDataGui::Curve_DroplineOpacity, DefaultProperty::droplineOpacity()).toDouble());

    const CurveLabelData defaultCLD = DefaultProperty::curveLabelData();
    setCurveLabelVisible(settings->value(ComDataGui::Curve_LabelVisible, defaultCLD.isVisible).toBool());
    setCurveLabelFont(qvariant_cast<QFont>(settings->value(ComDataGui::Curve_LabelFont, defaultCLD.font)));
    setCurveLabelColor(qvariant_cast<QColor>(settings->value(ComDataGui::Curve_LabelColor, defaultCLD.font)));
    setCurveLabelPosition(static_cast<QwtPlotCurve::LabelPosition>(settings->value(ComDataGui::Curve_LabelPosition, defaultCLD.pos).toInt()));
    setCurveLabelOffsetX(settings->value(ComDataGui::Curve_LabelOffsetX, defaultCLD.xOffset).toInt());
    setCurveLabelOffsetY(settings->value(ComDataGui::Curve_LabelOffsetY, defaultCLD.yOffset).toInt());
    setCurveLabelOpacity(settings->value(ComDataGui::Curve_LabelOpacity, defaultCLD.opacity).toDouble());

    settings->endGroup();

    // GraphWindow section
    settings->beginGroup(ComDataGui::Group_GraphWindow);
    QColor bgColor = qvariant_cast<QColor>(settings->value(ComDataGui::GraphWindow_BackgroundStartColor, DefaultProperty::backgroundStartColor()));
    setBackgroundStartColor(bgColor);
    QColor bgSecondColor = qvariant_cast<QColor>(settings->value(ComDataGui::GraphWindow_BackgroundStopColor, DefaultProperty::backgroundStopColor()));
    setBackgroundStopColor(bgSecondColor);
    setBackgroundLightness(settings->value(ComDataGui::GraphWindow_BackgroundLightness, DefaultProperty::backgroundLightness()).toInt());
    setGraphWindowGradientFill(settings->value(ComDataGui::GraphWindow_GradientFill, DefaultProperty::graphWindowGradientFill()).toInt());
    setConnectAxisBreak(settings->value(ComDataGui::GraphWindow_ConnectAxisBreak, DefaultProperty::isConnectAxisBreak()).toBool());
    setConnectMissingData(settings->value(ComDataGui::GraphWindow_ConnectMissingData, DefaultProperty::isConnectMissingData()).toBool());
    setBackgroundGradientFill((ComDataGui::Utils::GradientType) settings->value(ComDataGui::GraphWindow_BackgroundGraidentFill, DefaultProperty::backgroundGradientType()).toInt());
    setCanvasGradientFill((ComDataGui::Utils::GradientType) settings->value(ComDataGui::GraphWindow_CanvasGradientFill, DefaultProperty::canvasGradientFill()).toInt());
    setGradientFillMode(settings->value(ComDataGui::GraphWindow_GradientFillMode, DefaultProperty::graphWindowGradientFill()).toInt());
    settings->endGroup();

    // Graph section
    settings->beginGroup(ComDataGui::Group_Graph);
    QColor canvasStartColor = qvariant_cast<QColor>(settings->value(ComDataGui::Graph_CanvasStartColor, DefaultProperty::canvasStartColor()));
    setCanvasStartColor(canvasStartColor);
    QColor canvasStopColor = qvariant_cast<QColor>(settings->value(ComDataGui::Graph_CanvasStopColor, DefaultProperty::canvasStopColor()));
    setCanvasStopColor(canvasStopColor);
    setCanvasLightness(settings->value(ComDataGui::Graph_CanvasLightness, DefaultProperty::canvasLightness()).toInt());
    setCanvasGradientFillMode(settings->value(ComDataGui::Graph_CanvasGradientFillMode, DefaultProperty::canvasGradientFill()).toInt());

    setXAxisVisible(settings->value(ComDataGui::Graph_XAxisVisible, DefaultProperty::isXAxisVisible()).toBool());
    setYAxisVisible(settings->value(ComDataGui::Graph_YAxisVisible, DefaultProperty::isYAxisVisible()).toBool());
    setHorizontalTitleVisible(settings->value(ComDataGui::Graph_XLabelVisible, DefaultProperty::isHTitleVisible()).toBool());
    setVerticalTitleVisible(settings->value(ComDataGui::Graph_YLabelVisible, DefaultProperty::isVTitleVisible()).toBool());
    setDataVisible(settings->value(ComDataGui::Graph_DataVisible, DefaultProperty::isDataVisible()).toBool());
    setTitleVisible(settings->value(ComDataGui::Graph_TitleVisible, DefaultProperty::isTitleVisible()).toBool());
    setTitleFont(qvariant_cast<QFont>(settings->value(ComDataGui::Graph_TitleFont, DefaultProperty::titleFont())));
    setTitleText(settings->value(ComDataGui::Graph_TitleText, DefaultProperty::title()).toString());
    setTitleColor(qvariant_cast<QColor>(settings->value(ComDataGui::Graph_TitleColor, DefaultProperty::titleColor())));
    setTitleOpacity(settings->value(ComDataGui::Graph_TitleOpacity, DefaultProperty::titleOpacity()).toDouble());
    setMaxCurveDataCount(settings->value(ComDataGui::Graph_MaxPointsPerCurve, DefaultProperty::maxCurveDataCount()).toInt());
    setCanvasBorderRadius(settings->value(ComDataGui::Graph_CanvasBorderRadius, DefaultProperty::canvasBorderRadius()).toInt());
    settings->endGroup();

    // MultiItem
    settings->beginGroup(ComDataGui::Group_MultiItem);
    setUseGroupLineColor(settings->value(ComDataGui::MultiItem_UseGroupLineColors, DefaultProperty::useGroupLineColor()).toBool());
    setUseGroupPatternColor(settings->value(ComDataGui::MultiItem_UseGroupPatternColors, DefaultProperty::useGroupPatternColor()).toBool());
    setUseGroupPenStyle(settings->value(ComDataGui::MultiItem_UseGroupPenStyles, DefaultProperty::useGroupPenStyle()).toBool());
    setUseGroupBubbleStyle(settings->value(ComDataGui::MultiItem_UseGroupBubbleStyles, DefaultProperty::useGroupBubbleStyle()).toBool());

    // line colors
    QVariant lineClrVar = settings->value(ComDataGui::MultiItem_GroupLineColors, DefaultProperty::groupLineColors());
    QMap<QString, QVariant> lineClrMap = lineClrVar.toMap();
    setGroupLineColors(lineClrMap);

    // pattern colors
    QVariant patternClrVar = settings->value(ComDataGui::MultiItem_GroupPatternColors, DefaultProperty::groupPatternColors());
    QMap<QString, QVariant> patternClrMap = patternClrVar.toMap();
    setGroupPatternColors(patternClrMap);

    // line styles
    QVariant lineStyleVar = settings->value(ComDataGui::MultiItem_GroupPenStyles, DefaultProperty::groupPenStyles());
    QMap<QString, QVariant> lineStyleMap = lineStyleVar.toMap();
    setGroupPenStyles(lineStyleMap);

    // bubble styles
    QVariant bubbleStyleVar = settings->value(ComDataGui::MultiItem_GroupBubbleStyles, DefaultProperty::groupBubbleStyles());
    QMap<QString, QVariant> bubbleStyleMap = bubbleStyleVar.toMap();
    setGroupBubbleStyles(bubbleStyleMap);

    settings->endGroup();

    // ending
    settings->endGroup();
}

/*!
 * \brief 将属性值写到配置文件中.
 */
void GraphProperty::writeSettings()
{
    QSettings* settings = CommonUtils::SettingsManager::instance()->generalSettings();

    // beginning
    settings->beginGroup(ComDataGui::Group_VizGraph);

    // Axis section
    settings->beginGroup(ComDataGui::Group_Axis);
    settings->setValue(ComDataGui::Axis_LabelColor, axisLabelColor());
    settings->setValue(ComDataGui::Axis_TickColor, axisTickColor());
    settings->setValue(ComDataGui::Axis_TickWidth, axisTickWidth());
    settings->setValue(ComDataGui::Axis_TitleType, axisTitleType());
    settings->setValue(ComDataGui::Axis_LabelFont, axisLabelFont());
    settings->setValue(ComDataGui::Axis_ShowGrid, isGridVisible());
    settings->setValue(ComDataGui::Axis_ShowHorizontalGrid, isHorizontalGridVisible());
    settings->setValue(ComDataGui::Axis_ShowVerticalGrid, isVerticalGridVisible());
    settings->setValue(ComDataGui::Axis_ShowMajorGrid, isMajorGridVisible());
    settings->setValue(ComDataGui::Axis_ShowMinorGrid, isMinorGridVisible());
    settings->setValue(ComDataGui::Axis_GridColor, gridPenColor());
    settings->setValue(ComDataGui::Axis_GridWidth, gridPenWidth());
    settings->setValue(ComDataGui::Axis_GridStyle, (int)gridPenStyle());
    settings->setValue(ComDataGui::Axis_ShowMajorLabels, isMajorLabelsVisible());
    settings->setValue(ComDataGui::Axis_ShowMinorLabels, isMinorLabelsVisible());
    settings->setValue(ComDataGui::Axis_DecimalWidth, decimalWidth());
    settings->setValue(ComDataGui::Axis_Notation, axisNotation());
    settings->setValue(ComDataGui::Axis_Prefix, prefix());
    settings->setValue(ComDataGui::Axis_Suffix, suffix());
    settings->setValue(ComDataGui::Axis_ShowPlusSign, isPlusSignVisible());
    settings->setValue(ComDataGui::Axis_ShowMinusSign, isMinusSignVisible());
    settings->endGroup(); // Axis

    // Bubble section
    settings->beginGroup(ComDataGui::Group_Bubbles);
    settings->setValue(ComDataGui::Bubble_Size, bubbleSize());
    settings->setValue(ComDataGui::Bubble_EdgeWidth, bubbleEdgeWidth());
    settings->setValue(ComDataGui::Bubble_Opacity, bubbleOpacity());
    settings->endGroup(); // Bubbles

    // Pie section
    settings->beginGroup(ComDataGui::Group_Pies);
    settings->setValue(ComDataGui::Pie_ViewAngle, pieViewAngle());
    settings->setValue(ComDataGui::Pie_StartAngle, pieStartAngle());
    settings->setValue(ComDataGui::Pie_Thickness, pieThickness());
    settings->setValue(ComDataGui::Pie_CounterClockwise, isPieCounterClockwise());
    settings->setValue(ComDataGui::Pie_RadiusRatio, pieRadiusRatio());
    settings->setValue(ComDataGui::Pie_DisplacementRatio, pieDisplacementRatio());
    settings->endGroup();

    // Legend section
    settings->beginGroup(ComDataGui::Group_Legend);
    settings->setValue(ComDataGui::Legend_ShowLegend, isLegendVisible());
    settings->setValue(ComDataGui::Legend_Font, legendFont());
    settings->setValue(ComDataGui::Legend_Color, legendColor());
    settings->setValue(ComDataGui::Legend_Position, legendPosition());
    settings->setValue(ComDataGui::Legend_ShowItem, isLegendItemVisible());
    settings->setValue(ComDataGui::Legend_ItemFont, legendItemFont());
    settings->setValue(ComDataGui::Legend_ItemBgColor, legendItemBgColor());
    settings->setValue(ComDataGui::Legend_ItemColor, legendItemColor());
    settings->setValue(ComDataGui::Legend_BorderColor, legendItemBorderColor());
    settings->setValue(ComDataGui::Legend_Columns, legendItemColumns());
    settings->setValue(ComDataGui::Legend_HorizontalAlignment, legendItemHAlign());
    settings->setValue(ComDataGui::Legend_VerticalAlignment, legendItemVAlign());
    settings->setValue(ComDataGui::Legend_ItemMode, legendItemBgMode());
    settings->endGroup(); // Legend

    // ErrorBar section
    settings->beginGroup(ComDataGui::Group_ErrorBar);
    settings->setValue(ComDataGui::ErrorBar_PlusVisible, isErrorBarPlusVisible());
    settings->setValue(ComDataGui::ErrorBar_MinusVisible, isErrorBarMinusVisible());
    settings->setValue(ComDataGui::ErrorBar_Color, errorBarLineColor());
    settings->setValue(ComDataGui::ErrorBar_LineWidth, errorBarLineWidth());
    settings->setValue(ComDataGui::ErrorBar_HandleWidth, errorBarHandleWidth());
    settings->setValue(ComDataGui::ErrorBar_LineOpacity, errorBarLineOpacity());
    settings->endGroup();

    // BarChart section
    settings->beginGroup(ComDataGui::Group_BarChart);
    settings->setValue(ComDataGui::BarChart_FillOpacity, barChartFillOpacity());
    settings->setValue(ComDataGui::BarChart_FillPattern, (int)barChartFillPattern());
    settings->setValue(ComDataGui::BarChart_LineWidth, barChartLineWidth());
    settings->setValue(ComDataGui::BarChart_FillStyle, barChartFillStyle());
    settings->setValue(ComDataGui::BarChart_LabelVisible, isBarLabelVisible());
    settings->setValue(ComDataGui::BarChart_LabelFont, barLabelFont());
    settings->setValue(ComDataGui::BarChart_LabelOpacity, barLabelOpacity());
    settings->setValue(ComDataGui::BarChart_LabelColor, barLabelColor());
    settings->setValue(ComDataGui::BarChart_LabelOffsetX, barLabelOffsetX());
    settings->setValue(ComDataGui::BarChart_LabelOffsetY, barLabelOffsetY());
    settings->setValue(ComDataGui::BarChart_LabelPosition, barLabelPosition());
    settings->setValue(ComDataGui::BarChart_BorderPen, barChartBorderPen());
    settings->endGroup(); // BarChart

    // Curve section
    settings->beginGroup(ComDataGui::Group_Curves);
    settings->setValue(ComDataGui::Curve_LineWidth, curveLineWidth());
    settings->setValue(ComDataGui::Curve_LineOpacity, curveLineOpacity());
    settings->setValue(ComDataGui::Curve_AntiAliasing, isAntiAliasingCurve());
    settings->setValue(ComDataGui::Curve_Symbol, (int)symbol());
    settings->setValue(ComDataGui::Curve_SymbolColor, symbolColor());
    settings->setValue(ComDataGui::Curve_SymbolEdgeColor, symbolEdgeColor());
    settings->setValue(ComDataGui::Curve_SymbolEdgeWidth, symbolWidth());
    settings->setValue(ComDataGui::Curve_SymbolOpacity, symbolOpacity());
    settings->setValue(ComDataGui::Curve_SymbolSize, symbolSize());
    settings->setValue(ComDataGui::Curve_DroplineHorizontalVisible, isHorizontalDroplineVisible());
    settings->setValue(ComDataGui::Curve_DroplineVerticalVisible, isVerticalDroplineVisible());
    settings->setValue(ComDataGui::Curve_DroplineOpacity, droplineOpacity());
    settings->setValue(ComDataGui::Curve_DroplinePen, droplinePen());
    settings->setValue(ComDataGui::Curve_LabelVisible, isCurveLabelVisible());
    settings->setValue(ComDataGui::Curve_LabelFont, curveLabelFont());
    settings->setValue(ComDataGui::Curve_LabelColor, curveLabelColor());
    settings->setValue(ComDataGui::Curve_LabelPosition, curveLabelPosition());
    settings->setValue(ComDataGui::Curve_LabelOffsetX, curveLabelOffsetX());
    settings->setValue(ComDataGui::Curve_LabelOffsetY, curveLabelOffsetY());
    settings->setValue(ComDataGui::Curve_LabelOpacity, curveLabelOpacity());
    settings->endGroup(); // Curves

    // GraphWindow section
    settings->beginGroup(ComDataGui::Group_GraphWindow);
    settings->setValue(ComDataGui::GraphWindow_GradientFill, graphWindowGradientFill());
    settings->setValue(ComDataGui::GraphWindow_BackgroundStartColor, backgroundStartColor());
    settings->setValue(ComDataGui::GraphWindow_BackgroundStopColor, backgroundStopColor());
    settings->setValue(ComDataGui::GraphWindow_BackgroundLightness, backgroundLightness());
    settings->setValue(ComDataGui::GraphWindow_ConnectAxisBreak, isConnectAxisBreak());
    settings->setValue(ComDataGui::GraphWindow_ConnectMissingData, isConnectMissingData());
    settings->setValue(ComDataGui::GraphWindow_BackgroundGraidentFill, backgroundGradientFill());
    settings->setValue(ComDataGui::GraphWindow_CanvasGradientFill, canvasGradientFill());
    settings->setValue(ComDataGui::GraphWindow_GradientFillMode, gradientFillMode());
    settings->endGroup(); // GraphWindow

    // Graph section
    settings->beginGroup(ComDataGui::Group_Graph);
    settings->setValue(ComDataGui::Graph_CanvasStartColor, canvasStartColor());
    settings->setValue(ComDataGui::Graph_CanvasStopColor, canvasStopColor());
    settings->setValue(ComDataGui::Graph_CanvasLightness, canvasLightness());
    settings->setValue(ComDataGui::Graph_CanvasGradientFillMode, canvasGradientFillMode());
    settings->setValue(ComDataGui::Graph_DataVisible, isDataVisible());
    settings->setValue(ComDataGui::Graph_XAxisVisible, isXAxisVisible());
    settings->setValue(ComDataGui::Graph_YAxisVisible, isYAxisVisible());
    settings->setValue(ComDataGui::Graph_XLabelVisible, isHorizontalTitleVisible());
    settings->setValue(ComDataGui::Graph_YLabelVisible, isVerticalTitleVisible());
    settings->setValue(ComDataGui::Graph_TitleVisible, isTitleVisible());
    settings->setValue(ComDataGui::Graph_TitleFont, titleFont());
    settings->setValue(ComDataGui::Graph_TitleText, titleText());
    settings->setValue(ComDataGui::Graph_TitleColor, titleColor());
    settings->setValue(ComDataGui::Graph_TitleOpacity, titleOpacity());
    settings->setValue(ComDataGui::Graph_MaxPointsPerCurve, maxCurveDataCount());
    settings->setValue(ComDataGui::Graph_CanvasBorderRadius, canvasBorderRadius());
    settings->endGroup(); // Graph

    // MultiItem
    settings->beginGroup(ComDataGui::Group_MultiItem);
    settings->setValue(ComDataGui::MultiItem_UseGroupLineColors, useGroupLineColor());
    settings->setValue(ComDataGui::MultiItem_UseGroupPatternColors, useGroupPatternColor());
    settings->setValue(ComDataGui::MultiItem_UseGroupPenStyles, useGroupPenStyle());
    settings->setValue(ComDataGui::MultiItem_UseGroupBubbleStyles, useGroupBubbleStyle());

    settings->setValue(ComDataGui::MultiItem_GroupLineColors, groupLineColors());
    settings->setValue(ComDataGui::MultiItem_GroupPatternColors, groupPatternColors());
    settings->setValue(ComDataGui::MultiItem_GroupPenStyles, groupPenStyle());
    settings->setValue(ComDataGui::MultiItem_GroupBubbleStyles, groupBubbleStyles());

    settings->endGroup(); // MultiItem

    // ending
    settings->endGroup();
    CommonUtils::SettingsManager::instance()->saveSettings();
}

void GraphProperty::setGraph(Graph* g)
{
    if (!g) {
        qDebug() << __FUNCTION__ << "null graph, quit.";
        return;
    }

    m_graph = g;

    // retrieve properties
    m_horizontalTitle = m_graph->axisTitle(QwtPlot::xBottom);
    m_verticalTitle = m_graph->axisTitle(QwtPlot::yLeft);

    apply();
}

Graph* GraphProperty::graph() const
{
    return m_graph;
}

void GraphProperty::setTitleText(const QString &text)
{
    m_title = text;
    if (m_graph)
        m_graph->titleLabel()->setText(text);
}

void GraphProperty::initVars()
{
    m_graph = nullptr;
    m_isXAxisVisible = true;
    m_isYAxisVisible = true;
    m_isHTitleVisible = true;
    m_isVTitleVisible = true;
    m_isTitleVisible = true;
    m_isDataVisible = true;
    m_isLegendVisible = true;
    m_isLegendItemVisible = false;
    m_connectAxisBreak = false;
    m_connectMissingData = false;
    m_isAxisMajorMarkVisible = true;
    m_isAxisMinorMarkVisible = false;
    m_isBarLabelVisible = false;
    m_isGridVisible = false;
    m_isMajorGridVisible = true;
    m_isMinorGridVisible = false;
    m_isGridHorizontalVisible = false;
    m_isGridVerticalVisible = false;
    m_isHorizontalDroplineVisible = true;
    m_isVerticalDroplineVisible = true;
    m_useGroupLineColor = false;
    m_useGroupPatternColor = false;
    m_useGroupPenStyle = false;
    m_useGroupBubbleStyle = false;
    m_axisMajorLabelsVisible = true;
    m_axisMinorLabelsVisible = false;
    m_axisPlusSignVisible = false;
    m_axisMinusSignVisible = false;

    m_axisTickColor = Qt::black;
    m_axisLabelColor = Qt::black;
    m_axisDecimalWidth = 0;

    m_bgStartColor = Qt::white;
    m_bgStopColor = Qt::white;
    m_bgGradientFill = ComDataGui::Utils::None;

    m_canvasStartColor = Qt::gray;
    m_canvasGradientFill = ComDataGui::Utils::None;

    m_titleColor = Qt::black;

    m_titleOpacity = 1.0;
    m_graphWindowGradientFill = ComDataGui::Utils::None;

    m_axisTickWidth = 1.0;
    m_barChartLineWidth = 1;
    m_curveLineWidth = 1;
    m_symbolSize = 12;
    m_symbolWidth = 1;
    m_maxCurveDataCount = 2000;
    m_bgLightness = 255;
    m_bgGradientFillMode = 1;

    m_barChartFillOpacity = 1.0;
    m_curveLineOpacity = 1.0;
    m_barLabelOpacity = 1.0;
    m_symbolOpacity = 1.0;
    m_droplineOpacity = 1.0;

    m_legendColor = Qt::black;

    m_legendItemBgColor = Qt::white;
    m_legendItemFontColor = Qt::black;
    m_legendItemBorderColor = Qt::black;

    m_legendPosition = 0;
    m_legendItemColumns = 0;
    m_legendItemHAlign = 0;
    m_legendItemVAlign = 0;
    m_legendItemBgMode = 0;

    m_barChartFillPattern = Qt::SolidPattern;
    m_barChartFillStyle = QwtColumnSymbol::Raised;

    m_barLabelPosition = QwtPlot::OutsideEnd;
    m_barLabelColor = Qt::black;
    m_barLabelOffsetX = 0;
    m_barLabelOffsetY = 0;

    m_gridPen = QPen(Qt::black, 1, Qt::SolidLine);
    m_barChartBorderPen = QPen(Qt::black, 1, Qt::SolidLine);
    m_droplinePen = QPen(Qt::black, 1, Qt::SolidLine);

    m_symbolColor = Qt::yellow;
    m_symbolEdgeColor = Qt::yellow;
    m_symbolStyle = QwtSymbol::NoSymbol;

    m_bubbleSize = 12;
    m_bubbleEdgeWidth = 1;
    m_bubbleOpacity = 1.0;
    m_canvasBorderRadius = 7;

    m_pieStartAngle = 90;
    m_pieThickness = 15;
    m_isCounterClockwise = true;
    m_pieViewAngle = 60;

}

void GraphProperty::initFrom(const GraphProperty &other)
{
    m_graph = other.m_graph;
    m_isXAxisVisible = other.m_isXAxisVisible;
    m_isYAxisVisible = other.m_isYAxisVisible;
    m_isHTitleVisible = other.m_isHTitleVisible;
    m_isVTitleVisible = other.m_isVTitleVisible;
    m_isTitleVisible = other.m_isTitleVisible;
    m_isDataVisible = other.m_isDataVisible;
    m_isLegendVisible = other.m_isLegendVisible;
    m_isLegendItemVisible = other.m_isLegendItemVisible;
    m_connectAxisBreak = other.m_connectAxisBreak;
    m_connectMissingData = other.m_connectMissingData;
    m_isAxisMajorMarkVisible = other.m_isAxisMajorMarkVisible;
    m_isAxisMinorMarkVisible = other.m_isAxisMinorMarkVisible;
    m_isBarLabelVisible = other.m_isBarLabelVisible;
    m_isGridVisible = other.m_isGridVisible;
    m_isMajorGridVisible = other.m_isMajorGridVisible;
    m_isMinorGridVisible = other.m_isMinorGridVisible;
    m_isGridHorizontalVisible = other.m_isGridHorizontalVisible;
    m_isGridVerticalVisible = other.m_isGridVerticalVisible;
    m_isHorizontalDroplineVisible = other.m_isHorizontalDroplineVisible;
    m_isVerticalDroplineVisible = other.m_isVerticalDroplineVisible;
    m_useGroupLineColor = other.m_useGroupLineColor;
    m_useGroupPatternColor = other.m_useGroupPatternColor;
    m_useGroupPenStyle = other.m_useGroupPenStyle;
    m_useGroupBubbleStyle = other.m_useGroupBubbleStyle;
    m_axisMajorLabelsVisible = other.m_axisMajorLabelsVisible;
    m_axisMinorLabelsVisible = other.m_axisMinorLabelsVisible;
    m_axisPlusSignVisible = other.m_axisPlusSignVisible;
    m_axisMinusSignVisible = other.m_axisMinusSignVisible;
    m_isAntiAliasingCurve = other.m_isAntiAliasingCurve;

    m_axisLabelFont = other.m_axisLabelFont;
    m_axisTickColor = other.m_axisTickColor;
    m_axisLabelColor = other.m_axisLabelColor;
    m_axisLabelPrefix = other.m_axisLabelPrefix;
    m_axisLabelSuffix = other.m_axisLabelSuffix;
    m_axisDecimalWidth = other.m_axisDecimalWidth;
    m_axisNotation = other.m_axisNotation;

    m_title = other.m_title;
    m_verticalTitle = other.m_verticalTitle;
    m_horizontalTitle = other.m_horizontalTitle;
    m_bgStartColor = other.m_bgStartColor;
    m_bgStopColor = other.m_bgStopColor;
    m_bgGradientFill = other.m_bgGradientFill;

    m_canvasStartColor = other.m_canvasStartColor;
    m_canvasGradientFill = other.m_canvasGradientFill;

    m_titleFont = other.m_titleFont;
    m_titleColor = other.m_titleColor;

    m_titleOpacity = other.m_titleOpacity;
    m_graphWindowGradientFill = other.m_graphWindowGradientFill;
    m_axisTickWidth = other.m_axisTickWidth;
    m_barChartLineWidth = other.m_barChartLineWidth;
    m_curveLineWidth = other.m_curveLineWidth;
    m_symbolSize = other.m_symbolSize;
    m_symbolWidth = other.m_symbolWidth;
    m_maxCurveDataCount = other.m_maxCurveDataCount;
    m_bgLightness = other.m_bgLightness;
    m_bgGradientFillMode = other.m_bgGradientFillMode;

    m_barChartFillOpacity = other.m_barChartFillOpacity;
    m_curveLineOpacity = other.m_curveLineOpacity;
    m_barLabelOpacity = other.m_barLabelOpacity;
    m_symbolOpacity = other.m_symbolOpacity;
    m_droplineOpacity = other.m_droplineOpacity;

    m_legendFont = other.m_legendFont;
    m_legendColor = other.m_legendColor;

    m_legendItemFont = other.m_legendItemFont;
    m_legendItemBgColor = other.m_legendItemBgColor;
    m_legendItemFontColor = other.m_legendItemFontColor;
    m_legendItemBorderColor = other.m_legendItemBorderColor;

    m_legendPosition = other.m_legendPosition;
    m_legendItemColumns = other.m_legendItemColumns;
    m_legendItemHAlign = other.m_legendItemHAlign;
    m_legendItemVAlign = other.m_legendItemVAlign;
    m_legendItemBgMode = other.m_legendItemBgMode;

    m_barChartFillPattern = other.m_barChartFillPattern;
    m_barChartFillStyle = other.m_barChartFillStyle;

    m_barLabelFont = other.m_barLabelFont;
    m_barLabelPosition = other.m_barLabelPosition;
    m_barLabelColor = other.m_barLabelColor;
    m_barLabelOffsetX = other.m_barLabelOffsetX;
    m_barLabelOffsetY = other.m_barLabelOffsetY;

    m_gridPen = other.m_gridPen;
    m_barChartBorderPen = other.m_barChartBorderPen;
    m_droplinePen = other.m_droplinePen;

    m_symbolColor = other.m_symbolColor;
    m_symbolEdgeColor = other.m_symbolEdgeColor;
    m_symbolStyle = other.m_symbolStyle;

    m_groupLineColors = other.m_groupLineColors;
    m_groupPatternColors = other.m_groupPatternColors;
    m_groupPenStyles = other.m_groupPenStyles;
    m_groupBubbleStyles = other.m_groupBubbleStyles;

    m_curveLabelData = other.m_curveLabelData;

    m_bubbleSize = other.m_bubbleSize;
    m_bubbleEdgeWidth = other.m_bubbleEdgeWidth;
    m_bubbleOpacity = other.m_bubbleOpacity;
    m_canvasBorderRadius = other.m_canvasBorderRadius;

    m_pieStartAngle = other.m_pieStartAngle;
    m_pieThickness = other.m_pieThickness;
    m_isCounterClockwise = other.m_isCounterClockwise;
    m_pieViewAngle = other.m_pieViewAngle;
    m_canvasLightness = other.m_canvasLightness;
    m_canvasStopColor = other.m_canvasStopColor;
    m_canvasGradientFillMode = other.m_canvasGradientFillMode;

    m_axisTitleType = other.m_axisTitleType;

    m_isErrorBarPlusVisible = other.m_isErrorBarPlusVisible;
    m_isErrorBarMinusVisible = other.m_isErrorBarMinusVisible;
    m_errorBarLineColor = other.m_errorBarLineColor;
    m_errorBarLineWidth = other.m_errorBarLineWidth;
    m_errorBarHandleWidth = other.m_errorBarHandleWidth;
    m_errorBarLineOpacity = other.m_errorBarLineOpacity;
}

/*!
 * \brief 打印变量值.
 */
void GraphProperty::print() const
{
#undef DEBUG
#define DEBUG(VAR) qDebug() << #VAR << "=" << VAR;

    DEBUG(m_graph )
    DEBUG(m_isXAxisVisible )
    DEBUG(m_isYAxisVisible )
    DEBUG(m_isHTitleVisible )
    DEBUG(m_isVTitleVisible )
    DEBUG(m_isTitleVisible )
    DEBUG(m_isDataVisible )
    DEBUG(m_isLegendVisible )
    DEBUG(m_isLegendItemVisible )
    DEBUG(m_connectAxisBreak )
    DEBUG(m_connectMissingData )
    DEBUG(m_isAxisMajorMarkVisible )
    DEBUG(m_isAxisMinorMarkVisible )
    DEBUG(m_isBarLabelVisible )
    DEBUG(m_isGridVisible )
    DEBUG(m_isMajorGridVisible )
    DEBUG(m_isMinorGridVisible )
    DEBUG(m_isGridHorizontalVisible )
    DEBUG(m_isGridVerticalVisible )
    DEBUG(m_isHorizontalDroplineVisible )
    DEBUG(m_isVerticalDroplineVisible )
    DEBUG(m_useGroupLineColor )
    DEBUG(m_useGroupPatternColor )
    DEBUG(m_useGroupPenStyle )
    DEBUG(m_useGroupBubbleStyle )
    DEBUG(m_axisMajorLabelsVisible )
    DEBUG(m_axisMinorLabelsVisible )
    DEBUG(m_axisPlusSignVisible )
    DEBUG(m_axisMinusSignVisible )
    DEBUG(m_isAntiAliasingCurve)

    DEBUG(m_axisLabelFont )
    DEBUG(m_axisTickColor )
    DEBUG(m_axisLabelColor )
    DEBUG(m_axisLabelPrefix )
    DEBUG(m_axisLabelSuffix )
    DEBUG(m_axisDecimalWidth )

    DEBUG(m_title )
    DEBUG(m_verticalTitle )
    DEBUG(m_horizontalTitle )
    DEBUG(m_bgStartColor )
    DEBUG(m_bgStopColor )
    DEBUG(m_bgGradientFill )

    DEBUG(m_canvasStartColor )
    DEBUG(m_canvasGradientFill )

    DEBUG(m_titleFont )
    DEBUG(m_titleColor )

    DEBUG(m_titleOpacity )
    DEBUG(m_graphWindowGradientFill )
    DEBUG(m_axisTickWidth )
    DEBUG(m_barChartLineWidth )
    DEBUG(m_curveLineWidth )
    DEBUG(m_symbolSize )
    DEBUG(m_symbolWidth )
    DEBUG(m_maxCurveDataCount )
    DEBUG(m_bgLightness )
    DEBUG(m_bgGradientFillMode )

    DEBUG(m_barChartFillOpacity )
    DEBUG(m_curveLineOpacity )
    DEBUG(m_barLabelOpacity )
    DEBUG(m_symbolOpacity )
    DEBUG(m_droplineOpacity )

    DEBUG(m_legendFont )
    DEBUG(m_legendColor )

    DEBUG(m_legendItemFont )
    DEBUG(m_legendItemBgColor )
    DEBUG(m_legendItemFontColor )
    DEBUG(m_legendItemBorderColor )

    DEBUG(m_legendPosition )
    DEBUG(m_legendItemColumns )
    DEBUG(m_legendItemHAlign )
    DEBUG(m_legendItemVAlign )
    DEBUG(m_legendItemBgMode )

    DEBUG(m_barChartFillPattern )
    DEBUG(m_barChartFillStyle )

    DEBUG(m_barLabelFont )
    DEBUG(m_barLabelPosition )
    DEBUG(m_barLabelColor )
    DEBUG(m_barLabelOffsetX )
    DEBUG(m_barLabelOffsetY )

    DEBUG(m_gridPen )
    DEBUG(m_barChartBorderPen )
    DEBUG(m_droplinePen )

    DEBUG(m_symbolColor )
    DEBUG(m_symbolEdgeColor )
    DEBUG(m_symbolStyle )

    DEBUG(m_groupLineColors )
    DEBUG(m_groupPatternColors )
    DEBUG(m_groupPenStyles )
    DEBUG(m_groupBubbleStyles )

//    DEBUG(m_curveLabelData )

    DEBUG(m_bubbleSize )
    DEBUG(m_bubbleEdgeWidth )
    DEBUG(m_bubbleOpacity )
    DEBUG(m_canvasBorderRadius )

#undef DEBUG

}

}
