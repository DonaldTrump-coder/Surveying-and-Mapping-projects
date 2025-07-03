#ifndef GRAPHIC_ITEM_H
#define GRAPHIC_ITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include "adjustment.h"

enum drawmode
{
    Init,Points,Precision
};

class graphic_item : public QGraphicsItem
{
private:
    //绘图模式
    drawmode mode=Init;

    //绘图窗口大小
    QRectF size;

    //绘图机制对应的平差计算对象
    adjustment* adj;

    //尺度信息，绘图距离/实际距离
    double x_scale;
    double y_scale;

    //实际坐标的中点
    double center_x;
    double center_y;

public:
    graphic_item();
    ~graphic_item();

    //将平差计算对象关联至绘图对象
    void set_data(adjustment* adj);
    QRectF boundingRect() const override;

    //重写控件的绘图函数
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //获取绘图窗口大小
    void get_size(QRectF window_size);

    //该边绘图模式的外部接口
    void Set_drawing_mode(drawmode mode);

    //绘制点和点号的函数
    void draw_control_point(QPainter *painter,double x,double y,QString name);
    void draw_unknown_point(QPainter *painter,double x,double y,QString name);

    //绘制边的函数
    void draw_control_line(QPainter *painter,double x1,double y1,double x2,double y2);
    void draw_line(QPainter *painter,double x1,double y1,double x2,double y2);

    //绘制误差椭圆的函数
    void draw_ellipse(QPainter *painter,double x,double y,double angle,double long_r,double short_r);
};

#endif // GRAPHIC_ITEM_H
