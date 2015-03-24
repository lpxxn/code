include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$SC_LIB_NAME

include(../simcube.pri)

TRANSLATIONS += $${SIMCUBE_SOURCE_TREE}/translations/$${SC_LIB_NAME}_zh_CN.ts \
#                $${SIMCUBE_SOURCE_TREE}/translations/$${SC_LIB_NAME}_zh_TW.ts

# 预编译头文件
#isEmpty(PRECOMPILED_HEADER):PRECOMPILED_HEADER = $$PWD/shared/simcube_pch.h

win32 {
    DLLDESTDIR = $$SIMCUBE_APP_PATH
}

DESTDIR = $$SIMCUBE_LIBRARY_PATH

include(rpath.pri)

TARGET = $$qtLibraryName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

QMAKE_TARGET_DESCRIPTION = "SimCube Library"
