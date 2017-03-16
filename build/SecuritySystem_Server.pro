TEMPLATE = app
CONFIG += console

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../include/ \
               ../../Common/include \
               ../../Dependencies/unix/boost_1_63_0 \
               ../../Dependencies/unix/opencv-2.4.13 \
               ../../Dependencies/unix/casablanca/Release/include


LIBS += -L/usr/local/lib/ \
        -L/usr/lib/arm-linux-gnueabihf/ \
        -L/usr/local/ssl/lib

CONFIG(debug, debug | release) {
  TARGET = Argusd

  LIBS += -L../../../Common/build/Debug
  LIBS += -L../../../Dependencies/unix/casablanca/Release/Debug/Binaries

  LIBS += -lCommond \
}
CONFIG(release, debug | release) {
  TARGET = Argus

  LIBS += -L../../../Common/build/Release
  LIBS += -L../../../Dependencies/unix/casablanca/Release/Release/Binaries

  LIBS += -lCommon \
}

LIBS += -lcpprest \
        -lboost_system \
        -lssl \
        -lcrypto \
        -lopencv_calib3d \
        -lopencv_contrib \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_flann \
        -lopencv_gpu \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_legacy \
        -lopencv_ml \
        -lopencv_nonfree \
        -lopencv_objdetect \
        -lopencv_ocl \
        -lopencv_photo \
        -lopencv_stitching \
        -lopencv_superres \
        -lopencv_video \
        -lopencv_videostab

HEADERS += ../include/Internal/ServerSignals.h \
           ../include/Interfaces/CentralService.h \
           ../include/Config/ConfigFile.h

SOURCES += ../src/main.cpp \
           ../src/Internal/ServerSignals.cpp \
           ../src/Interfaces/CentralService.cpp \
           ../src/Config/ConfigFile.cpp
