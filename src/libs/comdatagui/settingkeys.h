#ifndef SETTINGKEYS_H
#define SETTINGKEYS_H

#include <QString>

namespace ComDataGui
{
    /// Groups
    const QString Group_VizGraph = QStringLiteral("VizGraph");
    const QString Group_Axis = QStringLiteral("Axis");
    const QString Group_GraphWindow = QStringLiteral("GraphWindow");
    const QString Group_Graph = QStringLiteral("Graph");
    const QString Group_Curves = QStringLiteral("Curves");
    const QString Group_Legend = QStringLiteral("Legend");
    const QString Group_ErrorBar = QStringLiteral("ErrorBar");
    const QString Group_BarChart = QStringLiteral("BarChart");
    const QString Group_Pies = QStringLiteral("Pies");
    const QString Group_Bubbles = QStringLiteral("Bubbles");
    const QString Group_MultiItem = QStringLiteral("MultiItem");

    /// Keys
    // Axis
    const QString Axis_TickColor = QStringLiteral("AxisTickColor");
    const QString Axis_LabelColor = QStringLiteral("AxisLabelColor");
    const QString Axis_TickWidth = QStringLiteral("AxisTickWidth");
    const QString Axis_LabelFont = QStringLiteral("AxisLabelFont");
    const QString Axis_TitleType = QStringLiteral("AxisTitleType");
    const QString Axis_ShowGrid = QStringLiteral("AxisShowGrid");
    const QString Axis_ShowHorizontalGrid = QStringLiteral("AxisShowHorizontalGrid");
    const QString Axis_ShowVerticalGrid = QStringLiteral("AxisShowVerticalGrid");
    const QString Axis_ShowMajorGrid = QStringLiteral("AxisShowMajorGrid");
    const QString Axis_ShowMinorGrid = QStringLiteral("AxisShowMinorGrid");
    const QString Axis_GridColor = QStringLiteral("AxisGridColor");
    const QString Axis_GridWidth = QStringLiteral("AxisGridWidth");
    const QString Axis_GridStyle = QStringLiteral("AxisGridStyle");
    const QString Axis_ShowMajorLabels = QStringLiteral("AxisShowMajorLabels");
    const QString Axis_ShowMinorLabels = QStringLiteral("AxisShowMinorLabels");
    const QString Axis_DecimalWidth = QStringLiteral("AxisDecimalWidth");
    const QString Axis_Notation = QStringLiteral("AxisNotation");
    const QString Axis_Prefix = QStringLiteral("AxisPrefix");
    const QString Axis_Suffix = QStringLiteral("AxisSuffix");
    const QString Axis_ShowPlusSign = QStringLiteral("AxisShowPlusSign");
    const QString Axis_ShowMinusSign = QStringLiteral("AxisShowMinusSign");

    // Bubble
    const QString Bubble_Size = QStringLiteral("BubbleSize");
    const QString Bubble_EdgeWidth = QStringLiteral("BubbleEdgeWidth");
    const QString Bubble_Opacity = QStringLiteral("BubbleOpacity");

    // Pie
    const QString Pie_ViewAngle = QStringLiteral("PieViewAngle");
    const QString Pie_StartAngle = QStringLiteral("PieStartAngle");
    const QString Pie_Thickness = QStringLiteral("PieThickness");
    const QString Pie_CounterClockwise = QStringLiteral("PieCounterClockwise");
    const QString Pie_RadiusRatio = QStringLiteral("PieRadiusRatio");
    const QString Pie_DisplacementRatio = QStringLiteral("PieDisplacementRatio");

    // Legend
    const QString Legend_ShowLegend = QStringLiteral("LegendShowLegend");
    const QString Legend_Font = QStringLiteral("LegendFont");
    const QString Legend_Color = QStringLiteral("LegendColor");
    const QString Legend_Position = QStringLiteral("LegendPosition");
    const QString Legend_ShowItem = QStringLiteral("LegendShowItem");
    const QString Legend_ItemFont = QStringLiteral("LegendItemFont");
    const QString Legend_ItemBgColor = QStringLiteral("LegendItemBgColor");
    const QString Legend_ItemColor = QStringLiteral("LegendItemColor");
    const QString Legend_BorderColor = QStringLiteral("LegendBorderColor");
    const QString Legend_Columns = QStringLiteral("LegendColumns");
    const QString Legend_HorizontalAlignment = QStringLiteral("LegendHorizontalAlignment");
    const QString Legend_VerticalAlignment = QStringLiteral("LegendVerticalAlignment");
    const QString Legend_ItemMode = QStringLiteral("LegendItemMode");

    // ErrorBar
    const QString ErrorBar_PlusVisible = QStringLiteral("ErrorBarPlusVisible");
    const QString ErrorBar_MinusVisible = QStringLiteral("ErrorBarMinusVisible");
    const QString ErrorBar_Color = QStringLiteral("ErrorBarColor");
    const QString ErrorBar_LineWidth = QStringLiteral("ErrorBarLineWidth");
    const QString ErrorBar_HandleWidth = QStringLiteral("ErrorBarHandleWidth");
    const QString ErrorBar_LineOpacity = QStringLiteral("ErrorBarLineOpacity");

    // BarChart
    const QString BarChart_FillPattern = QStringLiteral("BarChartFillPattern");
    const QString BarChart_LineWidth = QStringLiteral("BarChartLineWidth");
    const QString BarChart_FillOpacity = QStringLiteral("BarChartFillOpacity");
    const QString BarChart_FillStyle = QStringLiteral("BarChartFillStyle");
    const QString BarChart_LabelVisible = QStringLiteral("BarChartLabelVisible");
    const QString BarChart_LabelFont = QStringLiteral("BarChartLabelFont");
    const QString BarChart_LabelOpacity = QStringLiteral("BarChartLabelOpacity");
    const QString BarChart_LabelColor = QStringLiteral("BarChartLabelColor");
    const QString BarChart_LabelOffsetX = QStringLiteral("BarChartOffsetX");
    const QString BarChart_LabelOffsetY = QStringLiteral("BarChartOffsetY");
    const QString BarChart_LabelPosition = QStringLiteral("BarChartLabelPosition");
    const QString BarChart_BorderPen = QStringLiteral("BarChartBorderPen");

