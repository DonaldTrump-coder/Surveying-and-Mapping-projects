#include "intersection.h"
#include "./ui_intersection.h"
#include"front_intersection.h"
#include"back_intersection.h"

intersection::intersection(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::intersection)
{
    ui->setupUi(this);
}

intersection::~intersection()
{
    delete ui;
}

//按下按钮后，跳转至前方交会的对话框
void intersection::on_pushButton_clicked()
{
    front_intersection *p=new front_intersection;//建立一个对话框对象
    p->show();//显示对话框
}

//按下按钮后，跳转至后方交会的对话框
void intersection::on_pushButton_2_clicked()
{
    back_intersection *p=new back_intersection;
    p->show();
}

