include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$SC_PLUGIN_NAME

include(../simcube.pri)

TRANSLATIONS += $${SIMCUBE_SOURCE_TREE}/translations/$${SC_PLUGIN_NAME}_zh_CN.ts \
#                $${SIMCUBE_SOURCE_TREE}/translations/$${SC_PLUGIN_NAME}_zh_TW.ts

# 预编译头文件
#isEmpty(PRECOMPILED_HEADER):PRECOMPILED_HEADER = $$PWD/shared/simcube_gui_pch.h

isEmpty(PROVIDER) {
    PROVIDER = simcube
} else {
    LIBS += -L$$SIMCUBE_PLUGIN_PATH/simcube
}

DESTDIR = $$SIMCUBE_PLUGIN_PATH/$$PROVIDER

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryName($$TARGET)

QMAKE_TARGET_DESCRIPTION = "SimCube Plugin"