    // Curve
    const QString Curve_LineWidth = QStringLiteral("CurveLineWidth");
    const QString Curve_LineOpacity = QStringLiteral("CurveLineOpacity");
    const QString Curve_AntiAliasing = QStringLiteral("CurveAntiAliasing");
    const QString Curve_Symbol = QStringLiteral("CurveSymbol");
    const QString Curve_SymbolSize = QStringLiteral("CurveSize");
    const QString Curve_SymbolEdgeWidth = QStringLiteral("CurveEdgeWidth");
    const QString Curve_SymbolEdgeColor = QStringLiteral("CurveEdgeColor");
    const QString Curve_SymbolColor = QStringLiteral("CurveSymbolColor");
    const QString Curve_SymbolOpacity = QStringLiteral("CurveOpacity");
    const QString Curve_DroplineHorizontalVisible = QStringLiteral("CurveDroplineHorizontalVisible");
    const QString Curve_DroplineVerticalVisible = QStringLiteral("CurveDroplineVerticalVisible");
    const QString Curve_DroplineOpacity = QStringLiteral("CurveDroplineOpacity");
    const QString Curve_DroplinePen = QStringLiteral("CurveDroplinePen");
    const QString Curve_LabelVisible = QStringLiteral("CurveLabelVisible");
    const QString Curve_LabelFont = QStringLiteral("CurveLabelFont");
    const QString Curve_LabelColor = QStringLiteral("CurveLabelColor");
    const QString Curve_LabelPosition = QStringLiteral("CurveLabelPosition");
    const QString Curve_LabelOffsetX = QStringLiteral("CurveLabelOffsetX");
    const QString Curve_LabelOffsetY = QStringLiteral("CurveLabelOffsetY");
    const QString Curve_LabelOpacity = QStringLiteral("CurveLabelOpacity");

    // GraphWindow
    const QString GraphWindow_BackgroundStartColor = QStringLiteral("GraphWindowBackgroundColorFirst");
    const QString GraphWindow_BackgroundStopColor = QStringLiteral("GraphWindowBackgroundColorSecond");
    const QString GraphWindow_BackgroundLightness = QStringLiteral("GraphWindowBackgroundLightness");
    const QString GraphWindow_GradientFill = QStringLiteral("GraphWindowGradientFill");
    const QString GraphWindow_BackgroundGraidentFill = QStringLiteral("GraphWindowCanvasGradientFill");
    const QString GraphWindow_CanvasGradientFill = QStringLiteral("GraphWindowCanvasGradientFill");
    const QString GraphWindow_ConnectAxisBreak = QStringLiteral("GraphWindowConnectAxisBreak");
    const QString GraphWindow_ConnectMissingData = QStringLiteral("GraphWindowConnectMissingData");
    const QString GraphWindow_GradientFillMode = QStringLiteral("GraphWindowGradientFillMode");

    // Graph
    const QString Graph_CanvasStartColor = QStringLiteral("GraphCanvasStartColor");
    const QString Graph_CanvasStopColor = QStringLiteral("GraphCanvasStopColor");
    const QString Graph_CanvasLightness = QStringLiteral("GraphCanvasLightness");
    const QString Graph_CanvasGradientFillMode = QStringLiteral("GraphCanvasGradientFillMode");
    const QString Graph_XAxisVisible = QStringLiteral("GraphXAxisVisible");
    const QString Graph_YAxisVisible = QStringLiteral("GraphYAxisVisible");
    const QString Graph_XLabelVisible = QStringLiteral("GraphXLabelVisible");
    const QString Graph_YLabelVisible = QStringLiteral("GraphYLabelVisible");
    const QString Graph_DataVisible = QStringLiteral("GraphDataVisible");
    const QString Graph_TitleVisible = QStringLiteral("GraphTitleVisible");
    const QString Graph_TitleFont = QStringLiteral("GraphTitleFont");
    const QString Graph_TitleText = QStringLiteral("GraphTitleText");
    const QString Graph_TitleColor = QStringLiteral("GraphTitleColor");
    const QString Graph_TitleOpacity = QStringLiteral("GraphTitleOpacity");
    const QString Graph_MaxPointsPerCurve = QStringLiteral("GraphMaxPointsPerCurve");
    const QString Graph_CanvasBorderRadius = QStringLiteral("GraphCanvasBorderRadius");

    // MultiItem
    const QString MultiItem_UseGroupLineColors = QStringLiteral("MultiItemUseLineColors");
    const QString MultiItem_UseGroupPatternColors = QStringLiteral("MultiItemUsePatternColors");
    const QString MultiItem_UseGroupPenStyles = QStringLiteral("MultiItemUsePenStyles");
    const QString MultiItem_UseGroupBubbleStyles = QStringLiteral("MultiItemUseBubbleStyles");

    const QString MultiItem_GroupLineColors = QStringLiteral("MultiItemGroupLineColors");
    const QString MultiItem_GroupPatternColors = QStringLiteral("MultiItemGroupPatternColors");
    const QString MultiItem_GroupPenStyles = QStringLiteral("MultiItemGroupPenStyles");
    const QString MultiItem_GroupBubbleStyles = QStringLiteral("MultiItemGroupBubbleStyles");
}
#endif // SETTINGKEYS_H
