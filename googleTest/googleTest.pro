TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG -= ppc ppc64
CONFIG += x86 x86_64

HEADERS += \
    audiodatatest.h \
    chromagramtest.h

SOURCES += main.cpp \
    audiodatatest.cpp \
    chromagramtest.cpp

DEPENDPATH  += /usr/local/lib
INCLUDEPATH += /usr/local/include

LIBS += -lkeyfinder
LIBS += -lgtest
