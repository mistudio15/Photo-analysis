######################################################################
# Automatically generated by qmake (3.1) Tue Sep 27 23:55:17 2022
######################################################################

QT       += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TEMPLATE = app
TARGET = ExifAlalyze
INCLUDEPATH += include

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += include/extract_exif.h \
           include/formanalyze.h \
           include/formgraph.h \
           include/handlers.h \
           include/input_bin_file.h \
           include/qcustomplot.h \
           include/stdafx.h \
           include/win.h
FORMS += formanalyze.ui formgraph.ui win.ui
SOURCES += src/extract_exif.cpp \
           src/formanalyze.cpp \
           src/formgraph.cpp \
           src/handlers.cpp \
           src/input_bin_file.cpp \
           src/main.cpp \
           src/qcustomplot.cpp \
           src/stdafx.cpp \
           src/win.cpp