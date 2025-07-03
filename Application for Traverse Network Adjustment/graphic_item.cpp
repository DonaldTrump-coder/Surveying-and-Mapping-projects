#include "graphic_item.h"
#include "tools.h"

graphic_item::graphic_item()
{

}

graphic_item::~graphic_item()
{
    //delete adj;
}

void graphic_item::set_data(adjustment* Adj)
{
    adj=Adj;
}

void graphic_item::get_size(QRectF window_size)
{
    size=window_size;
}

QRectF graphic_item::boundingRect() const
{
    return size;
}

void graphic_item::Set_drawing_mode(drawmode draw_mode)
{
    mode=draw_mode;
}

void graphic_item::draw_control_point(QPainter *painter,double x,double y,QString name)
{
    //将实际点位和绘图点位进行转换，以实际点位的中心和绘图区域的中心为基准
    //进行坐标的平移和缩放
    double x_to_draw=(y-center_y)*y_scale+size.width()/2;
    double y_to_draw=-(x-center_x)*x_scale+size.height()/2;

    //控制点需要画三角形，因此用三个点坐标来画
    double x1=0,x2=0,x3=0;
    double y1=0,y2=0,y3=0;
    x1=x_to_draw+4;
    y1=y_to_draw+4;
    x2=x_to_draw;
    y2=y_to_draw-4;
    x3=x_to_draw-4;
    y3=y_to_draw+4;
    painter->drawLine(x1,y1,x2,y2);
    painter->drawLine(x2,y2,x3,y3);
    painter->drawLine(x3,y3,x1,y1);

    //画坐标点号
    painter->drawText(x_to_draw+4,y_to_draw-4,name);
}

void graphic_item::draw_unknown_point(QPainter *painter,double x,double y,QString name)
{
    double x_to_draw=(y-center_y)*y_scale+size.width()/2;
    double y_to_draw=-(x-center_x)*x_scale+size.height()/2;
    painter->drawEllipse(QPointF(x_to_draw,y_to_draw),2,2);
    painter->drawText(x_to_draw+4,y_to_draw-4,name);
}

void graphic_item::draw_control_line(QPainter *painter,double x1,double y1,double x2,double y2)
{
    double x1_to_draw=(y1-center_y)*y_scale+size.width()/2;
    double y1_to_draw=-(x1-center_x)*x_scale+size.height()/2;
    double x2_to_draw=(y2-center_y)*y_scale+size.width()/2;
    double y2_to_draw=-(x2-center_x)*x_scale+size.height()/2;
    painter->drawLine(QPointF(x1_to_draw,y1_to_draw-2),QPointF(x2_to_draw,y2_to_draw-2));
    painter->drawLine(QPointF(x1_to_draw,y1_to_draw+2),QPointF(x2_to_draw,y2_to_draw+2));
}

void graphic_item::draw_line(QPainter *painter,double x1,double y1,double x2,double y2)
{
    double x1_to_draw=(y1-center_y)*y_scale+size.width()/2;
    double y1_to_draw=-(x1-center_x)*x_scale+size.height()/2;
    double x2_to_draw=(y2-center_y)*y_scale+size.width()/2;
    double y2_to_draw=-(x2-center_x)*x_scale+size.height()/2;
    painter->drawLine(QPointF(x1_to_draw,y1_to_draw),QPointF(x2_to_draw,y2_to_draw));
}

void graphic_item::draw_ellipse(QPainter *painter,double x,double y,double angle,double long_r,double short_r)
{
    double x_to_draw=(y-center_y)*y_scale+size.width()/2;
    double y_to_draw=-(x-center_x)*x_scale+size.height()/2;
    QPointF center(x_to_draw,y_to_draw);
    double angle_degree=angle*180/pi;
    painter->save();
    painter->translate(center);
    painter->rotate(angle_degree);
    painter->drawEllipse(QPointF(0,0),short_r,long_r);//方位角是以纵轴为基准旋转，因此绘制椭圆时长短轴互换方便编写程序
    painter->drawLine(-short_r,0,short_r,0);
    painter->drawLine(0,-long_r,0,long_r);
    painter->restore();
}

