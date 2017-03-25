#*************************************************************************
#
# Copyright 2011-2013 Ibrahim Sha'ath
#
# This file is part of LibKeyFinder.
#
# LibKeyFinder is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LibKeyFinder is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.
#
#*************************************************************************

cache()

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11
LIBS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++

LIBS += -lkeyfinder

HEADERS += _testhelpers.h

SOURCES += \
    main.cpp \
    _testhelpers.cpp \
    audiodatatest.cpp \
    binodetest.cpp \
    chromagramtest.cpp \
    chromatransformtest.cpp \
    chromatransformfactorytest.cpp \
    constantstest.cpp \
    downsamplershortcuttest.cpp \
    fftadaptertest.cpp \
    keyclassifiertest.cpp \
    keyfindertest.cpp \
    lowpassfiltertest.cpp \
    lowpassfilterfactorytest.cpp \
    spectrumanalysertest.cpp \
    temporalwindowfactorytest.cpp \
    toneprofilestest.cpp \
    windowfunctiontest.cpp \
    workspacetest.cpp

macx{
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
  QMAKE_MAC_SDK = macosx10.12
  DEPENDPATH += /usr/local/lib
  INCLUDEPATH += /usr/local/include
  CONFIG -= ppc ppc64
  CONFIG += x86 x86_64
}

unix|macx{
  DEPENDPATH += /usr/local/lib
  INCLUDEPATH += /usr/local/include catch
  LIBS += -L/usr/local/lib -L/usr/lib
}

win32{
  INCLUDEPATH += C:/minGW32/local/include
  DEPENDPATH += C:/minGW32/local/bin
  LIBS += -LC:/minGW32/local/bin -LC:/minGW32/local/lib
}
