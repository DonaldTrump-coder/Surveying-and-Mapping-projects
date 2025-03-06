#ifndef FRONT_INTERSECTION_H
#define FRONT_INTERSECTION_H

#include <QMainWindow>

namespace Ui {
class front_intersection;
}

class front_intersection : public QMainWindow
{
    Q_OBJECT

public:
    explicit front_intersection(QWidget *parent = nullptr);
    ~front_intersection();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::front_intersection *ui;
};

//在对话框中定义一个用于计算的类
class calculation
{
private:
    double xa,xb,ya,yb;//定义double类型变量用于存储输入的点坐标
    double alpha,beta;//存储输入的观测角度
    double xp,yp;//存储用于输出的目标点坐标
    bool wrong=0;//用布尔变量表征输入数据是否合规
public:
    calculation(double XA,double YA,double XB,double YB,double ALPHA,double BETA);
    //构造函数，用于初始化参数成员

    void calculate();
    //用于实现计算功能的函数

    const double showx();
    const double showy();
    //用于返回目标点坐标的函数

    void test();
    //判断输入数据是否有误

    void warn();
    //用于弹出警告框的函数
};

#endif // FRONT_INTERSECTION_H
