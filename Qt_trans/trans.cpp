#include "trans.h"
#include "./ui_坐标转换.h"
#include "xyz2lbh.h"
#include "lbh2xyz.h"

//该界面是软件进入后的主界面，不进行计算工作，用于引导分别进入两种转换模式的窗口

trans::trans(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::trans)
{
    ui->setupUi(this);
}

trans::~trans()
{
    delete ui;
}

//创建对话框对象并打开
void trans::on_pushButton1_clicked()//跳转进入直角坐标转大地坐标计算窗口
{
    XYZ2LBH *p=new XYZ2LBH;
    p->show();//显示对话框
}


void trans::on_pushButton2_clicked()//跳转进入大地坐标转直角坐标计算窗口
{
    LBH2XYZ *p=new LBH2XYZ;
    p->show();//显示对话框
}
