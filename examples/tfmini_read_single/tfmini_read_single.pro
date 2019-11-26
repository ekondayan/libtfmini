QT += serialport
QT -= gui
CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = tfmini_read_single

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

linux-rasp-* {
  QMAKE_CXXFLAGS += -march=armv8-a -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8 -mfloat-abi=hard -funsafe-math-optimizations
}
else {
  QMAKE_CXXFLAGS += -march=native
}

CONFIG(release, debug|release) {
   QMAKE_CXXFLAGS += -O3
}

CONFIG(debug, debug|release) {
   QMAKE_CXXFLAGS += -O0 -g
}


QMAKE_CXXFLAGS += -std=c++17
#QMAKE_LFLAGS += -Xlinker -Map=output.map

SOURCES += \
        main.cpp \
    bsp_tfmini.cpp

HEADERS += \
    bsp_tfmini.h \
    ../../src/tfmini.h \
    ../../src/tfmini_comm.h \
    ../../src/tfmini_defs.h
