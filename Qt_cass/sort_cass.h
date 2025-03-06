#ifndef SORT_CASS_H
#define SORT_CASS_H
#include <QString>

//定义数据点类，存储一个点的点号、编码、xyh坐标信息
struct point
{
    //在后续读入文件信息的时候会有重复的点
    //会为其分配存储空间但不存储其信息，
    //因此需要为点数据设置一个初始值
    int num=-1;
    QString code="";
    double x=0,y=0,h=0;
};

//将处理数据的过程封装为一个类
class sort_cass
{
private:
    int total_num=0;//存储文件中所有的点数（包含重复点）
    int total_eff=0;//存储文件中不重复的点数
    point* points=NULL;//用于指向存储所有点的点数组
    bool wrong=0;//用于判断输入数据是否符合规范的标志变量
public:
    sort_cass(QString text);
    //用于读入数据、初始化数据的构造函数

    void sort();//用于对内存中的所有点数据排序
    QString output();//用于返回输出的数据内容
    bool test();//用于检测并返回输入数据是否有错误

    ~sort_cass();
    //需要在析构函数中释放动态内存
};

#endif // SORT_CASS_H
