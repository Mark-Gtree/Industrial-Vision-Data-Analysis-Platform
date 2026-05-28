#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include"qcustomplot.h"
#include "HikSdk/cmvcamera.h"
#include "HikSdk/camerathread.h"
#include "HikSdk/processingthread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initWidget();
    void saveImage(QString format,int index);

private slots:
    void slot_displayPixmap(QPixmap newPixmap,int index);

    void on_pb_start_clicked();
    void on_pb_stop_clicked();
    void on_pb_saveOne_clicked();

    void on_pb_detect_start_clicked();

    void on_pb_detect_stop_clicked();

    void on_pb_Save_Data_clicked();

    void on_pb_Draw_Data_clicked();

private:
    Ui::MainWindow *ui;

    int m_deviceNum;
    bool m_bOpenDevice;
    QString m_savePath;
    QString m_cameraOnePath;
     QString m_savedataPath;
     QString m_endsavedataPath;

    MV_CC_DEVICE_INFO_LIST m_stDevList;        //设备信息列表结构体变量，用来存储设备列表
    MV_CC_DEVICE_INFO *m_DeviceInfo;
    CMvCamera *m_pcMyCamera;                //相机指针对象
    CameraThread *m_cameraThread;           //相机线程对象
    ProcessingThread *m_processingThread;   //处理线程对象
    ExcelOperator *m_excel;
     QCustomPlot *m_CustomPlot;
};
#endif // MAINWINDOW_H
