#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

namespace ComDataGui
{
namespace Constants
{

const char SETTINGS_ID_VIZGRAPH[] = "Z.VizGraph";
const char SETTINGS_ID_BOOKS[] = "Z.Books";
const char SETTINGS_ID_AXIS[] = "C.Axis";
const char SETTINGS_ID_GRAPHWINDOW[] = "A.GraphWindow";
const char SETTINGS_ID_MULTIITEM[] = "E.MultiItem";
const char SETTINGS_ID_GRAPH[] = "B.Graph";
const char SETTINGS_ID_CURVES[] = "F.Curves";
const char SETTINGS_ID_LEGEND[] = "D.Legend";
const char SETTINGS_ID_ERRORBAR[] = "Z.ErrorBar";
const char SETTINGS_ID_BARCHART[] = "H.BarChart";
const char SETTINGS_ID_PIES[] = "Z.Pies";
const char SETTINGS_ID_BUBBLES[] = "G.Bubbles";
const char SETTTINS_ID_MULTIITEM[] = "E.MultiItem";

const char SETTINGS_CATEGORY_VIZGRAPH[] = "D.VizGraph";                                                
const char SETTINGS_CATEGORY_VIZGRAPH_ICON[] = "://images/vizgraphsetting.png";

const char SETTINGS_CATEGORY_BOOKS[] = "C.Books";
const char SETTINGS_CATEGORY_BOOKS_ICON[] = "://images/bookssetting.png";


const char C_VIZ_WORKSHEET[]    = "Table.Context.Worksheet";
const char C_VIZ_GRAPH[]        = "Table.Context.Graph";

// menus

const char M_COLUMN_FILLNUMWITH[]  = "SimViz.Menu.Column.FillNumWith";
const char M_ADDCOLUMN[]           = "SimViz.Menu.AddColumn";
const char M_INSERTCOLUMN[]        = "SimViz.Menu.InsertColumn";
const char M_SHOWCOMPLEXROW[]      = "SimViz.Menu.ShowComplexrow";
const char M_COLUMNHEADERSETTING[] = "SimViz.Menu.ColumnHeaderSetting";
const char M_WORKSHEETSHOW[]       = "SimViz.Menu.TableShow";

const char M_PLOT[]                = "Table.Menu.Plot";
const char M_PLOT_LINE[]           = "Table.Menu.Plot.Line";
const char M_PLOT_AREA[]           = "Table.Menu.Plot.Area";
const char M_PLOT_CBP[]            = "Table.Menu.Plot.CBP";

// Plot Groups
const char G_PLOT_LINE[]           = "SimViz.Group.Plot.Line";
const char G_PLOT_AREA[]           = "SimViz.Group.Plot.Area";
const char G_PLOT_BUBBLE[]           = "SimViz.Group.Plot.Bubble";
const char G_PLOT_COLUMN[]         = "SimViz.Group.Plot.Column";
const char G_PLOT_BAR[]            = "SimViz.Group.Plot.Bar";
const char G_PLOT_PIE[]            = "SimViz.Group.Plot.Pie";
const char G_PLOT_SURFACE[]        = "SimViz.Group.Plot.Surface";

// Table groups
const char G_TABLE_MOVE[]           = "SimViz.Group.Column.Move";
const char G_TABLE_COLUMNSETAS[]    = "SimViz.Group.Column.ColumnSetAs";
const char G_TABLE_TOGGLE[]         = "SimViz.Group.Column.Toggle";
const char G_TABLE_APPENDINSERT[]   = "SimViz.Group.Column.AppendInsert";
const char G_TABLE_DATAMARK[]       = "SimViz.Group.Column.DataMark";
const char G_TABLE_SORT[]           = "SimViz.Group.Column.Sort";
const char G_TABLE_FILL[]           = "SimViz.Group.Column.Fill";
//const char G_WORKSHEET_SHOW[]      = "SimViz.Group.Worksheet.Show";

const char G_COLUMN_ADDCOLUMN[]      = "SimViz.Group.Column.AddColumn";
const char G_COLUMN_INSERTCOLUMN[]   = "SimViz.Group.Column.InsertColumn";
const char G_COLUMN_COLUMN_SORT[]    = "SimViz.Group.Column.Sort";
const char G_COLUMN_BOOKTABLESORT[]  = "SimViz.Group.Column.BookTableSort";
const char G_COLUMN_MARKDATA[]       = "SimViz.Group.Column.MarkData";

// show comlex row groups
//const char G_SHOWCOMPLEXROW_SHOW[]   = "SimViz.Group.ShowComplex.Show";
//const char G_WORKSHEET_SHOW_CONTENT[]   = "SimViz.Group.Table.Show";

// column header setting groups
const char G_COLUMNHEADERSETTING_CONTENT[] = "SimViz.Group.ColumnHeaderSetting.Content";

const char G_EDIT_UNDOREDO[]          = "SimViz.Group.Edit.UndoRedo";

// Graph groups
const char G_GRAPH_LAYER[]           = "SimViz.Group.Graph.Layer";

// Data Groups
const char G_DATA_MARK[]             = "SimViz.Group.Data.Mark";
const char G_DATA_MASK[]             = "SimViz.Group.Data.Mask";
const char G_DATA_POINT[]            = "SimViz.Group.Data.Point";

// actions
const char MARK[]                  = "SimViz.Mark";
const char UNMARK[]                = "SimViz.UnMark";

//about table
const char WORKSHEETSHOWHEADERLABEL[]        = "SimViz.WorkSheetShowHeaderLabel";
const char WORKSHEETSHOWCOLUMNHEADER[]       = "SimViz.WorkSheetShowColumnHeader";
const char WORKSHEETSHOWROWHEADER[]          = "SimViz.WorkSheetShowRowHeader";
const char WORKSHEETSHOWGRID[]         = "SimViz.WorkSheetShowColumnLine";

const char ROWNUMBER[]             = "SimViz.RowNumber";
const char UNIFORMRANDOMNUMBERS[]  = "SimViz.UniformRandomNumbers";
const char NORMALRANDOMNUMBERS[]   = "SimViz.NormalRandomNumbers";

const char ADDSTRINGCOLUMN[]       = "SimViz.AddStringColumn";
const char ADDINTEGERCOLUMN[]      = "SimViz.AddIntegerColumn";
const char ADDFLOATCOLUMN[]        = "SimViz.AddFloatColumn";
const char ADDDOUBLECOLUMN[]       = "SimViz.AddDoubleColumn";
const char ADDFLOATCOMPLEXCOLUMN[] = "SimViz.AddFloatComplexColumn";
const char ADDDOUBLECOMPLEXCOLUMN[] = "SimViz.AddDoubleComplexColumn";
const char ADDDATETIMECOLUMN[]     = "SimViz.AddDateTimeColumn";

const char INSERTSTRINGCOLUMN[]        = "SimViz.InsertStringColumn";
const char INSERTINTEGERCOLUMN[]       = "SimViz.InsertIntegerColumn";
const char INSERTFLOATCOLUMN[]         = "SimViz.InsertFloatColumn";
const char INSERTDOUBLECOLUMN[]        = "SimViz.InsertDoubleColumn";
const char INSERTFLOATCOMPLEXCOLUMN[]  = "SimViz.InsertFloatComplexColumn";
const char INSERTDOUBLECOMPLEXCOLUMN[] = "SimViz.InsertDoubleComplexColumn";
const char INSERTDATETIMECOLUMN[]      = "SimViz.InsertDateTimeColumn";
const char DELETECOLUMN[]              = "SimViz.DeleteColumn";
const char CLEARTABLE[]                = "SimViz.ClearTable";
const char APPENDROW[]                 = "SimViz.AppendRow";
const char INSERTROW[]                 = "SimViz.InsertRow";

const char MOVEFIRST[]             = "SimViz.MoveFirst";
const char MOVEEND[]               = "SimViz.MoveEnd";
const char MOVELEFT[]              = "SimViz.MoveLeft";
const char MOVERIGHT[]             = "SimViz.MoveRight";
const char SETHEADER[]             = "SimViz.SetHeader";
const char SHOWCOLUMN[]            = "SimViz.ShowColumn";
const char HIDECOLUMN[]            = "SimViz.HidColumn";
const char COLUMNSORTASC[]         = "SimViz.ColumnSortAsc";
const char COLUMNSORTDESC[]        = "SimViz.ColumnSortDesc";
const char BOOKTABLESORTASC[]      = "SimViz.BookTableSortAsc";
const char BOOKTABLESORTDESC[]     = "SimViz.BookTableSortDesc";

// about graph
const char GRAPHNEWLEGEND[]        = "SimViz.GraphNewLegend";
const char GRAPHUPDATELEGEND[]     = "SimViz.GraphUpdateLegend";
const char GRAPFITLAYERTOGRAPH[]   = "SimViz.GraphFitLayerToGraph";
const char GRAPHEXCHANGEXYAXIS[]   = "SimViz.GraphExchangeXYAxis";
const char GRAPHEXPORTIMAGE[]   = "SimViz.GraphExportImage";
const char GRAPHTILEGRAPHS[]   = "SimViz.GraphTileGraphs";
const char GRAPHZOOMIN[]   = "SimViz.GraphZoomIn";
const char GRAPHZOOMOUT[]   = "SimViz.GraphZoomOut";
const char GRAPHZOOMINHOR[]   = "SimViz.GraphZoomInHorizontally";
const char GRAPHZOOMOUTHOR[]   = "SimViz.GraphZoomOutHorizontally";
const char GRAPHZOOMINVER[]   = "SimViz.GraphZoomInVertically";
const char GRAPHZOOMOUTVER[]   = "SimViz.GraphZoomOutVertically";

// about data
const char MARKDATARANGE[]         = "SimViz.MarkDataRange";
const char CLEARDATAMARK[]         = "SimViz.ClearDataMark";
const char MASKDATARANGE[]         = "SimViz.MaskDataRange";
const char UNMASKDATARANGE[]       = "SimViz.UnmaskDataColor";
const char CHANGEMASKCOLOR[]       = "SimViz.ChangeMaskColor";
const char SHOWORHIDMASKPOINT[]    = "SimViz.ShowOrHidMaskPoint";
const char REMOVEBADPOINT[]        = "SimViz.RemoveBadPoint";
const char SHOWPOINTVALUE[]        = "SimViz.ShowPointValue";

// about show complex Row
const char SHOWLONGNAME[]          = "SimViz.ShowLongName";
const char SHOWUNIT[]              = "SimViz.ShowUnit";
const char SHOWCOMMENT[]           = "SimViz.ShowComment";
const char SHOWFILTER[]            = "SimViz.ShowFilter";
const char SHOWUSER[]              = "SimViz.ShowUser";
const char SHOWTHUMBNAILS[]        = "SimViz.ShowShowThumbnails";

// column header setting
const char X[]                     = "SimViz.X";
const char Y[]                     = "SimViz.Y";
const char Z[]                     = "SimViz.Z";
const char V[]                     = "SimViz.V";
const char VX[]                    = "SimViz.VX";
const char VY[]                    = "SimViz.VY";
const char VZ[]                    = "SimViz.VZ";
const char LABEL[]                 = "SimViz.Label";
const char DISREGARD[]             = "SimViz.Disregard";
const char XERROR[]                = "SimViz.XError";
const char YERROR[]                = "SimViz.YError";

//const char SETCOLUMNASPROPERTY[]   = "SimViz.SetColumnProperty";

//about plot
// plot line
const char PLOTLINE[]              = "Table.PlotLine";
const char PLOTHORIZONTALSTEP[]    = "Table.PlotHorizontalStep";
const char PLOTVERTICALSTEP[]      = "Table.PlotVerticalStep";
const char PLOTSPLINECONNECTED[]   = "Table.PlotSplineConnected";
// plot menu
const char PLOTAREA[]              = "Table.PlotArea";
const char PLOTBUBBLE[]              = "Table.PlotBubble";
const char PLOTSTACKAREA[]         = "Table.PlotStackArea";
const char PLOTFILLAREA[]          = "Table.PlotFillRrea";
// plot column/bar/pie
const char PLOTCOLUMN[]            = "Table.PlotColumn";
const char PLOTCOLUMNPLUSLABEL[]   = "Table.PlotColumnPlusLabel";
const char PLOTBAR[]               = "Table.PlotBar";
const char PLOTBARPLUSLABEL[]      = "Table.PlotBarPlusLabel";
const char PLOTSTACKCOLUMN[]       = "Table.PlotStackColumn";
const char PLOTSTACKBAR[]          = "Table.PlotStackBar";
const char PLOTFLOATINGCOLUMN[]    = "Table.PlotFloatColumn";
const char PLOTFLOATINGBAR[]       = "Table.PlotFloatBar";
const char PLOT3DCOLORPIECHART[]   = "Table.Plot3DColorPieChart";
const char PLOT2DBWPIECHART[]      = "Table.Plot2DBWPieChart";
const char PLOT3DSURFACE[]         = "Table.Plot3DSurface";



// table column
const char TABLEROWLONGNAME[]             = QT_TRANSLATE_NOOP("SimViz", "LongName");
const char TABLEROWUNIT[]                 = QT_TRANSLATE_NOOP("SimViz", "Unit");
const char TABLEROWCOMMENT[]              = QT_TRANSLATE_NOOP("SimViz", "Comment");
const char TABLEROWUSERFUNCTION[]         = QT_TRANSLATE_NOOP("SimViz", "User Function");
const char TABLEROWFILTER[]               = QT_TRANSLATE_NOOP("SimViz", "Filter");
const char TABLEROWTHUNMBNAIL[]           = QT_TRANSLATE_NOOP("SimViz", "Thumbnails");

// Graph
const char ICON_NEWLEGEND[]          = ":/images/newlegend.png";
const char ICON_UPDATELEGEND[]       = ":/images/updateLegend.png";
const char ICON_FITLAYERTOLEGEND[]   = ":/images/fitlaytoleg.png";
const char ICON_EXCHANGEXYAXISES[]   = ":/images/exchangeXYaxises.png";
const char ICON_EXPORTIMAGE[]        = ":/images/exportimage.png";
const char ICON_TILEGRAPHS[]         = "://images/sixup.png";
const char ICON_ZOOMIN[]             = "://images/zoomin.png";
const char ICON_ZOOMOUT[]            = "://images/zoomout.png";
const char ICON_ZOOMINHOR[]          = "://images/zoominhorizontal.png";
const char ICON_ZOOMOUTHOR[]         = "://images/zoomouthorizontal.png";
const char ICON_ZOOMINVER[]          = "://images/zoominvertical.png";
const char ICON_ZOOMOUTVER[]          = "://images/zoomoutvertical.png";

const char ICON_UNDO[]               = ":/images/undo.png";
const char ICON_REDO[]               = ":/images/redo.png";

const char ICON_COLUMNX[]            = ":/images/x_col.png";
const char ICON_COLUMNY[]            = ":/images/y_col.png";
const char ICON_COLUMNZ[]            = ":/images/z_col.png";
const char ICON_COLUMNLABEL[]        = ":/images/set_label_col.png";
const char ICON_COLUMNDISREGARD[]    = ":/images/disregard_col.png";
const char ICON_COLUMNXERROR[]       = ":/images/x_error.png";
const char ICON_COLUMNYERROR[]       = ":/images/y_error.png";
const char ICON_COLUMNV[]            = "://images/v_col.png";
const char ICON_COLUMNVX[]           = "://images/vx_col.png";
const char ICON_COLUMNVY[]           = "://images/vy_col.png";
const char ICON_COLUMNVZ[]           = "://images/vz_col.png";

const char ICON_COLUMNMOVELEFT[]       = ":/images/move_col_left.png";
const char ICON_COLUMNMOVRIGHT[]       = ":/images/move_col_right.png";
const char ICON_COLUMNMOVFIRST[]       = ":/images/move_col_first.png";
const char ICON_COLUMNMOVELAST[]       = ":/images/move_col_last.png";

const char ICON_LINE[]               = ":/images/line.png";
const char ICON_HORIZONTALSTEP[]     = ":/images/horizontalstep.png";
const char ICON_VERTICALSTEP[]       = ":/images/verticalstep.png";
const char ICON_STICK[]              = ":/images/stick.png";
const char ICON_SPLINECONNECTED[]    = ":/images/splineconnected.png";

const char ICON_AREA[]               = ":/images/area.png";
const char ICON_BUBBLE[]             = ":/images/bubble.png";
const char ICON_STACKAREA[]          = ":/images/stackarea.png";
const char ICON_FILLAREA[]           = ":/images/fillarea.png";

const char ICON_BAR[]                = ":/images/bar.png";
const char ICON_BARPLUSLABEL[]       = ":/images/barpluslabel.png";
const char ICON_COLUMN[]             = ":/images/column.png";
const char ICON_COLUMNPLUSLABEL[]    = ":/images/columnpluslabel.png";

const char ICON_STACKCOLUMN[]        = ":/images/stackcolumn.png";
const char ICON_STACKBAR[]           = ":/images/stackbar.png";
const char ICON_FLOATINGCOLUMN[]     = ":/images/floatingcolumn.png";
const char ICON_FLOATINGBAR[]        = ":/images/floatingbar.png";

const char ICON_PIE3D[]              = ":/images/pie3d.png";
const char ICON_PIE2D[]              = ":/images/pie2d.png";
const char ICON_CLEAR[]              = ":/images/clear.png";
const char ICON_DELETECOL[]          = "://images/deletecol.png";
const char ICON_APPEND_ROW[]         = ":/images/appendrow.png";
const char ICON_INSERT_ROW[]         = ":/images/insertrow.png";
// row Number
const char ICON_ROWNUMBER[]             = ":/images/rownumber.png";
const char ICON_UNIFORMRANDOMNUMBERS[]  = ":/images/uniformrandomnumbers.png";
const char ICON_NORMALRANDOMNUMBERS[]   = ":/images/normalrandomnumbers.png";

// Mask
const char ICON_MARK[]                  = ":/images/mark.png";
const char ICON_UNMARK[]                = ":/images/unmark.png";

// Sort
const char ICON_ASC_COLUMN[]            = ":/images/ascbycol.png";
const char ICON_DESC_COLUMN[]           = ":/images/descbycol.png";
const char ICON_ASC_TABLE[]             = ":/images/ascbytable.png";
const char ICON_DESC_TABLE[]            = ":/images/descbytable.png";

// window Icon
const char ICON_WORKSHEETWIN[]          = "://images/worksheetwin.png";
const char ICON_GRAPHWIN[]              = "://images/graphwin.png";
const char ICON_DELETETABLE[]           = "://images/deletetable.png";
const char ICON_DELETEWINDOW[]          = "://images/deletewindow.png";
}
}
#endif // CONSTANTS_H
