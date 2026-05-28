#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    myText = "";
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::setPixmap(QPixmap pixmap,QString text)
{
    myPixmap = pixmap;
    myText = text;
   // myw=w;
    //myh=h;
    this->update();
}

void MyGLWidget::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,500,500,myPixmap);   //如果使用drawImage,无法实现显示文字功能(文字会被覆盖)
    QPen pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawText(10,20,myText);   //界面上显示文字
    painter.end();
}
