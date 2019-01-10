CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -O3

CONFIG(debug, debug|release) {
    TARGET_PATH = $$OUT_PWD/debug
}
CONFIG(release, debug|release) {
    TARGET_PATH = $$OUT_PWD/release
}
#TODO: make config for Unix and release
win32: CONFIG(debug, debug|release): QMAKE_POST_LINK += $$quote(copy $$shell_path($$TARGET_PATH/landscape.exe) $$shell_path($$OUT_PWD/../gui/debug/landscape.exe))

SOURCES += \
    cellular_automata.cpp \
    diamond_square.cpp \
    hill_algorithm.cpp \
    landscape.cpp \
    perlin_noise.cpp \
    rivers_generator.cpp \
    binary_heap.cpp \
    mapfield.cpp \
    node.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cellular_automata.h \
    diamond_square.h \
    hill_algorithm.h \
    landscape.h \
    perlin_noise.h \
    rivers_generator.h \
    binary_heap.h \
    mapfield.h \
    node.h
