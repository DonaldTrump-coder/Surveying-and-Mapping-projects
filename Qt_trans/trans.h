#ifndef TRANS_H
#define TRANS_H

//该头文件中对坐标转换过程中可能用到的常量进行了定义
#include <QMainWindow>
#include<math.h>
#define pie 3.1415926535897932384626//定义圆周率

const double a=6378137.0;//定义半长轴a
const double a2=a*a;//定义a的平方值
const double alpha=1.0/298.257223563;//定义扁率α
const double e2=(2.0-alpha)*alpha;//定义椭球偏心率的平方

QT_BEGIN_NAMESPACE
namespace Ui {
class trans;
}
QT_END_NAMESPACE

class trans : public QMainWindow
{
    Q_OBJECT

public:
    trans(QWidget *parent = nullptr);
    ~trans();

private slots://对两个按钮按下时的跳转对话框功能函数进行声明
    void on_pushButton1_clicked();

    void on_pushButton2_clicked();

private:
    Ui::trans *ui;
};
#endif // TRANS_H
