#ifndef QUAD_H
#define QUAD_H

//达成实验目的所需功能最基本的类，作为基类
//可用于计算并返回单个大地四边形的参数
class quad
{
public:
    quad(double XA=0,double YA=0,
         double XB=0,double YB=0,
         double A1=0,double A2=0,
         double A3=0,double A4=0,
         double A5=0,double A6=0,
         double A7=0,double A8=0);
    //大地四边形类的构造函数，用于传入已知参数，其中所有参数的缺省值均设为0

    void calculate();//用于计算大地四边形未知参数的计算函数，实现主要功能

    //传回大地四边形对象中的目标参数，用于输出
    double showxc();
    double showxd();
    double showyc();
    double showyd();
    double* showw();//输出闭合差数组的函数

protected://后续对基类进行继承为链表结点时还需要访问这些参数，
          //因此将对象成员设置为protected类型，增强安全性和封装性
    double xa,xb,ya,yb;//已知点的坐标
    double a1,a2,a3,a4,a5,a6,a7,a8;//已知角度值
    double xc,xd,yc,yd;//目标点的坐标
    double w[4];//待求的闭合差，用数组表示比较方便
                //其中wd为w[0]，w1,w2,w3分别为w[1],w[2],w[3]
};


//拓展程序功能：实现多个大地四边形的存储和计算
//为节省存储空间，多个大地四边形需要动态存储，每需要存储一个四边形时再申请一个四边形的存储空间
//动态存储考虑采用链表的结构
//计算功能已定义好，因此采用继承的方式，将单个大地四边形的类派生为大地四边形链表结点
class quadNode:public quad
{
private:
    quadNode* p=nullptr;//指针域，链表节点的数据成员增加一个指向下一个节点的指针
public:
    using quad::quad;//继承基类的构造函数，传参方式不变
    void setnext(quadNode* add);//用于设置指针指向的下一个结点地址
    quadNode* callnext();//用于引用当前结点指针指向的下一个结点

    //返回已知参数，用于在计算存储的大地四边形时恢复已知值
    double showxa();
    double showya();
    double showxb();
    double showyb();
    double showa1();
    double showa2();
    double showa3();
    double showa4();
    double showa5();
    double showa6();
    double showa7();
    double showa8();
};

//定义大地四边形链表类
class quadList
{
private:
    quadNode* head;//头指针，指向的结点不用于存储数据
    quadNode* last;//尾指针，初始化时与头指针指向的位置相同
    int Num;//链表存储的大地四边形个数
public:
    quadList();//链表的构造函数
    void append(double XA,double YA,
                double XB,double YB,
                double A1,double A2,
                double A3,double A4,
                double A5,double A6,
                double A7,double A8);//用于在链表末尾添加新结点
    quadNode* find(int n);//通过整数索引找到指定的链表结点
    int showNum();//用于返回链表存储的大地四边形个数
    ~quadList();//链表析构函数
};

#endif // QUAD_H
