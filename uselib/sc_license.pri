INCLUDEPATH *= $$SIMCUBE_SOURCE_TREE/include

win32:contains(QMAKE_HOST.arch, x86_64): {
    LIBS *= -L$$SIMCUBE_SOURCE_TREE/lib/license/win64 -lLicenseDll
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/license/win64/LicenseDll.dll
}

win32:contains(QMAKE_HOST.arch, x86): {
    LIBS *= -L$$SIMCUBE_SOURCE_TREE/lib/license/win32 -lLicenseDll
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/license/win32/LicenseDll.dll
}

unix: contains(QMAKE_HOST.arch, x86_64): {
    LIBS *= -L$$SIMCUBE_SOURCE_TREE/lib/license/linux64 -lLicenseDll
    QMAKE_RPATHDIR += $$SIMCUBE_SOURCE_TREE/lib/license/linux64
    FILES_COPY_SRC = $$SIMCUBE_SOURCE_TREE/lib/license/linux64/libLicenseDll.so
}

#拷贝依赖的动态链接库到app路径下
$$copyFile($$FILES_COPY_SRC, $$SIMCUBE_APP_PATH)
