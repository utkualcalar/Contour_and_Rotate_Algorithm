QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RTSP_CONTOUR
TEMPLATE = app

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ffmpegdecoder_contour.cpp \
    login_contour.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ffmpegdecoder_contour.h \
    login_contour.h \
    mainwindow.h

FORMS += \
    login_contour.ui \
    mainwindow.ui

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_videoio

INCLUDEPATH += /usr/include/opencv4
LIBS += -L /usr/local/lib -lpthread -lavcodec  -lavformat  -lavutil  -lswscale -lswresample -lavdevice -lavfilter


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
