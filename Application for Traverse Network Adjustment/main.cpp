#include "application.h"

#include <QApplication>
#include <QDebug>
#include "matrix.h"

void test();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application w;
    w.show();
    return a.exec();
}

void test()
{
    Matrix a(3,3);
    a.SetMatrix_ele(0,0,1);
    a.SetMatrix_ele(0,1,2);
    a.SetMatrix_ele(0,2,3);

    a.SetMatrix_ele(1,0,0);
    a.SetMatrix_ele(1,1,1);
    a.SetMatrix_ele(1,2,4);

    a.SetMatrix_ele(2,0,5);
    a.SetMatrix_ele(2,1,6);
    a.SetMatrix_ele(2,2,0);

    Matrix b=a.inv();
    qDebug()<<b.getMatrix_ele(2,1);
}
