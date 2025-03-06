#ifndef INPUT_H
#define INPUT_H
#include <QString>
#include "func.h"

//建立用于存储点坐标的点结构体
struct point
{
    double x,y,z;
};

//建立用于存储三角形顶点点号的三角形结构体
struct triangle
{
    int a,b,c;
};

//定义一个用于存储和处理数据的类
class input
{
public:
    QString getfile();//便于用户选择文件、读文件的函数
    void init(QString text);//将输入文本进行解译处理的函数
    point* getpoints();//用于外界访问存储所有点的数组
    triangle* gettriangles();//用于外界访问存储所有三角形的数组
    void destroy();//用于销毁所有动态数组的函数
    point findcenter();//用于返回所有点的中心位置坐标的函数
    point findspan();//返回所有点xyz坐标的范围
    int gettrinum();//返回三角形个数的函数
    bool test();//返回是否有输入格式错误的函数
    ~input();//类的析构函数
private:
    int point_num=0;//总点数
    int tri_num=0;//总三角形个数
    point* points=nullptr;//存储所有点信息的数组指针
    triangle* triangles=nullptr;//存储所有三角形信息的数组指针
    bool wrong;//判断是否有输入格式错误的变量
};

#endif // INPUT_H
