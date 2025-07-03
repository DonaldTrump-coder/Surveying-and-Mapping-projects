#include "application.h"
#include "./ui_application.h"
#include "tools.h"

Application::Application(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Application)
{
    ui->setupUi(this);
    this->setWindowTitle("导线网计算应用程序");

    /*菜单*/
    menubar=new QMenuBar();
    this->setMenuBar(menubar);
    menu1=new QMenu("文件");
    menu2=new QMenu("计算");
    menu3=new QMenu("结果");
    menu4=new QMenu("帮助");
    menubar->addMenu(menu1);
    menubar->addMenu(menu2);
    menubar->addMenu(menu3);
    menubar->addMenu(menu4);

    action1=new QAction("打开文件");
    action2=new QAction("导出文件");
    menu1->addAction(action1);
    menu1->addAction(action2);

    action3=new QAction("坐标概算");
    action4=new QAction("坐标平差");
    action5=new QAction("精度评定");
    menu2->addAction(action3);
    menu2->addAction(action4);
    menu2->addAction(action5);

    action6=new QAction("成果显示");
    menu3->addAction(action6);

    action7=new QAction("帮助页面");
    menu4->addAction(action7);
    /*菜单*/

    /*工具栏*/
    toolbar=new QToolBar("工具栏");
    this->addToolBar(toolbar);
    open_tool=new QAction(QIcon(":/icons/Icons/open_tool.png"),"打开文件",this);
    save_tool=new QAction(QIcon(":/icons/Icons/save_tool.png"),"保存结果",this);
    calculate_tool=new QAction(QIcon(":/icons/Icons/calculate_tool.png"),"坐标计算",this);
    precision_tool=new QAction(QIcon(":/icons/Icons/precision_tool.png"),"精度评定",this);
    display_tool=new QAction(QIcon(":/icons/Icons/display_tool.png"),"结果显示",this);
    help_tool=new QAction(QIcon(":/icons/Icons/help_tool.png"),"帮助页面",this);
    toolbar->addAction(open_tool);
    toolbar->addAction(save_tool);
    toolbar->addAction(calculate_tool);
    toolbar->addAction(precision_tool);
    toolbar->addAction(display_tool);
    toolbar->addAction(help_tool);
    /*工具栏*/

    /*状态栏*/
    status_bar=new QStatusBar();
    this->setStatusBar(status_bar);
    statuslabel=new QLabel("未导入文件",this);
    status_bar->addWidget(statuslabel);
    /*状态栏*/

    ui->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
    ui->plainTextEdit_2->setWordWrapMode(QTextOption::NoWrap);

    connect(ui->Button_Fileinput,&QPushButton::clicked,this,&Application::FileInput);
    connect(ui->Button_Estimation,&QPushButton::clicked,this,&Application::Estimation);
    connect(ui->Button_Adjust,&QPushButton::clicked,this,&Application::Adjust);
    connect(ui->Button_Display,&QPushButton::clicked,this,&Application::Display);
    connect(ui->Button_Precision,&QPushButton::clicked,this,&Application::Get_Precision);
    connect(ui->Button_Output,&QPushButton::clicked,this,&Application::FileOutput);

    connect(action1,&QAction::triggered,this,&Application::FileInput);
    connect(action2,&QAction::triggered,this,&Application::FileOutput);
    connect(action3,&QAction::triggered,this,&Application::Estimation);
    connect(action4,&QAction::triggered,this,&Application::Adjust);
    connect(action5,&QAction::triggered,this,&Application::Get_Precision);
    connect(action6,&QAction::triggered,this,&Application::Display);
    connect(action7,&QAction::triggered,this,&Application::get_helping);

    connect(open_tool,&QAction::triggered,this,&Application::FileInput);
    connect(save_tool,&QAction::triggered,this,&Application::FileOutput);
    connect(calculate_tool,&QAction::triggered,this,&Application::calculation);
    connect(precision_tool,&QAction::triggered,this,&Application::Get_Precision);
    connect(display_tool,&QAction::triggered,this,&Application::Display);
    connect(help_tool,&QAction::triggered,this,&Application::get_helping);


    this->show();
    item=new graphic_item();
    item->setPos(0,0);
    scene.addItem(item);
    scene.setSceneRect(ui->graphicsView->viewport()->rect());
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    item->get_size(ui->graphicsView->viewport()->rect());
    item->set_data(&adj);
}

