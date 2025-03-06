#ifndef POLYGON_CALCULATE_H
#define POLYGON_CALCULATE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class polygon_calculate;
}
QT_END_NAMESPACE

class polygon_calculate : public QMainWindow
{
    Q_OBJECT

public:
    polygon_calculate(QWidget *parent = nullptr);
    ~polygon_calculate();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::polygon_calculate *ui;
};
#endif // POLYGON_CALCULATE_H


//为便于存储点坐标，建立表示点的结构体，同时存储一个点的x和y坐标
struct Point
{
    double x;
    double y;
};

//将计算、检查功能都封装进名为calculation的类中
class calculation
{
private:
    int N;//表示多边形上点的总个数，用于建立n个点的动态数组
    Point* p;//在类中定义一个点结构体（数组）指针，用于访问点的数组，访问多边形上的每一个点
    bool wrong;//用布尔值表示输入的数据格式是否正确，用于检查
public:
    calculation(QString text);//类的构造函数，用于传入用户输入的文本内容
    double calculate();//主要实现计算功能的函数，返回值即为面积计算结果
    ~calculation();//析构函数中，需要手动释放动态数组内存
    void test(QStringList list);//检查函数，检查输入内容是否格式正确
    void warn();//警告函数，用于在格式不正确的时候弹出警告框
};
