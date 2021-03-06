#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T11:30:59
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_egcastest_view
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11

LIBS += -legcas


SOURCES += tst_egcastest_view.cpp \
           ../../src/view/egcformulaitem.cpp \
    ../../src/view/egcasscene.cpp \
    ../../src/view/egcpixmapitem.cpp \
    ../../src/view/egctextitem.cpp \
    ../../src/view/resizehandle.cpp \

HEADERS += ../../src/view/egcformulaitem.h \
    ../../src/view/egcasscene.h \
    ../../src/view/egcpixmapitem.h \
    ../../src/view/egctextitem.h \
    ../../src/view/resizehandle.h



DEFINES += SRCDIR=\\\"$$PWD/\\\"
