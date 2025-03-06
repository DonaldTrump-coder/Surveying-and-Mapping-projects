#ifndef CALCULATION_H
#define CALCULATION_H
#include <QString>

//定义文件的枚举类型，用于读写文件操作函数中
enum file_opera
{
    read,write
};

//通过一个函数处理文件读、写的功能
QString file_operation(file_opera,QString="");

//定义点结构体类型，其中的观测点还包含了观测角度的结构体类型
struct point
{
    double x,y;//点坐标
    QString code;//点号

    //观测角度的结构体类型
    //其中包括了左角、右角的枚举类型
    struct angle
    {
        double val;//观测角度值（以弧度为单位）

        enum angle_feature
        {
            left,right
        } feature;

    } point_angle;
};

//定义观测边结构体类型
struct line
{
    double length;//观测长度
    double direction;//方位角
};

//定义初始化数据并提供计算功能的类
class calculation
{
public:
    calculation(QString text);//从文本数据中读取数据元素
    ~calculation();//析构函数，释放动态内存
    void estimation();//进行坐标及方位角推算的函数
    int correction(double limit);//进行闭合差计算、坐标与方位角改正的函数
    QString output(bool wrong);//进行文本结果输出的函数
    bool test();//返回输入格式是否错误的函数
private:
    int line_num,angle_num;//观测边、观测角个数
    point* points;//存储点类型的数组
    line* lines;//存储观测边类型的数组
    double theta,T,R;//存储改正量和闭合差的变量
    bool wrong;//存储输入格式是否有误的变量
};

#endif // CALCULATION_H
