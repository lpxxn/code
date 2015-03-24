PROVIDER = script
include(../../simcubeplugin.pri)
include(../../../uselib/sc_QXlsx.pri)

QT += script

HEADERS += \
    officescript.h \
    xlsxdocument.h \
    docxdocument.h \
    xlsxhelper.h \
    typedeclare.h

SOURCES += \
    officescript.cpp \
    xlsxdocument.cpp \
    docxdocument.cpp \
    xlsxhelper.cpp
