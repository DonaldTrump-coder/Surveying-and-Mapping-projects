#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

#include <QString>
#include "matrix.h"

//边长观测值结构
struct line_obs
{
    QString start;
    //观测值起始点名

    QString end;
    //观测值终点名

    double length=0;
    //观测距离
};

//角度观测值结构，表示最原始的方位观测值
struct ang_obs_data
{
    QString start;
    //观测值起始点名

    QString end;
    //观测值终点名

    double angle=0;
    //观测角度值（度分秒）
};

//角度观测值结构，经过稍微处理，包含了
//指向对应观测边的指针以便更好索引
struct ang_obs
{
    line_obs* start=nullptr;
    //起始边（基准边）

    line_obs* end=nullptr;
    //终边

    double angle=0;
    //观测角度值（弧度）
};

//点坐标结构（只在初始化的时候用到）
struct point
{
    QString name;
    //点名

    double x=0;
    double y=0;
    //点坐标
};

//概算用的边结构
//实际用的时候只会与观测边相同，因为只有有观测长度的边才有概算中的意义
struct line_coarse
{
    QString start;
    //起点点名

    QString end;
    //终点点名

    double orientation=0;
    //边的方位角概算值（控制边为已知值）

    double length=0;
    //边长概算值（控制边为已知值）

    bool known=0;
    //边是否已知，0为未知
};

//概算用的点结构
//已知点才能用来做概算，所有点都变为已知点后就能结束概算
struct coarse_point
{
    QString name;
    //点名

    double x=0;
    double y=0;
    //点坐标

    bool known=0;
    //点是否为已知
};

//在类中完成平差的各部分内容
class adjustment
{
private:
    //控制点个数、控制点数组指针
    int control_point_num=0;
    point* control_points=nullptr;

    //待求点个数、待求点数组指针
    int unknown_point_num=0;
    point* unknown_points=nullptr;

    //参与概算的所有点的个数、数组指针
    int coarse_point_num=0;
    coarse_point* coarsepoints=nullptr;

    //边长观测值个数、指针
    int line_obs_num=0;
    line_obs* line_observations=nullptr;
    line_coarse* coarselines=nullptr;

    //角度观测值、指针
    //其中包含了方向观测值个数和包含两个边的角度个数
    int angle_obs_num=0;
    int angle_num=0;
    ang_obs_data* angle_observations=nullptr;
    ang_obs* post_angle_observations=nullptr;

    //用来索引两个参与概算的控制点
    int control_point_index1;
    int control_point_index2;

    int base_num=0;//测站数

    Matrix deltaX;//参数改正数
    Matrix X_init;//参数初值
    Matrix L;//常数项矩阵
    Matrix A;//系数矩阵
    Matrix v;//观测值改正数矩阵
    Matrix P;//权矩阵

    double m0;//单位权中误差
    Matrix Qx;//参数改正数协因数矩阵
    Matrix Qf;//观测值改正数协因数矩阵
    Matrix Mf;//观测值改正数的协方差矩阵
    Matrix Mx;//待定点位x方向误差向量（每一个待定点）
    Matrix My;//待定点位y方向误差向量（每一个待定点）
    Matrix Mk;//待定点位误差向量

    Matrix Q;//长半径方位角
    Matrix E_long;//长半径
    Matrix E_short;//短半径

    QString result="";//输出文本

    bool b_wrong=0;//若程序数据有问题，则该变量控制不可进行平差和精度评定
    bool b_control_points_wrong=0;//若找不到合适的控制边，则不可进行概算和后续
    bool b_data_wrong=0;//若导入的文本数据有问题，则不可进行概算

public:
    //类构造和析构函数
    adjustment();
    ~adjustment();

    //类初始化函数，用于恢复初始状态时删除指针值
    void init();

    //从观测数据中解译点、观测边角等信息
    void get_data(QString text);

    //对观测角进行整理
    void get_ang_obs();

    //根据指定端点得到边的指针
    line_obs* get_line(QString start,QString end);
    line_coarse* get_coarse_line(QString start,QString end);

    //坐标概算
    void get_coarse();

    //根据给定点坐标计算方位角和距离
    double get_orientation(double x1,double y1,double x2,double y2);
    double get_length(double x1,double y1,double x2,double y2);

    //将指定边中的各端点都置为已知点、将边置为已知边
    void set_points_known(line_coarse* line);

    //根据点号搜索概算点的指针或索引
    coarse_point* get_coarse_point_with_name(QString name);
    int get_coarse_point_index(QString name);

    //建立用于平差计算的各矩阵
    void construct_matrics();

    //在概算完成的边中，给定端点名，搜索并计算对应的边方位角和长度
    double get_coarse_orientation_with_names(QString start,QString end);
    double get_coarse_length_with_names(QString start,QString end);

    //判断给定点号是不是控制点
    bool is_control_point(QString name);

    //平差计算
    void adjust_calc();

    //更新参数值（待求点坐标）
    void update();

    //精度评定
    void get_precision();

    //构建输出文本
    void output_text();

    //提供接口，返回输出文本
    QString get_result();

    //提供接口，返回所有概算点
    coarse_point* get_coarse_points();

    //提供接口返回点数、观测数据数，便于绘图
    int get_coarse_points_num();

    int get_control_points_num();

    int get_unknown_points_num();

    int get_line_obs_num();

    //返回观测边数据、角数据
    line_obs* get_line_observations();

    int get_angle_obs_num();

    ang_obs_data* get_ang_obs_data();

    Matrix* get_Q();

    Matrix* get_E_long();

    Matrix* get_E_short();

    //给定点名，添加新待求点
    void add_new_unknown_points(QString name);

    //给定边角观测数据，添加新观测数据
    void add_new_line_obs(QString start,QString end,double length);
    void add_new_angle_obs(QString start,QString end,double angle);

    //返回是否出现相关错误
    bool get_control_points_wrong();

    bool get_wrong();

    bool get_data_wrong();
};

#endif // ADJUSTMENT_H
