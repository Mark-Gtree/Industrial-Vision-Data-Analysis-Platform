#include "processingthread.h"

ProcessingThread::ProcessingThread(QObject *parent)
    : QThread{parent}
{
    this->initThread();
}

ProcessingThread::~ProcessingThread()
{

}

void ProcessingThread::initThread()   //初始化图像处理线程
{
    startFlag = false;
    detectFlag=false;
    m_threadId = 0;



}

void ProcessingThread::setThreadId(int id)
{
    m_threadId = id;
}

void ProcessingThread::setSwitchFlag(bool switchFlag)
{
    startFlag = switchFlag;
}
void ProcessingThread::setdetectFlag(bool switchFlag)
{
    detectFlag = switchFlag;
}

void ProcessingThread::run()
{
    while(startFlag)
    {
            if(m_imageVector_1.size() == 1)
            {
               testMutex.lock();   //加锁,注意互斥量
                //图像算法处理代码
                if(detectFlag)
                {
                cv::Mat img=m_imageVector_1.at(0);
                cv:: Mat dst = img.clone();
                             if (!img.data&&!dst.data)
                             {
                              printf("error!");
                              return;
                             }

              cv::GaussianBlur(img, img, cv::Size(7, 7), 3, 3);


              threshold(img, img, 138, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
              cv::Mat img_Close;
              cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(-1, -1));
              morphologyEx(img, img_Close, cv::MORPH_CLOSE, kernel);
              //寻找最外层轮廓
              std::vector<std::vector<cv::Point>> contours;
              std::vector<cv::Vec4i> hierarchy;
              cv::findContours(img_Close, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE,cv:: Point());
                            if(contours.size()==1)
                             {
                             //绘制轮廓的最小外结圆
                             cv:: Point2f center;
                             float radius;
                             cv::  minEnclosingCircle(contours[0], center, radius);
                             cv::circle(dst, center, radius,cv:: Scalar(255, 255, 255), 5);
                            // printf("圆的半径: %f\n", radius);
                            qDebug()<<"x:"<<center.x<<endl;
                            qDebug()<<"y:"<<center.y<<endl;
                            //printf("X: %f\n", center.x);
                            //  printf("Y: %f\n", center.y);
                            cv::drawMarker(dst, cv::Point(center.x,center.y),cv::Scalar(255,255,255), cv::MARKER_TILTED_CROSS, 100, 5,8);
                            //m_data.push_back(center);
                            m_data_x.push_back(center.x);
                            m_data_y.push_back(center.y);
                             }
                QPixmap newPixmap_detected = cvMatToPixmap(dst);
                emit signal_newPixmap(newPixmap_detected,0);
                 }
                else
                {
                QPixmap newPixmap_1 = cvMatToPixmap(m_imageVector_1.at(0));//at(0)仅读
                //将处理好的图像发现到主界面
                emit signal_newPixmap(newPixmap_1,0);
                }
                testMutex.unlock();
                //使用完后清空容器
                m_imageVector_1.clear();//移除vector中的所有元素
            }
        msleep(20);   //此处缓冲可减少cpu运行率,注意不要慢于相机线程的缓冲
    }
}
//Mat->QImage
QImage ProcessingThread::cvMatToImage(const cv::Mat cvMat)
{
    if(cvMat.channels() > 1)
    {
        return QImage((const unsigned char*)(cvMat.data),cvMat.cols,cvMat.rows,QImage::Format_RGB888);
    }
    else
    {
        return QImage((const unsigned char*)(cvMat.data),cvMat.cols,cvMat.rows,QImage::Format_Indexed8);
    }
}

//可以将此处修改为算法处理函数
//Mat->QPixmap
QPixmap ProcessingThread::cvMatToPixmap(const cv::Mat cvMat)
{
    QImage myImage;
    //qDebug()<<"channels:"<<cvMat.channels();
    if(cvMat.channels() > 1)
    {
        myImage = QImage((const unsigned char*)(cvMat.data),cvMat.cols,cvMat.rows,QImage::Format_RGB888);   //彩色图
    }
    else
    {
        myImage = QImage((const unsigned char*)(cvMat.data),cvMat.cols,cvMat.rows,QImage::Format_Indexed8);   //灰度图
    }

    //QSize表示图像在界面上的显示尺寸
    return QPixmap::fromImage(myImage).scaled(QSize(500,500),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
}
