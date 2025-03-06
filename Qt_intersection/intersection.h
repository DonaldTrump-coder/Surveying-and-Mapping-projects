#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class intersection;
}
QT_END_NAMESPACE

class intersection : public QMainWindow
{
    Q_OBJECT

public:
    intersection(QWidget *parent = nullptr);
    ~intersection();

private slots:
    //声明跳转槽函数，用于分别调至两个不同的对话框
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::intersection *ui;
};
#endif // INTERSECTION_H
