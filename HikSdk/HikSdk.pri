HEADERS += \
    $$PWD/camerathread.h \
    $$PWD/cmvcamera.h \
    $$PWD/globalvar.h \
    $$PWD/processingthread.h

SOURCES += \
    $$PWD/camerathread.cpp \
    $$PWD/cmvcamera.cpp \
    $$PWD/globalvar.cpp \
    $$PWD/processingthread.cpp

msvc:
{
    #OpenCV
    LIBS += -L$$PWD/OpenCV/OpenCV_Msvc2/lib/ -lopencv_world460d
    INCLUDEPATH += $$PWD/OpenCV/OpenCV_Msvc2/Includes
    DEPENDPATH += $$PWD/OpenCV/OpenCV_Msvc2/Includes


INCLUDEPATH += $$PWD/../../../Opencv/opencv/build/x64/vc15
DEPENDPATH += $$PWD/../../../Opencv/opencv/build/x64/vc15

    #SDK
    LIBS += -L$$PWD/SDK/Lib/ -lMvCameraControl
    INCLUDEPATH += $$PWD/SDK/Includes
    DEPENDPATH += $$PWD/SDK/Includes
}


