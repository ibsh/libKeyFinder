TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    audiodatatest.cpp \
    chromagramtest.cpp

DEPENDPATH  += /usr/local/lib
INCLUDEPATH += /usr/local/include

CONFIG -= ppc ppc64
CONFIG += x86 x86_64

LIBS += -lkeyfinder
LIBS += -lgtest

HEADERS += \
    audiodatatest.h \
    chromagramtest.h
