#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>
#include "globalvar.h"

class CameraThread : public QThread
{
    Q_OBJECT

public:
    explicit CameraThread(QObject *parent = nullptr);
     ~CameraThread();

    void initThread();
    void setCameraPtr(CMvCamera *camera);
    void setCameraIndex(int index);
    void setSwitchFlag(bool switchFlag);

    void run();

signals:
    void signal_messImage(QImage myImage,int index);

private:
    bool startFlag;
    int m_cameraIndex;

    CMvCamera *cameraPtr = NULL;
};

#endif // MYTHREAD_H
