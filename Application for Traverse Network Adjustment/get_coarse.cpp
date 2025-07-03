#include "adjustment.h"
#include "tools.h"
#include <QDebug>

void adjustment::get_coarse()
{
    delete[] coarsepoints;
    delete[] coarselines;
    b_wrong=0;
    coarsepoints=new coarse_point[control_point_num+unknown_point_num];
    coarse_point_num=control_point_num+unknown_point_num;
    coarselines=new line_coarse[line_obs_num+1];
    int pointsindex=0;
    for(int i=0;i<control_point_num;i++)
    {
        coarsepoints[pointsindex].name=control_points[i].name;
        coarsepoints[pointsindex].known=1;
        coarsepoints[pointsindex].x=control_points[i].x;
        coarsepoints[pointsindex].y=control_points[i].y;
        pointsindex++;
    }

    for(int i=0;i<unknown_point_num;i++)
    {
        coarsepoints[pointsindex].name=unknown_points[i].name;
        coarsepoints[pointsindex].known=0;
        pointsindex++;
    }

    coarselines[0].start=control_points[control_point_index1].name;
    coarselines[0].end=control_points[control_point_index2].name;
    coarselines[0].known=1;
    coarselines[0].orientation=get_orientation(control_points[control_point_index1].x,
                                               control_points[control_point_index1].y,
                                               control_points[control_point_index2].x,
                                               control_points[control_point_index2].y);
    coarselines[0].length=get_length(control_points[control_point_index1].x,
                                     control_points[control_point_index1].y,
                                     control_points[control_point_index2].x,
                                     control_points[control_point_index2].y);
    for(int i=1;i<line_obs_num+1;i++)
    {
        coarselines[i].start=line_observations[i].start;
        coarselines[i].end=line_observations[i].end;
        coarselines[i].known=0;
        coarselines[i].length=line_observations[i].length;
        coarselines[i].orientation=0;
    }

    int unknown_points_num=-1;
    int i_times=0;
    bool coarse_complete=0;
    int point_index=0;//选定一个待求点
    while(1)
    {
        i_times++;
        bool point_finished=0;
        coarse_complete=1;
        for(int i=0;i<coarse_point_num;i++)
        {
            if(coarsepoints[i].known==0)
                coarse_complete=0;
        }
        if(coarse_complete)
            break;

        //数据出错时终止循环概算
        int unknown_points_temp=0;
        for(int i=0;i<coarse_point_num;i++)
        {
            if(coarsepoints[i].known==0)
                unknown_points_temp++;
        }
        if(unknown_points_temp==unknown_points_num&&i_times>coarse_point_num*coarse_point_num)
        {
            b_wrong=1;
            warning("请检查数据是否正确！");
            return;
        }
        unknown_points_num=unknown_points_temp;

        if(point_index>=coarse_point_num)//循环挑选待求点
            point_index=0;

        if(coarsepoints[point_index].known)//若此时的点不是待求点，则跳转至下一个点
        {
            point_index++;
            continue;
        }

        for(int k=0;k<line_obs_num+1;k++)
        {
            if(coarselines[k].known==1)
                continue;

            bool startknown=0;
            bool endknown=0;
            QString start=coarselines[k].start;
            QString end=coarselines[k].end;
            double x1=0,y1=0,x2=0,y2=0;
            for(int j=0;j<coarse_point_num;j++)
            {
                if(coarsepoints[j].name==start&&coarsepoints[j].known==1)
                {
                    x1=coarsepoints[j].x;
                    y1=coarsepoints[j].y;
                    startknown=1;
                }
                if(coarsepoints[j].name==end&&coarsepoints[j].known==1)
                {
                    x2=coarsepoints[j].x;
                    y2=coarsepoints[j].y;
                    endknown=1;
                }
            }
            if(startknown==1&&endknown==1)
            {
                coarselines[k].orientation=get_orientation(x1,y1,x2,y2);
                coarselines[k].known=1;
                continue;
            }
        }

        //对于待求点，开始挑选另一点组成待求边
        for(int i=0;i<coarse_point_num;i++)
        {
            if(point_finished==1)
                break;
            if(!coarsepoints[i].known)
                continue;
            if(get_line(coarsepoints[point_index].name,coarsepoints[i].name)==nullptr)
                continue;
            if(get_coarse_line(coarsepoints[point_index].name,coarsepoints[i].name)==nullptr)
                continue;

            //组成待求边，获得该边的指针
            line_obs* line=get_line(coarsepoints[point_index].name,coarsepoints[i].name);//待求边
            line_coarse* coarse_line=get_coarse_line(coarsepoints[point_index].name,coarsepoints[i].name);

            bool found=0;
            //遍历所有的角，找出包含该边的角
            for(int k=0;k<angle_num;k++)
            {
                if(found==1)
                    break;
                if(post_angle_observations[k].start==nullptr||post_angle_observations[k].end==nullptr)
                    continue;
                if(((post_angle_observations[k].start->start==line->start)&&(post_angle_observations[k].start->end==line->end))||((post_angle_observations[k].start->end==line->start)&&(post_angle_observations[k].start->start==line->end)))
                {
                    QString start=post_angle_observations[k].end->start;
                    QString end=post_angle_observations[k].end->end;
                    for(int j=0;j<line_obs_num+1;j++)
                    {
                        if((coarselines[j].start==start&&coarselines[j].end==end)||(coarselines[j].end==start&&coarselines[j].start==end))
                        {
                            if(coarselines[j].known==1)
                            {
                                //角度的终边已知，计算始边的方位角
                                if(coarselines[j].start==coarse_line->end||coarselines[j].end==coarse_line->start)
                                {
                                    coarse_line->orientation=rad_abs(coarselines[j].orientation-post_angle_observations[k].angle+pi);
                                    coarse_line->known=1;
                                    k=angle_num;
                                    set_points_known(coarse_line);
                                    found=1;
                                    point_finished=1;
                                    break;
                                }
                                else if(coarselines[j].start==coarse_line->start||coarselines[j].end==coarse_line->end)
                                {
                                    coarse_line->orientation=rad_abs(coarselines[j].orientation-post_angle_observations[k].angle);
                                    coarse_line->known=1;
                                    k=angle_num;
                                    set_points_known(coarse_line);
                                    found=1;
                                    point_finished=1;
                                    break;
                                }
                            }
                        }
                    }
                }
                else if((post_angle_observations[k].end->start==line->start)&&(post_angle_observations[k].end->end==line->end)||(post_angle_observations[k].end->end==line->start)&&(post_angle_observations[k].end->start==line->end))
                {
                    QString start=post_angle_observations[k].start->start;
                    QString end=post_angle_observations[k].start->end;
                    for(int j=0;j<line_obs_num+1;j++)
                    {
                        if((coarselines[j].start==start&&coarselines[j].end==end)||(coarselines[j].end==start&&coarselines[j].start==end))
                        {
                            if(coarselines[j].known==1)
                            {
                                //角度的始边已知，计算终边的方位角
                                if(coarselines[j].start==coarse_line->end||coarselines[j].end==coarse_line->start)
                                {
                                    coarse_line->orientation=rad_abs(coarselines[j].orientation+post_angle_observations[k].angle+pi);
                                    coarse_line->known=1;
                                    k=angle_num;
                                    set_points_known(coarse_line);
                                    found=1;
                                    point_finished=1;
                                    break;
                                }
                                else if(coarselines[j].start==coarse_line->start||coarselines[j].end==coarse_line->end)
                                {
                                    coarse_line->orientation=rad_abs(coarselines[j].orientation+post_angle_observations[k].angle);
                                    coarse_line->known=1;
                                    k=angle_num;
                                    set_points_known(coarse_line);
                                    found=1;
                                    point_finished=1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        point_index++;
    }
}
