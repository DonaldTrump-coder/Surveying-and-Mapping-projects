#include "canvas.h"
#include "ui_canvas.h"
#include <QPainter>
#include "draw_dlg.h"

canvas::canvas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::canvas)
{
    ui->setupUi(this);

    //设置白色背景
    setPalette(QPalette(Qt::white));

    //自动填充背景
    setAutoFillBackground(true);
}

canvas::~canvas()
{
    delete ui;
}

//设置绘图流程
void canvas::paintEvent(QPaintEvent* event)
{
    //定义QPainter对象
    QPainter painter(this);

    //获取当前窗口的长和宽，使用相对坐标进行绘图
    double W=this->width();
    double H=this->height();

    //设置绘图范围，使绘图区域不太靠近对话框的边界
    double xmax=W-10;
    double ymax=H-10;
    double xmin=30;
    double ymin=30;

    //根据屏幕范围和实际数据点的坐标范围，计算
    //笛卡尔坐标系中单位长度对应的屏幕坐标长度
    //即缩放参数
    double dx=(xmax-xmin)/a.findspan().x;
    double dy=(ymax-ymin)/a.findspan().y;

    //使用一个点对象表示屏幕上中心点对应的屏幕坐标
    point screen_center;
    screen_center.x=(xmax+xmin)/2;
    screen_center.y=(ymax+ymin)/2;

    //使用一个点对象表示笛卡尔坐标系原点对应的屏幕坐标
    point screen_origin;
    //将屏幕中心与数据点中心重合，从而通过
    //缩放参数dx，dy计算出原点对应的屏幕坐标
    screen_origin.x=screen_center.x-dx*a.findcenter().x;
    screen_origin.y=screen_center.y+dy*a.findcenter().y;

    //依据原点的屏幕坐标画出两条正交的坐标轴
    painter.drawLine(QPointF(xmin,screen_origin.y),QPointF(xmax,screen_origin.y));
    painter.drawLine(QPointF(screen_origin.x,ymax),QPointF(screen_origin.x,ymin));

    //用画线的方式画坐标轴末端的箭头
    double ds=15;//箭头的长度
    double alpha=pi/6;//箭头和坐标轴的夹角
    painter.drawLine(QPointF(xmax,screen_origin.y),QPointF(xmax-ds*cos(alpha),screen_origin.y-ds*sin(alpha)));
    painter.drawLine(QPointF(xmax,screen_origin.y),QPointF(xmax-ds*cos(alpha),screen_origin.y+ds*sin(alpha)));
    painter.drawLine(QPointF(screen_origin.x,ymin),QPointF(screen_origin.x-ds*sin(alpha),ymin+ds*cos(alpha)));
    painter.drawLine(QPointF(screen_origin.x,ymin),QPointF(screen_origin.x+ds*sin(alpha),ymin+ds*cos(alpha)));

    //用画线、画文字的方式画坐标轴刻度线和刻度值
    double scale;//当前的刻度值
    double dX=10,dY=10;//x，y轴每刻度值之间的间隔
    double scale_length=5;//刻度线的像素长度
    //画x轴正半轴部分
    for(scale=0;screen_origin.x+dx*scale<xmax;scale=scale+dX)
    {
        if(scale==0)
            continue;//跳过0刻度
        painter.drawLine(QPointF(screen_origin.x+dx*scale,screen_origin.y),QPointF(screen_origin.x+dx*scale,screen_origin.y+scale_length));

        //画刻度值
        if(scale<100)
            painter.drawText(QPointF(screen_origin.x+dx*scale-7,screen_origin.y+4*scale_length),QString("%1").arg(int(scale)));
        else
            painter.drawText(QPointF(screen_origin.x+dx*scale-9,screen_origin.y+4*scale_length),QString("%1").arg(int(scale)));
        //依据数字的长度不同，对数字进行不同的位置偏移
        //使显示结果更好看些
    }
    //画x轴负半轴部分
    for(scale=0;screen_origin.x-dx*scale>xmin;scale=scale+dX)
    {
        if(scale==0)
            continue;
        painter.drawLine(QPointF(screen_origin.x-dx*scale,screen_origin.y),QPointF(screen_origin.x-dx*scale,screen_origin.y+scale_length));
        if(scale<100)
            painter.drawText(QPointF(screen_origin.x-dx*scale-7,screen_origin.y+4*scale_length),QString("%1").arg(int(-scale)));
        else
            painter.drawText(QPointF(screen_origin.x-dx*scale-9,screen_origin.y+4*scale_length),QString("%1").arg(int(-scale)));
    }
    //画y轴负半轴部分
    for(scale=0;screen_origin.y+dy*scale<ymax;scale=scale+dY)
    {
        if(scale==0)
            continue;
        painter.drawLine(QPointF(screen_origin.x,screen_origin.y+dy*scale),QPointF(screen_origin.x+scale_length,screen_origin.y+dy*scale));
        if(scale<100)
            painter.drawText(QPointF(screen_origin.x-5*scale_length,screen_origin.y+dy*scale+4),QString("%1").arg(int(-scale)));
        else
            painter.drawText(QPointF(screen_origin.x-6*scale_length,screen_origin.y+dy*scale+4),QString("%1").arg(int(-scale)));
    }
    //画y轴正半轴部分
    for(scale=0;screen_origin.y-dy*scale>ymin;scale=scale+dY)
    {
        if(scale==0)
            continue;
        painter.drawLine(QPointF(screen_origin.x,screen_origin.y-dy*scale),QPointF(screen_origin.x+scale_length,screen_origin.y-dy*scale));
        if(scale<100)
            painter.drawText(QPointF(screen_origin.x-5*scale_length,screen_origin.y-dy*scale+4),QString("%1").arg(int(scale)));
        else
            painter.drawText(QPointF(screen_origin.x-6*scale_length,screen_origin.y-dy*scale+4),QString("%1").arg(int(scale)));
    }

    //将三角网用绿线画
    //需要先定义一个QPen对象传给QPainter对象
    QPen pen;
    pen.setWidth(1);//线宽设为1
    pen.setColor(Qt::green);//颜色设为绿色
    painter.setPen(pen);

    //用三个整数变量存储每次读到的三个点号
    int tri1,tri2,tri3;

    //用三个浮点数变量存储每次读到的两个点坐标
    double x1,y1,x2,y2;
    triangle* t=a.gettriangles();
    point* p=a.getpoints();

    //遍历所有的三角形
    for(int i=0;i<a.gettrinum();i++)
    {
        //读取每个三角形的三个顶点号
        tri1=t[i].a;
        tri2=t[i].b;
        tri3=t[i].c;

        //根据点号分三次找出要画的所有线
        //每条线只需要两个点，因此都用x1，y1，x2，y2存储点坐标
        //通过相对坐标的形式找到点对应的屏幕坐标并画线
        x1=screen_origin.x+dx*p[tri1-1].x;
        y1=screen_origin.y-dy*p[tri1-1].y;
        x2=screen_origin.x+dx*p[tri2-1].x;
        y2=screen_origin.y-dy*p[tri2-1].y;
        painter.drawLine(QPointF(x1,y1),QPointF(x2,y2));

        x1=screen_origin.x+dx*p[tri2-1].x;
        y1=screen_origin.y-dy*p[tri2-1].y;
        x2=screen_origin.x+dx*p[tri3-1].x;
        y2=screen_origin.y-dy*p[tri3-1].y;
        painter.drawLine(QPointF(x1,y1),QPointF(x2,y2));

        x1=screen_origin.x+dx*p[tri1-1].x;
        y1=screen_origin.y-dy*p[tri1-1].y;
        x2=screen_origin.x+dx*p[tri3-1].x;
        y2=screen_origin.y-dy*p[tri3-1].y;
        painter.drawLine(QPointF(x1,y1),QPointF(x2,y2));
    }
}
