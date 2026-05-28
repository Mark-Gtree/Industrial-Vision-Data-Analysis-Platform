#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>
class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

    void setPixmap(QPixmap pixmap,QString text);

protected:
    void paintEvent(QPaintEvent *);   //重写paintEvent

private:
    QPixmap myPixmap;
    QString myText;
 //   int myw;
   // int myh;

};
#endif // MYGLWIDGET_H
