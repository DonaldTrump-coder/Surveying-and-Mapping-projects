#ifndef LBH2XYZ_H
#define LBH2XYZ_H

#include <QMainWindow>
#include "trans.h"

namespace Ui {
class LBH2XYZ;
}

class LBH2XYZ : public QMainWindow
{
    Q_OBJECT

public:
    explicit LBH2XYZ(QWidget *parent = nullptr);
    ~LBH2XYZ();

private slots:
    void on_pushButton_clicked();//声明计算按钮按下时的计算功能函数

    void on_pushButton_2_clicked();//声明“重新输入”按钮按下时的清除功能函数

private:
    Ui::LBH2XYZ *ui;
};

//在头文件中定义用于计算大地坐标转为直角坐标的类calculation
class calculation
{
private:
    double L,B,H,X,Y,Z;//定义变量存储一个点的XYZ、LBH坐标
    double N;//定义变量存储N值
public:
    calculation(double l,double b,double h);//在构造函数中传入已知的LBH坐标参数
    void calculate();//用于计算XYZ坐标的函数

    //用于将类中封装的XYZ坐标值传出，增强安全性
    //返回值为常量类型，用于提高计算结果的安全性
    const double showX();//返回X的值
    const double showY();//返回Y的值
    const double showZ();//返回Z的值
};

#endif // LBH2XYZ_H
