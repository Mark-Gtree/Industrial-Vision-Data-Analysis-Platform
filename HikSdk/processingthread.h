#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H
#include <QDate>
#include <QObject>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QMutex>
#include<vector>
#include "globalvar.h"
#include"exceloperator.h"
#include "ui_mainwindow.h"
class ProcessingThread : public QThread
{
    Q_OBJECT

public:
    explicit ProcessingThread(QObject *parent = nullptr);
     ~ProcessingThread();

    void initThread();
    void setThreadId(int id);
    void setSwitchFlag(bool switchFlag);
    void setdetectFlag(bool switchFlag);
    void run();

    QImage cvMatToImage(const cv::Mat cvMat);
    QPixmap cvMatToPixmap(const cv::Mat cvMat);

signals:
    void signal_newPixmap(QPixmap newPixmap,int id);

private:
    QMutex testMutex;
    Ui::MainWindow *ui;
    bool startFlag;
    bool detectFlag;
    int m_threadId;


};
#endif // PROCESSINGTHREAD_H
