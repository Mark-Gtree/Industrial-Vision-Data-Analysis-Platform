#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QVector>
#include "cmvcamera.h"
//通过使用extern关键字声明变量名而不定义它
extern QVector<cv::Mat> m_imageVector_1;   //帧保存容器

extern QVector<cv::Point2f> m_data;   //数据保存容器

extern QVector<std::float_t> m_data_x;

extern QVector<std::float_t> m_data_y;
#endif // GLOBALVAR_H
