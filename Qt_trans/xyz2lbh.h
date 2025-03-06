#ifndef XYZ2LBH_H
#define XYZ2LBH_H

#include <QMainWindow>
#include "trans.h"

namespace Ui {
class XYZ2LBH;
}

class XYZ2LBH : public QMainWindow
{
    Q_OBJECT

public:
    explicit XYZ2LBH(QWidget *parent = nullptr);
    ~XYZ2LBH();

private slots:
    void on_pushButton_clicked();//声明计算按钮按下时的计算功能函数

    void on_pushButton_2_clicked();//声明“重新输入”按钮按下时的清除功能函数

private:
    Ui::XYZ2LBH *ui;
};

//在头文件中定义用于计算直角坐标转为大地坐标的类calculation2
class calculation2
{
private:
    double X,Y,Z,L,B=0,H=0;//定义类中的变量用于存储一个点的大地坐标、直角坐标值，其中为便于计算，将B和H值置为初值0
    double N;//定义用于辅助计算的变量用于存储N的值
public:
    calculation2(double X,double Y,double Z);//构造函数用于传入已知参数
    void calculate();//用于实现计算功能的函数

    //用于将类中封装的LBH坐标值传出，增强安全性
    //返回值为常量类型，用于提高计算结果的安全性
    const double showL();//返回L的值
    const double showB();//返回B的值
    const double showH();//返回H的值
};

#endif // XYZ2LBH_H