void graphic_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(mode==Init)
    {
        //painter->setBrush(Qt::white); //设置画刷颜色
        //painter->drawRect(size);

        //测试用
        //QPen pen(Qt::black,2,Qt::SolidLine);
        //painter->setPen(pen);
        //painter->drawLine(size.topLeft(),size.bottomRight());
    }
    else
    {
        QPen pen(Qt::black,1,Qt::SolidLine);
        painter->setPen(pen);
        QFont font("Consolas",12);
        painter->setFont(font);

        coarse_point* points=adj->get_coarse_points();

        double dx=0;
        double dy=0;
        double xmin=0,ymin=0,xmax=0,ymax=0;
        xmin=xmax=points[0].x;
        ymin=ymax=points[0].y;
        for(int i=0;i<adj->get_coarse_points_num();i++)
        {
            if(points[i].x>xmax)
                xmax=points[i].x;
            if(points[i].x<xmin)
                xmin=points[i].x;
            if(points[i].y>ymax)
                ymax=points[i].y;
            if(points[i].y<ymin)
                ymin=points[i].y;
        }
        dx=xmax-xmin;
        dy=ymax-ymin;
        center_x=0.5*(xmax+xmin);
        center_y=0.5*(ymax+ymin);
        x_scale=size.height()*0.9/dx;
        y_scale=size.width()*0.9/dy;

        for(int i=0;i<adj->get_control_points_num();i++)
        {
            draw_control_point(painter,points[i].x,points[i].y,points[i].name);
        }
        for(int i=0;i<adj->get_unknown_points_num();i++)
        {
            draw_unknown_point(painter,points[i+adj->get_control_points_num()].x,points[i+adj->get_control_points_num()].y,points[i+adj->get_control_points_num()].name);
        }

        for(int i=0;i<adj->get_line_obs_num()+1;i++)
        {
            double x1=0,y1=0,x2=0,y2=0;
            QString start=adj->get_line_observations()[i].start;
            QString end=adj->get_line_observations()[i].end;
            bool arecontrolpoints=1;
            for(int j=0;j<adj->get_coarse_points_num();j++)
            {
                if(points[j].name==start)
                {
                    x1=points[j].x;
                    y1=points[j].y;
                    if(j>=adj->get_control_points_num())
                        arecontrolpoints=0;
                }
                if(points[j].name==end)
                {
                    x2=points[j].x;
                    y2=points[j].y;
                    if(j>=adj->get_control_points_num())
                        arecontrolpoints=0;
                }
            }
            if(arecontrolpoints==1)
            {
                //绘制控制边
                draw_control_line(painter,x1,y1,x2,y2);
            }
            else
            {
                //绘制观测边
                draw_line(painter,x1,y1,x2,y2);
            }
        }

        //补充观测角但未观测边的图形
        for(int i=0;i<adj->get_angle_obs_num();i++)//遍历每一个角度观测
        {
            QString start=adj->get_ang_obs_data()[i].start;
            QString end=adj->get_ang_obs_data()[i].end;
            bool matched=0;
            for(int j=0;j<adj->get_line_obs_num()+1;j++)
            {
                if(adj->get_line_observations()[j].start==start&&adj->get_line_observations()[j].end==end)
                {
                    matched=1;
                    break;
                }
                if(adj->get_line_observations()[j].start==end&&adj->get_line_observations()[j].end==start)
                {
                    matched=1;
                    break;
                }
            }
            if(matched==1)
                continue;
            double x1=0,y1=0,x2=0,y2=0;
            for(int j=0;j<adj->get_coarse_points_num();j++)
            {
                if(points[j].name==start)
                {
                    x1=points[j].x;
                    y1=points[j].y;
                }
                if(points[j].name==end)
                {
                    x2=points[j].x;
                    y2=points[j].y;
                }
            }
            draw_line(painter,x1,y1,x2,y2);
        }

        if(mode==Precision)
        {
            double x=0,y=0;
            double angle=0,long_r=0,short_r=0;
            for(int i=0;i<adj->get_unknown_points_num();i++)
            {
                x=adj->get_coarse_points()[i+2].x;
                y=adj->get_coarse_points()[i+2].y;
                angle=adj->get_Q()->getMatrix_ele(i,0);
                long_r=adj->get_E_long()->getMatrix_ele(i,0)*3;
                short_r=adj->get_E_short()->getMatrix_ele(i,0)*3;
                draw_ellipse(painter,x,y,angle,long_r,short_r);
            }
        }
    }
}
