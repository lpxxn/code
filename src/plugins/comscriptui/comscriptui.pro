PROVIDER = script
include(../../simcubeplugin.pri)

QT += script uitools

contains(DEFINES, USELOG) { # 拷贝日志配置文件
    include(../../../uselib/sc_log4qt.pri)
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/src/shared/log4qt.conf
    $$copyFile($$FILES_COPY_SRC, $$SIMCUBE_APP_PATH)
}

HEADERS += \
    uiscript.h \
    uiloader.h

SOURCES += \
    uiscript.cpp \
    uiloader.cpp
