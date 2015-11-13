TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()
QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    instr.h

