#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initWidget();
}

MainWindow::~MainWindow()
{

        if(m_pcMyCamera)
        {
            m_pcMyCamera->Close();
            delete m_pcMyCamera;
            m_pcMyCamera = NULL;
        }

        //关闭线程,防止关闭窗口出错
        if(m_cameraThread->isRunning())
        {
            m_cameraThread->setSwitchFlag(false);
        }
        if(m_processingThread->isRunning())
        {
            m_processingThread->setSwitchFlag(false);
        }

        m_data_x.clear();
          m_data_y.clear();
}

void MainWindow::initWidget()
{
    //初始化变量
    m_deviceNum = 0;
    m_bOpenDevice = false;
    m_savePath = "";
    m_cameraOnePath = "";
    m_endsavedataPath="";
    //生成保存图片的文件夹
    QString curDate = QDate::currentDate().toString("yyyy-MM-dd");
    m_savePath = QDir::currentPath() + "/SaveImages/" + curDate + "/";
    QDir saveDir(m_savePath);
    if(!saveDir.exists())
    {
        if(!saveDir.mkpath(m_savePath))
        {
            qDebug()<<"创建文件夹失败！";
            //return;
        }
    }
    //生成保存数据的文件夹
    m_savedataPath = QDir::currentPath() + "/SavePoint/" + curDate+ "/" ;
    qDebug()<<m_savePath<<endl;
    QDir saveDir_data(m_savedataPath);
    if(!saveDir_data.exists())
    {
        if(!saveDir.mkpath(m_savedataPath))
        {
            qDebug()<<"创建文件夹失败！";
            //return;
        }
    }


        //相机对象
        m_pcMyCamera = new CMvCamera;

        //相机线程对象实例化
        m_cameraThread= new CameraThread();

        //处理线程对象实例化
        m_processingThread = new ProcessingThread();

        //保存数据线程对象实例化
        m_excel=new ExcelOperator;

        //绘制plot对象实例化
        m_CustomPlot=new QCustomPlot(ui->draw_data);
        m_CustomPlot->resize(ui->draw_data->width(), ui->draw_data->height());
        m_CustomPlot->xAxis->setRange(0,2000);
        m_CustomPlot->yAxis->setRange(0, 2000);
        m_CustomPlot->xAxis->setLabel("X");
        m_CustomPlot->yAxis->setLabel("Y");
        connect(m_processingThread,SIGNAL(signal_newPixmap(QPixmap,int)),this,SLOT(slot_displayPixmap(QPixmap,int)),Qt::BlockingQueuedConnection);


    //初始化相机
    memset(&m_stDevList,0,sizeof(MV_CC_DEVICE_INFO_LIST));
    //枚举子网内所有设备
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE,&m_stDevList);
    if(MV_OK != nRet)
    {
        return;
    }
    m_deviceNum = m_stDevList.nDeviceNum;
    if(m_deviceNum== 0)
    {
        QMessageBox::warning(this,"警告","请检查相机是否正常连接！");
        return;
    }
    for(int i=0;i<m_deviceNum;i++)
    {
        MV_CC_DEVICE_INFO *pDeviceInfo = m_stDevList.pDeviceInfo[i];
        int nRet = m_pcMyCamera->Open(pDeviceInfo);   //打开相机
        if(MV_OK != nRet)
        {
            delete m_pcMyCamera;
            m_pcMyCamera = NULL;
            QMessageBox::warning(this,"警告","打开设备失败！");
            return;
        }
        m_pcMyCamera->SetEnumValue("TriggerMode",1);        //设置为触发模式
        m_pcMyCamera->SetEnumValue("TriggerSource",7);      //设置触发源为软触发
        m_pcMyCamera->SetFloatValue("ExposureTime",5000);   //设置曝光时间
        m_cameraThread->setCameraPtr(m_pcMyCamera);
        m_cameraThread->setCameraIndex(i);
        m_processingThread->setThreadId(i);
    }
}

void MainWindow::saveImage(QString format,int index)
{
    cv::Mat image;
    //当前时间
    QString curTime = QTime::currentTime().toString("hh_mm_ss");
    if(index == 1)
    {
        //format: .bmp .tif .png .jpg
        QString saveNameOne = m_cameraOnePath + "Grab_" + curTime + "." + format;
        m_pcMyCamera->CommandExecute("TriggerSoftware");
        int readFlag = m_pcMyCamera->SaveBuffer(saveNameOne.toUtf8());
        if(MV_OK == readFlag)
        {
            qDebug()<<"相机一图像保存成功！";
        }
        else
        {
            qDebug()<<"相机一图像保存失败！";
        }
    }
}

