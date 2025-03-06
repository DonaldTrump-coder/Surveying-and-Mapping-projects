#include "levelingnetwork_dlg.h"
#include "./ui_levelingnetwork_dlg.h"
#include "levelingnetwork.h"

levelingnetwork_dlg::levelingnetwork_dlg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::levelingnetwork_dlg)
{
    ui->setupUi(this);
}

levelingnetwork_dlg::~levelingnetwork_dlg()
{
    delete ui;
}

levelingnetwork a;//在当前对话框中建立一个全局变量，一个水准网对象

void levelingnetwork_dlg::on_pushButton_clicked()
{
    //在计算初值之前先进行初始化，清除上次计算保留的数据
    a.init();

    //从输入文本框中获取数据并拆分后存储于水准网对象中
    a.settext(ui->input->toPlainText());

    //若输入数据格式有误，则停止后续计算
    if(a.judge()==1)
        return;

    //分别根据已知点、未知点高程计算未知点的高程估值
    a.calculate_appro_using_knownpoint();
    a.calculate_appro_using_unknownpoint();

    //输出所有点的高程或高程估值
    ui->output->setText(a.output_text());
}

void levelingnetwork_dlg::on_pushButton_2_clicked()
{
    //清除输入、输出框中的内容
    ui->input->clear();
    ui->output->clear();
}

//用于显示权矩阵的响应函数
void levelingnetwork_dlg::on_pushButton_3_clicked()
{
    //根据水准网对象中的数据计算并显示观测对应的权矩阵
    ui->output->setText(a.show_weight());
}
