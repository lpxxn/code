include(../../simcubelibrary.pri)
include(../../../uselib/sc_qwt.pri)
include(../../../uselib/sc_QXlsx.pri)

DEFINES += COMDATAGUI_LIBRARY
QT += script

HEADERS += \
            comdatagui_global.h \
            areagraph.h \
            barchartgraph.h \
            curvegraph.h \
            graph.h \
            multibarchartgraph.h \
            histogramgraph.h \
            booktable.h \
            pie2dgraph.h \
            pie3dgraph.h \
            graphproperty.h \
            detail/settingsbase.h \
            vizgraphbasesettings.h \
            layersettings.h \
            graphwindowsettings.h \
            axissettings.h \
            legendsettings.h \
            errorbarsettings.h \
            stacklinesettings.h \
            columnsettings.h \
            piesettings.h \
            bubblesettings.h \
            waterfallsettings.h \
            constants.h \
            booksettings.h \
            curvesettings.h \
            settingkeys.h \
            utils.h \
            multiitemsettings.h \
            bubblegraph.h \
            detail/psv_define.h \
            detail/psv_public.h \
            detail/psv_item.h \
            detail/psv_pie3item.h \
            detail/psv_sectoritem.h \
            detail/complextableview.h \
            detail/datatableview.h \
            detail/datamodel.h \
            detail/complexmodel.h \
            detail/tablemodelbase.h \
            detail/tableviewbase.h \
            symbolcombobox.h \
    comdataactionmanager.h \
    detail/bookundocommand.h \
    detail/delta.h \
    detail/deltahandler.h \
    detail/deltamaster.h \
    detail/comparator.h \
    detail/thumbnailwidget.h \
    detail/exporthandler.h \
    detail/filterdialog.h \
    detail/advancedfunctiondialog.h \
    detail/booktableundomanager.h \
    columnpropertydata.h

SOURCES += \
           areagraph.cpp \
           barchartgraph.cpp \
           curvegraph.cpp \
           graph.cpp \
           multibarchartgraph.cpp \
           histogramgraph.cpp \
           booktable.cpp \
            pie2dgraph.cpp \
            pie3dgraph.cpp \
            graphproperty.cpp \
            detail/settingsbase.cpp \
            vizgraphbasesettings.cpp \
            layersettings.cpp \
            graphwindowsettings.cpp \
            axissettings.cpp \
            legendsettings.cpp \
            errorbarsettings.cpp \
            stacklinesettings.cpp \
            columnsettings.cpp \
            piesettings.cpp \
            bubblesettings.cpp \
            waterfallsettings.cpp \
            booksettings.cpp \
            curvesettings.cpp \
            utils.cpp \
            multiitemsettings.cpp \
            bubblegraph.cpp \
            detail/psv_define.cpp \
            detail/psv_public.cpp \
            detail/psv_item.cpp \
            detail/psv_pie3item.cpp \
            detail/psv_sectoritem.cpp \
           detail/complextableview.cpp \
           detail/datatableview.cpp \
           detail/datamodel.cpp \
           detail/complexmodel.cpp \
            detail/tablemodelbase.cpp \
            detail/tableviewbase.cpp \
            symbolcombobox.cpp \
    comdataactionmanager.cpp \
    detail/bookundocommand.cpp \
    detail/delta.cpp \
    detail/deltahandler.cpp \
    detail/deltamaster.cpp \
    detail/thumbnailwidget.cpp \
    detail/exporthandler.cpp \
    detail/filterdialog.cpp \
    detail/advancedfunctiondialog.cpp \
    detail/booktableundomanager.cpp \
    columnpropertydata.cpp

FORMS += \
        detail/graphwindowsettings.ui \
        detail/layersettings.ui \
        detail/axissettings.ui \
        detail/legendsettings.ui \
        detail/curvesettings.ui \
        detail/errorbarsettings.ui \
        detail/bubblesettings.ui \
        detail/piesettings.ui \
        detail/columnsettings.ui \
        detail/stacklinesettings.ui \
        detail/waterfallsettings.ui \
        detail/booksettings.ui \
    detail/generalsettings.ui \
    detail/filterdialog.ui \
    detail/advancedfunctiondialog.ui

RESOURCES += \
    res.qrc
