#include "adjustment.h"
#include "matrix.h"
#include <QDebug>
#include "tools.h"

//定义变量rou，用于从弧度转为秒
#define rou 206265

void adjustment::construct_matrics()
{
    //Matrix dX(base_num+2*unknown_point_num,1);
    Matrix X0(base_num+2*unknown_point_num,1);
    Matrix l(line_obs_num+angle_obs_num,1);
    Matrix B(line_obs_num+angle_obs_num,base_num+2*unknown_point_num);
    Matrix V(line_obs_num+angle_obs_num,1);

    //设置坐标参数初值
    for(int i=0;i<unknown_point_num;i++)
    {
        X0.SetMatrix_ele(i+base_num,0,coarsepoints[i+2].x);
        X0.SetMatrix_ele(i+base_num+unknown_point_num,0,coarsepoints[i+2].y);
    }

    int base_index=0;
    int point_index;
    double Z0;
    double T0;
    double D0;
    double a;
    double b;

    //设置测站点名
    QString base=angle_observations[0].start;

    //获得首个测站基准边的观测角
    Z0=get_coarse_orientation_with_names(angle_observations[0].start,angle_observations[0].end);
    //遍历所有的方向观测值
    for(int i=0;i<angle_obs_num;i++)
    {
        //测站变更
        if(angle_observations[i].start!=base)
        {
            //基准边方位角在参数中靠base_index索引
            base_index++;
            base=angle_observations[i].start;

            //当观测站变化时，就更新基准方向对应的方位角
            Z0=get_coarse_orientation_with_names(angle_observations[i].start,angle_observations[i].end);
        }
        X0.SetMatrix_ele(base_index,0,Z0);

        //为系数矩阵赋值-1
        B.SetMatrix_ele(i,base_index,-1);

        //方向观测值对应的T0
        T0=get_coarse_orientation_with_names(angle_observations[i].start,angle_observations[i].end);

        //为常数项矩阵赋值
        l.SetMatrix_ele(i,0,sec_abs(-(T0-Z0-dms2rad(angle_observations[i].angle))*rou));

        //构建参数改正数坐标部分的系数
        D0=get_coarse_length_with_names(angle_observations[i].start,angle_observations[i].end)*1000;
        a=rou*sin(T0)/D0;
        b=-rou*cos(T0)/D0;

        //第i个观测值中是否包含控制点，只为不是控制点的坐标改正数添加系数
        if(!is_control_point(angle_observations[i].start))
        {
            point_index=get_coarse_point_index(angle_observations[i].start);
            point_index+=base_num;
            B.SetMatrix_ele(i,point_index,a);
            point_index+=unknown_point_num;
            B.SetMatrix_ele(i,point_index,b);
        }
        if(!is_control_point(angle_observations[i].end))
        {
            point_index=get_coarse_point_index(angle_observations[i].end);
            point_index+=base_num;
            B.SetMatrix_ele(i,point_index,-a);
            point_index+=unknown_point_num;
            B.SetMatrix_ele(i,point_index,-b);
        }
    }

    double S;
    double S0;
    //为了方便搜索观测数据前面加了一行，实际的观测数据从第二个开始
    //遍历所有的边观测值
    for(int i=1;i<line_obs_num+1;i++)
    {
        //边的观测值
        S=line_observations[i].length;

        //边长的概算值
        S0=get_coarse_length_with_names(line_observations[i].start,line_observations[i].end);

        //边长观测方程的常数项
        l.SetMatrix_ele(i+angle_obs_num-1,0,-(S0-S)*1000);

        //边长的方位角概算值
        T0=get_coarse_orientation_with_names(line_observations[i].start,line_observations[i].end);

        //给非控制点坐标参数添加系数
        if(!is_control_point(line_observations[i].start))
        {
            //搜索起始点的序号
            point_index=get_coarse_point_index(line_observations[i].start);

            //系数添加的x坐标位置
            point_index+=base_num;
            B.SetMatrix_ele(i+angle_obs_num-1,point_index,-cos(T0));

            //系数添加的y坐标位置
            point_index+=unknown_point_num;
            B.SetMatrix_ele(i+angle_obs_num-1,point_index,-sin(T0));
        }
        if(!is_control_point(line_observations[i].end))
        {
            point_index=get_coarse_point_index(line_observations[i].end);
            point_index+=base_num;
            B.SetMatrix_ele(i+angle_obs_num-1,point_index,cos(T0));
            point_index+=unknown_point_num;
            B.SetMatrix_ele(i+angle_obs_num-1,point_index,sin(T0));
        }
    }
    X_init=X0;
    L=l;
    A=B;
    v=V;
}
