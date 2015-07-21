PROVIDER = script
include(../../simcubeplugin.pri)
include(../../../uselib/sc_QXlsx.pri)

QT += script xml

HEADERS += \
    officescript.h \
    xlsxdocument.h \
    docxdocument.h \
    xlsxhelper.h \
    typedeclare.h \
    docxhelper.h

SOURCES += \
    officescript.cpp \
    xlsxdocument.cpp \
    docxdocument.cpp \
    xlsxhelper.cpp \
    docxhelper.cpp