void MainWindow::slot_displayPixmap(QPixmap newPixmap,int index)
{
    if(newPixmap.isNull())
    {
        qDebug()<<"newPixmap is a null image!"<<"   index:"<<index;
        return;
    }
    if(index == 0)
    {
  //    int w=ui->gl_imageOne->width();
    //  int h=ui->gl_imageOne->height();
        ui->gl_imageOne->setPixmap(newPixmap,"Camera:live");   //使用OpenGLWidget显示
    }
}

void MainWindow::on_pb_start_clicked()
{
    if(!m_bOpenDevice)
    {
        m_bOpenDevice = true;
    }

        if(m_pcMyCamera->IsDeviceConnected())
        {
            m_pcMyCamera->StartGrabbing();   //开启相机采集
            if(!m_cameraThread->isRunning())
            {
                m_cameraThread->setSwitchFlag(true);
                m_cameraThread->start();
            }
            if(!m_processingThread->isRunning())
            {
                m_processingThread->setSwitchFlag(true);
                m_processingThread->start();
            }
        }

}

void MainWindow::on_pb_stop_clicked()
{
    if(m_bOpenDevice)
    {
        m_bOpenDevice = false;
    }
        m_pcMyCamera->StopGrabbing();   //关闭相机采集
        if(m_cameraThread->isRunning())
        {
            m_cameraThread->setSwitchFlag(false);
        }
        if(m_processingThread->isRunning())
        {
            m_processingThread->setSwitchFlag(false);
        }

}

void MainWindow::on_pb_saveOne_clicked()
{
    if(!m_bOpenDevice)
    {
        QMessageBox::warning(this,"警告","请开启相机采集！");
    }
    m_cameraOnePath = m_savePath + "Camera/";
    QDir dstDir(m_cameraOnePath);
    if(!dstDir.exists())
    {
        if(!dstDir.mkpath(m_cameraOnePath))
        {
            qDebug()<<"创建Camera1文件夹失败！";
            //return;
        }
    }
    saveImage("bmp",1);
}

void MainWindow::on_pb_detect_start_clicked()
{

                m_processingThread->setdetectFlag(true);

}

void MainWindow::on_pb_detect_stop_clicked()
{
        m_processingThread->setdetectFlag(false);
}

void MainWindow::on_pb_Save_Data_clicked()
{
    //QString sPath = QFileDialog::getOpenFileName(this,"保存文件对话框",".","(*.xls *.xlsx)");
    QString curTime = QTime::currentTime().toString("hh_mm_ss");
    QString endsavedataPath = m_savedataPath + "Data_" + curTime + ".xlsx";
    m_excel->open(m_savedataPath);
   // m_excel->addSheet("Sheet1");//新建sheet
   QAxObject* workSheet= m_excel->getSheet("Sheet1");
    m_excel->setCell(workSheet,1,1,"Frequency");
    m_excel->setCell(workSheet,1,2,"X");
    m_excel->setCell(workSheet,1,3,"Y");
    for(int i=2;i<(m_data_x.size()+2);i++)
    {
        m_excel->setCell(workSheet,i,1,QString("%1").arg(i-1));//第一列  时间
         m_excel->setCell(workSheet,i,2,QString("%1").arg(m_data_x[i-2]));//第二列 X
          m_excel->setCell(workSheet,i,3,QString("%1").arg(m_data_y[i-2]));//第三列 Y
    }
    m_excel->close(endsavedataPath);
}

void MainWindow::on_pb_Draw_Data_clicked()
{
    m_CustomPlot->addGraph(m_CustomPlot->yAxis, m_CustomPlot->xAxis);
    QPen drawPen;
    drawPen.setColor(Qt::red);
    drawPen.setWidth(4);
    QCPGraph * curGraph = m_CustomPlot->addGraph();
    curGraph->setPen(drawPen);
    curGraph->setLineStyle(QCPGraph::lsNone);
    curGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    QVector<double> drawX_Vector;      QVector<double>drawY_Vector;
    for(int i=0;i<m_data_x.size();i++)
    {
        double sawp_x=m_data_x[i];
        double sawp_y=m_data_y[i];
        drawX_Vector.push_back(sawp_x);
        drawY_Vector.push_back(sawp_y);
       }
    curGraph->setData(drawX_Vector, drawY_Vector);
    m_CustomPlot->xAxis->setVisible(true);
    m_CustomPlot->yAxis->setVisible(true);
     QString curTime = QTime::currentTime().toString("hh_mm_ss");
   QString path =m_savedataPath + "Draw_" + curTime + ".jpg";
    m_CustomPlot->savePng(path);
    drawX_Vector.clear();
    drawY_Vector.clear();
}