//管理程序的变量
Application::~Application()
{
    delete action1;
    delete action2;
    delete action3;
    delete action4;
    delete action5;
    delete action6;
    delete action7;
    delete menu1;
    delete menu2;
    delete menu3;
    delete menu4;
    delete menubar;

    delete open_tool;
    delete save_tool;
    delete calculate_tool;
    delete precision_tool;
    delete display_tool;
    delete help_tool;
    delete toolbar;

    delete statuslabel;
    delete status_bar;

    delete helping_dialog;

    delete item;
    delete ui;
}

void Application::FileInput()
{
    filecontext=getopenfile(getopenfilename());
    if(filecontext.isEmpty())
    {
        warning("文件导入失败！");
        return;
    }
    ui->plainTextEdit->setPlainText(filecontext);

    adj.init();
    item->Set_drawing_mode(Init);
    item->update();
    ui->plainTextEdit_2->clear();
    adj.get_data(filecontext);
    applicationmode=Applicationmode::Inputed;
    statuslabel->setText("已导入数据，未进行概算");
}

void Application::Estimation()
{
    if(applicationmode==Applicationmode::Init)
    {
        warning("请先导入观测文件！");
        return;
    }
    if(adj.get_control_points_wrong()==1)
    {
        warning("观测信息中控制点错误！");
        return;
    }
    if(adj.get_data_wrong()==1)
    {
        warning("请重新导入观测文件！");
        return;
    }
    item->Set_drawing_mode(Init);
    item->update();
    ui->plainTextEdit_2->clear();
    add_obs_from_text();
    adj.get_ang_obs();
    adj.get_coarse();
    applicationmode=Applicationmode::Estimated;
    statuslabel->setText("已进行概算，未进行平差");
}


void Application::Display()
{
    if(applicationmode==Applicationmode::Init||applicationmode==Applicationmode::Inputed||applicationmode==Applicationmode::Estimated||applicationmode==Applicationmode::Adjusted)
    {
        warning("请先进行精度评定！");
        return;
    }
    adj.output_text();
    ui->plainTextEdit_2->setPlainText(adj.get_result());
}

//平差计算
void Application::Adjust()
{
    if(applicationmode==Applicationmode::Init||applicationmode==Applicationmode::Inputed)
    {
        warning("请先进行坐标概算！");
        return;
    }
    if(adj.get_wrong()==1)
    {
        warning("请先检查数据是否正确！");
        return;
    }
    adj.adjust_calc();
    item->Set_drawing_mode(Points);
    item->update();
    applicationmode=Applicationmode::Adjusted;
    statuslabel->setText("已进行平差，未精度评定");
    ui->plainTextEdit_2->clear();
}

//精度评定
void Application::Get_Precision()
{
    if(applicationmode==Applicationmode::Init||applicationmode==Applicationmode::Inputed||applicationmode==Applicationmode::Estimated)
    {
        warning("请先进行平差！");
        return;
    }
    adj.get_precision();
    item->Set_drawing_mode(Precision);
    item->update();
    applicationmode=Applicationmode::Precision_got;
    statuslabel->setText("已完成精度评定，可获得结果");
    ui->plainTextEdit_2->clear();
}


void Application::on_pushButton_6_clicked()
{
    if(applicationmode==Applicationmode::Init)
    {
        warning("请先导入观测文件！");
        return;
    }
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty())
    {
        warning("请输入完整的观测点信息！");
        return;
    }
    if(ui->lineEdit_3->text().isEmpty()&&ui->lineEdit_4->text().isEmpty())
    {
        warning("请输入距离观测值或角度观测值！");
        return;
    }

    if(!ui->lineEdit_3->text().isEmpty())
    {
        bool ok;
        ui->lineEdit_3->text().toDouble(&ok);
        if(!ok)
        {
            warning("角度观测值请输入正确的数据！");
            return;
        }
    }
    if(!ui->lineEdit_4->text().isEmpty())
    {
        bool ok;
        ui->lineEdit_4->text().toDouble(&ok);

        //边长观测值不能小于0
        if(ui->lineEdit_4->text().toDouble()<=0)
            ok=false;

        if(!ok)
        {
            //弹出警告
            warning("距离观测值请输入正确的数据！");
            return;
        }
    }

    if(!ui->lineEdit_3->text().isEmpty())
    {
        //添加角度观测值
        extension_obs+="角度观测,"+
                         ui->lineEdit->text()+','+
                         ui->lineEdit_2->text()+','+
                         ui->lineEdit_3->text()+'\n';
        extension_obs_num++;
    }
    if(!ui->lineEdit_4->text().isEmpty())
    {
        //添加距离观测值
        extension_obs+="距离观测,"+
                         ui->lineEdit->text()+','+
                         ui->lineEdit_2->text()+','+
                         ui->lineEdit_4->text()+'\n';
        extension_obs_num++;
    }
    ui->plainTextEdit_3->setPlainText(extension_obs);
    applicationmode=Applicationmode::Inputed;
    statuslabel->setText("已导入数据，未进行概算");
}

