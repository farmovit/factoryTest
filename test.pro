TARGET = testApp

CONFIG += console
CONFIG += -std=c++1z
QMAKE_CXXFLAGS += -std=c++17
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += *.cpp
HEADERS += *.h
