#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T16:34:19
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectiondialog.cpp \
    sqlconnection.cpp \
    employee.cpp \
    facedetection.cpp \
    facerecogition.cpp

HEADERS  += mainwindow.h \
    connectiondialog.h \
    sqlconnection.h \
    employee.h \
    facedetection.h \
    facerecogition.h

FORMS    += mainwindow.ui \
    connectiondialog.ui \

INCLUDEPATH += C:\\opencv\\mybuild\\install\\include

LIBS += -LC:\\opencv\\mybuild\\lib \
-lopencv_calib3d300 \
-lopencv_core300 \
-lopencv_features2d300 \
-lopencv_flann300 \
-lopencv_highgui300 \
-lopencv_imgproc300 \
-lopencv_ml300 \
-lopencv_objdetect300 \
-lopencv_photo300 \
-lopencv_stitching300 \
-lopencv_superres300 \
-lopencv_ts300 \
-lopencv_video300 \
-lopencv_videostab300 \
-lopencv_imgcodecs300 \
-lopencv_shape300 \
-lopencv_videoio300 \
-lopencv_face300