void Application::add_obs_from_text()
{
    if(extension_obs.isEmpty())
    {
        return;
    }
    QStringList textlist=extension_obs.split('\n');
    for(int i=0;i<extension_obs_num;i++)
    {
        QStringList obs_list=textlist[i].split(',');
        adj.add_new_unknown_points(obs_list[1]);
        adj.add_new_unknown_points(obs_list[2]);
        if(obs_list[0]=="角度观测")
        {
            adj.add_new_angle_obs(obs_list[1],
                                  obs_list[2],
                                  obs_list[3].toDouble());
        }
        else if(obs_list[0]=="距离观测")
        {
            adj.add_new_line_obs(obs_list[1],
                                 obs_list[2],
                                 obs_list[3].toDouble());
        }
    }
}


void Application::on_pushButton_8_clicked()
{
    if(extension_obs.isEmpty())
    {
        return;
    }
    ui->plainTextEdit_3->clear();
    extension_obs="";
    extension_obs_num=0;

    //重新从文本数据初始化，清除掉添加过的数据
    adj.init();
    item->Set_drawing_mode(Init);
    item->update();
    ui->plainTextEdit_2->clear();
    adj.get_data(filecontext);
    applicationmode=Applicationmode::Inputed;
    statuslabel->setText("已导入数据，未进行概算");
}


void Application::FileOutput()
{
    if(applicationmode!=Applicationmode::Precision_got)
    {
        warning("请先进行精度评定！");
        return;
    }
    if(!ui->checkBox->isChecked()&&!ui->checkBox_2->isChecked())
    {
        warning("请至少选择一种导出方式！");
        return;
    }
    QString folder=getsavefolder();
    if(folder.isEmpty())
    {
        warning("请选择要保存的路径！");
        return;
    }
    folder+="\\";
    if(ui->checkBox->isChecked())
    {
        if(ui->lineEdit_5->text().isEmpty())
        {
            warning("请输入要保存的文本文件名称！");
            return;
        }
        //导出txt文件
        savefile(adj.get_result(),folder+ui->lineEdit_5->text()+".txt");
    }
    if(ui->checkBox_2->isChecked())
    {
        if(ui->lineEdit_6->text().isEmpty())
        {
            warning("请输入要保存的图像文件名称！");
            return;
        }
        //导出png文件
        QRectF rect=item->boundingRect();
        QImage image(rect.size().toSize(),QImage::Format_ARGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        item->scene()->render(&painter, QRectF(), rect);
        image.save(folder+ui->lineEdit_6->text()+".png");
        ui->graphicsView->setScene(item->scene());
    }
}

//封装了坐标概算和坐标平差的计算函数
void Application::calculation()
{
    Estimation();
    if(adj.get_wrong()==0&&applicationmode==Applicationmode::Estimated)
        Adjust();
}

//弹出帮助页面对话框
void Application::get_helping()
{
    //只有对话框未弹出时，才构建并显示新的对话框
    if(!helping_dialog)
    {
        helping_dialog=new Helping(this);
        helping_dialog->setWindowTitle("应用程序使用帮助");
        helping_dialog->setAttribute(Qt::WA_DeleteOnClose);
        helping_dialog->setWindowFlags(Qt::Window|Qt::WindowTitleHint|Qt::WindowCloseButtonHint);
    }

    //显示对话框
    helping_dialog->show();
}
