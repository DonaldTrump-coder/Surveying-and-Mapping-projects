#include "quad.h"
#include "func.h"

//单个大地四边形类的成员函数体
//单个大地四边形类的构造函数，传入已知参数、初始化未知参数
quad::quad(double XA,double YA,
           double XB,double YB,
           double A1,double A2,
           double A3,double A4,
           double A5,double A6,
           double A7,double A8)
{
    xa=XA;
    ya=YA;
    xb=XB;
    yb=YB;//传入已知点坐标

    //将传入的度分秒制角度转为弧度制角度，用于计算
    a1=torad(A1);
    a2=torad(A2);
    a3=torad(A3);
    a4=torad(A4);
    a5=torad(A5);
    a6=torad(A6);
    a7=torad(A7);
    a8=torad(A8);

    //将位置点的坐标、四个闭合差置为默认值0
    xc=yc=xd=yd=0;
    for(int i=0;i<4;i++)
    {
        w[i]=0;
    }
}

//大地四边形类的核心函数，实现了未知参数的计算功能
void quad::calculate()
{
    //前方交会计算公式，用于计算目标点坐标
    xd=(xa*cot(a1+a8)+xb*cot(a2)+yb-ya)/(cot(a2)+cot(a1+a8));
    yd=(ya*cot(a1+a8)+yb*cot(a2)+xa-xb)/(cot(a2)+cot(a1+a8));
    xc=(xa*cot(a1)+xb*cot(a2+a3)+yb-ya)/(cot(a2+a3)+cot(a1));
    yc=(ya*cot(a1)+yb*cot(a2+a3)+xa-xb)/(cot(a2+a3)+cot(a1));

    //闭合差计算公式，用于计算角度闭合差、极条件闭合差

    w[1]=(a5+a6+a7+a8-pi)*rou;//角度闭合差由弧度制转换为以秒为单位
    w[2]=(a1+a2+a3+a4-pi)*rou;
    w[3]=(a1+a2+a7+a8-pi)*rou;
    w[0]=(1-(sin(a1)*sin(a7)*sin(a4+a5))/(sin(a4)*sin(a1+a8)*sin(a6)))*rou;
    //极条件闭合差由弧度制转换为以秒为单位
}

//返回目标值
double quad::showxc()
{
    return xc;
}

double quad::showxd()
{
    return xd;
}

double quad::showyc()
{
    return yc;
}

double quad::showyd()
{
    return yd;
}

double* quad::showw()
{
    return w;
}

//大地四边形结点类的成员函数体
//使当前结点的指针指向目标结点
void quadNode::setnext(quadNode* add)//参数为目标节点的地址
{
    p=add;
}

//返回当前结点指针指向的下一个结点
quadNode* quadNode::callnext()
{
    return p;//返回下一个节点的地址
}

//返回大地四边形结点中的已知参数
double quadNode::showxa()
{
    return xa;
}

double quadNode::showya()
{
    return ya;
}

double quadNode::showxb()
{
    return xb;
}

double quadNode::showyb()
{
    return yb;
}

double quadNode::showa1()
{
    return a1;
}

double quadNode::showa2()
{
    return a2;
}

double quadNode::showa3()
{
    return a3;
}

double quadNode::showa4()
{
    return a4;
}

double quadNode::showa5()
{
    return a5;
}

double quadNode::showa6()
{
    return a6;
}

double quadNode::showa7()
{
    return a7;
}

double quadNode::showa8()
{
    return a8;
}


//大地四边形链表的成员函数体
//大地四边形链表的构造函数，用于建立一个新的、不存储数据的头结点并初始化成员变量
quadList::quadList()
{
    Num=0;//将存储的大地四边形个数设为0
    head=new quadNode();//动态创建一个新大地四边形头结点，头指针指向该结点
    last=head;//初始状态下，尾指针指向头节点
}

//用于在链表当前末尾结点后新增结点，同时传入新结点的已知参数
void quadList::append(double XA,double YA,
                      double XB,double YB,
                      double A1,double A2,
                      double A3,double A4,
                      double A5,double A6,
                      double A7,double A8)
{

    //使用大地四边形结点的成员函数，将新结点加至末尾结点后
    last->setnext(new quadNode(XA,YA,XB,YB,A1,A2,A3,A4,A5,A6,A7,A8));

    //当前存储的大地四边形个数增加1
    Num++;

    //尾指针后移一个
    last=last->callnext();
}

//链表类的析构函数中需要销毁所有的动态内存
quadList::~quadList()
{
    //一个结点指针用来遍历，另一个结点指针用来销毁结点
    quadNode *temp=head,*present=head;

    //循环遍历链表所有的结点
    while(present!=nullptr)
    {
        temp=temp->callnext();//temp指针用来遍历，销毁当前结点之前需要
                              //使temp指向下一个结点

        delete present;//销毁当前结点

        present=temp;//present指针后移
    }
}

//用来找到指定索引n的结点
quadNode* quadList::find(int n)
{
    //从头节点开始遍历
    quadNode *present=head;
    for(int i=1;i<=n;i++)
    {
        present=present->callnext();//present指针后移
    }
    return present;//找到目标结点之后，返回该结点的地址
}

//返回当前存储的大地四边形个数
int quadList::showNum()
{
    return Num;
}
