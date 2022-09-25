QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    include/extract_exif.h \
    include/formanalyze.h \
    include/handlers.h \
    include/input_bin_file.h \
    include/stdafx.h \
    include/win.h

SOURCES += \
    src/extract_exif.cpp \
    src/formanalyze.cpp \
    src/handlers.cpp \
    src/input_bin_file.cpp \
    src/main.cpp \
    src/stdafx.cpp \
    src/win.cpp

#include(include/include.pri)

FORMS += \
    formanalyze.ui \
    win.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
