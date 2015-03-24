include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$SC_APP_NAME

include(../simcube.pri)

TRANSLATIONS += $${SIMCUBE_SOURCE_TREE}/translations/$${SC_APP_NAME}_zh_CN.ts \
#                $${SIMCUBE_SOURCE_TREE}/translations/$${SC_APP_NAME}_zh_TW.ts

include(rpath.pri)
# 预编译头文件
#isEmpty(PRECOMPILED_HEADER):PRECOMPILED_HEADER = $$PWD/shared/simcube_gui_pch.h

DESTDIR = $$SIMCUBE_APP_PATH
TARGET = $$qtLibraryName($$TARGET)
TEMPLATE = app
