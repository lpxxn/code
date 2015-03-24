INCLUDEPATH *= $$THIRDPARTYINCLUDE

win32-msvc*:contains(QMAKE_HOST.arch, x86_64): {
    LIBS += -L$$SIMCUBE_SOURCE_TREE/lib/log4qt/win64/msvc -l$$qtLibraryName(log4qt)
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/log4qt/win64/msvc/$$qtLibraryName(log4qt).dll
}

win32-g++:contains(QMAKE_HOST.arch, x86_64): {
    LIBS += -L$$SIMCUBE_SOURCE_TREE/lib/log4qt/win64/gcc -l$$qtLibraryName(log4qt)
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/log4qt/win64/gcc/$$qtLibraryName(log4qt).dll
}

win32-msvc*:contains(QMAKE_HOST.arch, x86): {
    LIBS += -L$$SIMCUBE_SOURCE_TREE/lib/log4qt/win32/msvc -l$$qtLibraryName(log4qt)
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/log4qt/win32/msvc/$$qtLibraryName(log4qt).dll
}

win32-g++:contains(QMAKE_HOST.arch, x86): {
    LIBS += -L$$SIMCUBE_SOURCE_TREE/lib/log4qt/win32/gcc -l$$qtLibraryName(log4qt)
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/log4qt/win32/gcc/$$qtLibraryName(log4qt).dll
}

unix: contains(QMAKE_HOST.arch, x86_64): {
    LIBS *= -L$$SIMCUBE_SOURCE_TREE/lib/log4qt/linux64 -l$$qtLibraryName(log4qt)
    QMAKE_RPATHDIR += $$SIMCUBE_SOURCE_TREE/lib/log4qt/linux64
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/log4qt/linux64/liblog4qt.so
}


#拷贝依赖的动态链接库到app路径下
$$copyFile($$FILES_COPY_SRC, $$SIMCUBE_APP_PATH)
