#include "camerathread.h"

CameraThread::CameraThread(QObject *parent)
    : QThread{parent}
{
    this->initThread();
}

CameraThread::~CameraThread()
{
    if(cameraPtr == NULL)
    {
        delete cameraPtr;
    }
}

void CameraThread::initThread()
{
    startFlag = false;
    m_cameraIndex = 0;
}

void CameraThread::setCameraPtr(CMvCamera *camera)
{
    cameraPtr = camera;
}

void CameraThread::setCameraIndex(int index)
{
    m_cameraIndex = index;
}

void CameraThread::setSwitchFlag(bool switchFlag)
{
    startFlag = switchFlag;
}

void CameraThread::run()
{
    if(cameraPtr == NULL)
    {
        return;
    }
    while(startFlag)
    {
        //将图片添加到处理线程
        cv::Mat imagePtr;
        cameraPtr->CommandExecute("TriggerSoftware");
        cameraPtr->ReadBuffer(imagePtr);

        //qDebug()<<"m_cameraIndex:"<<m_cameraIndex;
        //添加到容器
        if(m_cameraIndex == 0)
        {
            m_imageVector_1.push_back(imagePtr);//从尾部插入数据 cv::Mat imagePtr
        }
        msleep(30);   //适当缓冲,减少cpu运行率
    }
}

