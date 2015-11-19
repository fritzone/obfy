TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++11 -Wextra -Wall

HEADERS += \
    instr.h

DISTFILES += \
    limitations.txt

