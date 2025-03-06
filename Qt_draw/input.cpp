#include "input.h"
#include <QFileDialog>
#include "func.h"
#include <QObject>

//便于用户打开指定文件的函数
//返回打开文件的内容
QString input::getfile()
{
    //建立QFileDialog对象
    QFileDialog a;
    QString pth="";

    //用户通过选择文件的方式指定文件路径，存入pth变量中
    pth=a.getOpenFileName(nullptr,
                      "打开文件",
                      "",
                      "Text(*.dat *.txt)");
    //只能打开.dat和.txt文件

    //打开指定路径的文件
    QFile file(pth);
    bool ret=file.open(QIODevice::ReadOnly|QIODevice::Text);
    //以只读方式打开

    //若打开文件失败，则结束程序并弹出对应的警告框
    if(ret==0)
    {
        warn("打开失败");
        return "";
    }

    //读入文件内容，存入要返回的变量中
    QString text=file.readAll();

    //关闭文件并弹出提示框
    file.close();
    warn("文件已打开，请检查\n无误后再进行绘制");

    return text;
}

//将输入的文本进行解译
void input::init(QString text)
{
    //先初始化动态内存，防止内存泄漏
    destroy();

    //用于表示输入格式错误的状态变量先置为0
    wrong=0;

    //按行分割内容
    QStringList lines=text.split('\n');

    //输入的文本至少需要有6行，否则就是不完整
    if(lines.length()<6)
    {
        warn("请输入完整的数据！");
        wrong=1;
        return;
    }

    //line变量用于遍历每一行
    int line=0;

    //对于含有#的行，不予处理，直接跳过
    while(lines[line].contains('#'))
    {
        line++;
    }

    //点数必须是正整数，不可含有小数点和负数
    if(lines[line].contains('.')||lines[line].toInt()<=0)
    {
        warn("点数请输入正整数！");
        wrong=1;
        return;
    }

    //将点数读入内存
    point_num=lines[line].toInt();
    line++;

    //三角形个数必须是正整数，不可含有小数点和负数
    if(lines[line].contains('.')||lines[line].toInt()<=0)
    {
        warn("三角形个数请输入正整数！");
        wrong=1;
        return;
    }

    //将三角形个数读入内存
    tri_num=lines[line].toInt();

    //依据读入的点数、三角形个数动态创建点、三角形数组
    points=new point[point_num];
    triangles=new triangle[tri_num];

    //读下一行
    line++;

    //依据点数用循环遍历所有的点数据
    for(int i=0;i<point_num;i++)
    {
        //点数据行开头必须包含‘v’字符表示
        if(!lines[line].contains('v'))
        {
            warn("请输入正确的点数！");
            wrong=1;
            return;
        }

        //点数据必须被空格分为四份
        if(lines[line].split(' ').length()!=4)
        {
            warn("请输入正确的点坐标！");
            wrong=1;
            return;
        }

        //分别读入点的xyz信息
        points[i].x=lines[line].split(' ')[1].toDouble();
        points[i].y=lines[line].split(' ')[2].toDouble();
        points[i].z=lines[line].split(' ')[3].toDouble();

        //每读完一个点，就读下一行的点
        line++;
    }

    //依据三角形个数用循环遍历所有的三角形数据
    for(int i=0;i<tri_num;i++)
    {
        //三角形数据行开头必须包含‘f’字符表示
        if(!lines[line].contains('f'))
        {
            warn("请输入正确的三角形个数！");
            wrong=1;
            return;
        }

        //三角形数据也必须被空格分成四份
        if(lines[line].split(' ').length()!=4)
        {
            warn("请输入正确的三角形顶点！");
            wrong=1;
            return;
        }

        //分别读入三角形的点号信息
        triangles[i].a=lines[line].split(' ')[1].toInt();
        triangles[i].b=lines[line].split(' ')[2].toInt();
        triangles[i].c=lines[line].split(' ')[3].toInt();

        //每读完一个三角形，就读下一行的三角形
        line++;
    }
}

//销毁所有的动态内存
//该函数可用于读新数据之前和析构函数中
void input::destroy()
{
    delete[] points;
    delete[] triangles;
    points=nullptr;
    triangles=nullptr;
    point_num=tri_num=0;
    //将点个数、三角形个数都置为默认值0
}

input::~input()
{
    destroy();
}

point* input::getpoints()
{
    return points;
}

triangle* input::gettriangles()
{
    return triangles;
}

//依据yz的最大、最小值求取其范围的中心点
point input::findcenter()
{
    //通过遍历所有点寻找xyz坐标的最小值
    double xmin=points[0].x;
    double ymin=points[0].y;
    double zmin=points[0].z;
    for(int i=0;i<point_num;i++)
    {
        if(points[i].x<xmin)
            xmin=points[i].x;
        if(points[i].y<ymin)
            ymin=points[i].y;
        if(points[i].z<zmin)
            zmin=points[i].z;
    }

    //将xyz坐标最大、最小值之间的跨度取一半并加到最小值上
    //相当于找最大、最小值之间的中点
    double dx=findspan().x/2;
    double dy=findspan().y/2;
    double dz=findspan().z/2;
    double x=xmin+dx;
    double y=ymin+dy;
    double z=zmin+dz;

    //建立一个用于返回的点类型变量
    point result;
    result.x=x;
    result.y=y;
    result.z=z;
    return result;
}

//用于寻找xyz坐标最大、最小值之间的跨度
point input::findspan()
{
    //初始化xyz坐标最大、最小值
    double xmin=points[0].x;
    double xmax=points[0].x;
    double ymin=points[0].y;
    double ymax=points[0].y;
    double zmin=points[0].z;
    double zmax=points[0].z;

    //通过遍历所有点找到xyz坐标的最大、最小值
    for(int i=0;i<point_num;i++)
    {
        if(points[i].x<xmin)
            xmin=points[i].x;
        if(points[i].x>xmax)
            xmax=points[i].x;
        if(points[i].y<ymin)
            ymin=points[i].y;
        if(points[i].y>ymax)
            ymax=points[i].y;
        if(points[i].z<zmin)
            zmin=points[i].z;
        if(points[i].z>zmax)
            zmax=points[i].z;
    }

    //根据xyz坐标的最大、最小值计算跨度
    //并存入要返回的点数据变量中
    point result;
    result.x=xmax-xmin;
    result.y=ymax-ymin;
    result.z=zmax-zmin;
    return result;
}

//返回三角形个数的变量
int input::gettrinum()
{
    return tri_num;
}

//返回表示是否有输入格式错误的状态变量
bool input::test()
{
    return wrong;
}
