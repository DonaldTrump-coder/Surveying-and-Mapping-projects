#include "sort_cass.h"
#include <QObject>
#include "func.h"

//在构造函数中向内存存入数据
sort_cass::sort_cass(QString text)
{
    //将整段数据按行分隔
    QStringList lines=text.split('\n');

    //判断首行的点数是否正确
    //点数只能是正整数
    if(lines[0].contains('.')||lines[0].toInt()<=0)
    {
        wrong=1;

        //弹出警告框
        warn("点数请输入正整数");

        return;
        //若格式不对，则无需再进行后续的读取数据步骤
    }

    total_eff=total_num=lines[0].toInt();
    //将总点数都置为首行的数据

    //判断实际的数据行数是否与首行的点数相等
    if(lines.length()!=total_num+1)
    {
        //若不相等则视为输入格式错误，停止继续处理
        wrong=1;
        warn("请输入正确的点个数");
        return;
    }

    //依据总点数动态创建点数组，存储点数据
    points=new point[total_num];

    //present变量用于指示当前存入的点数
    int present=0;

    //遍历输入文本中的所有点，依次存入点数组中
    for(int i=0;i<total_num;i++)
    {
        //每对一个点进行处理，就先默认将same变量置为0
        //若该点重复了，就后续将其置为1
        bool same=0;

        //每读一个点前先检查所有已读入的点，看是否重复
        for(int k=0;k<present;k++)
        {
            if(points[k].num==lines[i+1].split(',')[0].toInt())
            {
                //若有重复的点，就将表示未重复点总数的变量减一
                total_eff--;

                //将表示重复的变量置为1
                same=1;

                //结束重复点检查
                break;
            }
        }

        //若该行对应的点是重复点，则无需读入其数据
        //直接跳转至下一行数据
        if(same==1)
        {
            continue;
        }

        //若不是重复点，则将该行数据拆分并分别存入内存中
        QStringList elements=lines[i+1].split(',');
        points[i].num=elements[0].toInt();
        points[i].code=elements[1];
        points[i].x=elements[2].toDouble();
        points[i].y=elements[3].toDouble();
        points[i].h=elements[4].toDouble();
        present++;//每读入一个点，就将已读入的点数加一
    }
}

//排序函数，依据点号大小排序
void sort_cass::sort()
{
    //使用冒泡排序的方法
    for(int i=0;i<total_num-1;i++)
        for(int k=0;k<total_num-1-i;k++)
        {
            if(points[k].num>points[k+1].num)
            {
                //将点号大的点换至数组末端
                swap1(points[k],points[k+1]);
            }
        }
}

//返回输出值的函数
QString sort_cass::output()
{
    //整理后的点数，应是无重复的点数，输出的点也应是无重复的点
    QString output=QString("%1").arg(QString::number(total_eff));
    output=output+"\n";

    for(int i=0;i<total_num;i++)
    {
        //重复的点未读入，但其对应的存储空间仍存在
        //其点号默认值为-1，而输入文本内容中的点号都不可能是负数
        //因此若遇到该无意义的点，直接跳至下一点
        if(points[i].num==-1)
            continue;

        //在输出点对应的行中加上该点对应的各项数据
        output=output+QString::number(points[i].num)+",";
        output=output+points[i].code+",";
        output=output+QString::number(points[i].x)+",";
        output=output+QString::number(points[i].y)+",";
        output=output+QString::number(points[i].h)+"\n";
    }

    //将构造好的输出字符串返回
    return output;
}

//返回输入数据是否有格式错误的函数
bool sort_cass::test()
{
    return wrong;//返回wrong状态变量
}

//析构函数
sort_cass::~sort_cass()
{
    //释放点数组的数据
    delete[] points;
    points=NULL;
}
