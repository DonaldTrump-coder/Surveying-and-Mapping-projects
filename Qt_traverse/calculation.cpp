#include "calculation.h"
#include <QObject>
#include "func.h"
#include <QFileDialog>

//文件操作函数，可同时进行读或写操作
QString file_operation(file_opera a,QString content)
{
    //读文件
    if(a==read)
    {
        QFileDialog file_dlg;//选取需要读取的文件地址
        QString pth=file_dlg.getOpenFileName(nullptr,"打开文件","","*.dat *.txt");
        QFile file(pth);//将文件地址传给QFile类型对象

        //打开文件
        bool ret=file.open(QIODeviceBase::ReadOnly|QIODeviceBase::Text);
        if(ret==0)
        {
            warn("打开失败");
            return QString("打开失败");
        }

        //读取文件内容并返回文本
        QString text=file.readAll();

        file.close();//关闭文件
        warn("打开文件成功，请检查数据是否有误");
        return text;
    }

    //写文件
    else
    {
        //选取需要保存的文件地址
        QFileDialog file_dlg;
        QString pth=file_dlg.getSaveFileName(nullptr,"保存文件","","*.txt");
        QFile file(pth);
        bool ret=file.open(QIODeviceBase::WriteOnly|QIODeviceBase::Text);
        if(ret==0)
        {
            warn("保存失败");
            return QString("保存失败");
        }

        //将需要保存的内容传给QTextStream类型对象
        QTextStream stream(&file);
        stream<<content;

        file.close();//关闭文件
        warn("保存文件成功");
        return "";
    }
}

//用于获取已知参数的构造函数
calculation::calculation(QString content)
{
    //按行分割
    QStringList text=content.split('\n');

    //整个数据内容至少需要9行，否则就是内容不完整
    //检查内容是否完整
    if(text.length()<9)
    {
        warn("输入数据不完整！");
        wrong=1;
        return;
    }

    //检查观测边、角个数是否都输入正整数
    if(text[0].contains('.')||text[0].toInt()<=0)
    {
        warn("观测边个数请输入正整数！");
        wrong=1;
        return;
    }
    if(text[1].contains('.')||text[1].toInt()<=0)
    {
        warn("观测角个数请输入正整数！");
        wrong=1;
        return;
    }

    line_num=text[0].toInt();//读取的内容存入对象中
    angle_num=text[1].toInt();

    //根据读取的个数建立相应的动态数组
    points=new point[angle_num+2];//包含了所有点的数组
    lines=new line[line_num];//包含了所有边的数组

    //检查输入已知点坐标的数据是否用逗号分成了三份
    if(text[3].split(',').length()!=2||text[5].split(',').length()!=2)
    {
        warn("请输入正确的已知点坐标！");
        wrong=1;
        return;
    }

    //在点数组的始、末存入已知点数据
    points[0].code=text[2];//存点号
    points[0].x=text[3].split(',')[0].toDouble();//存x坐标
    points[0].y=text[3].split(',')[1].toDouble();//存y坐标
    points[angle_num+1].code=text[4];
    points[angle_num+1].x=text[5].split(',')[0].toDouble();
    points[angle_num+1].y=text[5].split(',')[1].toDouble();

    //遍历边观测数据，按顺序存入观测的点号
    int counter;//行计数器

    //按顺序遍历点数组
    for(int point_index=0;point_index<angle_num+1;point_index++)
    {
        //由于输入的观测数据可以不按顺序来
        //因此需要逐行对观测边数据进行检查，从点数组起点一直查找到终点
        //构建出观测点的顺序
        counter=6+angle_num;
        for(int i=0;i<line_num;i++,counter++)
        {
            //解译观测边数据
            if(text[counter].split(',').length()!=3)
            {
                //判断观测边数据是否被逗号分割成了三份
                warn("请输入正确的观测边数据！");
                wrong=1;
                return;
            }

            //点数组的起点已知，因此从起点开始循环进行点观测顺序的检查
            //若观测边数据的起始点与当前的已知点相同，则将观测边的终点作为下一个已知点
            if(text[counter].split(',')[0]==
                points[point_index].code)
            {
                //观测点、观测边都分别按顺序存储
                points[point_index+1].code=text[counter].split(',')[1];
                lines[point_index].length=text[counter].split(',')[2].toDouble();
                break;//找到下一个点后就跳到下一个点作为已知点，不用再继续搜索观测边数据
            }
        }
    }

    //读入观测点中的角度数据
    counter=6;
    for(int i=0;i<angle_num;i++,counter++)
    {
        QStringList angle_information=text[counter].split(',');
        if(angle_information.length()!=3)
        {
            //判断观测点数据是否被逗号分割成了三份
            warn("请输入正确的观测角数据！");
            wrong=1;
            return;
        }

        //判断读观测点数据的过程中是否会读到观测边数据
        //若读到观测边数据，说明观测点数据的个数不匹配
        if(angle_information[2]!="left"&&angle_information[2]!="right")
        {
            warn("观测点或观测边数据的个数与实际输入的个数不符！");
            wrong=1;
            return;
        }

        //导线中不会存在观测点号不出现在观测边中的情况
        //用一个布尔变量检查是否有观测点没在之前参与点数组的构建
        bool exist=0;
        for(int k=0;k<angle_num+2;k++)
        {
            //根据构建的点顺序逐行搜索符合顺序的点
            //并将对应的观测角数据存入点数组
            if(points[k].code==angle_information[0])
            {
                exist=1;

                //观测角数据是度分秒类型，用函数转换成弧度制
                points[k].point_angle.val=torad(angle_information[1].toDouble());

                //存入左角、右角信息
                if(angle_information[2]=="left")
                    points[k].point_angle.feature=point::angle::left;
                if(angle_information[2]=="right")
                    points[k].point_angle.feature=point::angle::right;
            }
        }

        if(exist==0)
        {
            //若有观测点没在观测边数据中，说明出现了多余的点号
            warn("观测边的点号和观测角的点号不匹配！");
            wrong=1;
            return;
        }
    }
}

//析构函数中释放构造函数申请的动态内存
calculation::~calculation()
{
    delete[] points;
    delete[] lines;
}

//根据读入的数据进行方位角和坐标估算
void calculation::estimation()
{
    //设定一个起始边的方位角初值
    lines[0].direction=pi/2;

    //dx，dy存储每个边的坐标差
    double dx,dy;

    //逐点、逐边推算点坐标和方位角
    for(int i=1;i<line_num;i++)
    {
        //根据前一个边的已知方位角推算xy坐标差
        dx=lines[i-1].length*cos(lines[i-1].direction);
        dy=lines[i-1].length*sin(lines[i-1].direction);

        //推算当前点坐标
        points[i].x=points[i-1].x+dx;
        points[i].y=points[i-1].y+dy;

        //根据观测角数据和前一个边的一直方位角推算当前边的方位角
        if(points[i].point_angle.feature==point::angle::left)
            lines[i].direction=lines[i-1].direction+points[i].point_angle.val-pi;
        if(points[i].point_angle.feature==point::angle::right)
            lines[i].direction=lines[i-1].direction-points[i].point_angle.val+pi;

        //方位角等效至0-2π范围内
        rad_normalize(lines[i].direction);
    }

    //推算最后一个点的坐标，并将其与真实坐标比较
    dx=lines[line_num-1].length*cos(lines[line_num-1].direction);
    dy=lines[line_num-1].length*sin(lines[line_num-1].direction);
    double estimated_x=points[line_num-1].x+dx;
    double estimated_y=points[line_num-1].y+dy;

    //计算估算的的起终点方位角
    double estimated_alpha=azimuth(points[0].x,points[0].y,estimated_x,estimated_y);

    //计算真实的起终点方位角
    double exact_alpha=azimuth(points[0].x,points[0].y,points[angle_num+1].x,points[angle_num+1].y);

    //计算方位角改正数
    theta=exact_alpha-estimated_alpha;

    //计算估算的起终点距离
    dx=estimated_x-points[0].x;
    dy=estimated_y-points[0].y;
    double estimated_length=sqrt(dx*dx+dy*dy);

    //计算真实的起终点距离
    dx=points[angle_num+1].x-points[0].x;
    dy=points[angle_num+1].y-points[0].y;
    double exact_length=sqrt(dx*dx+dy*dy);

    //计算闭合边长度比
    R=exact_length/estimated_length;

    //计算全长相对闭合差
    T=abs((exact_length-estimated_length)/exact_length);
}

//给定限差，检查闭合差是否在范围内并进行改正
int calculation::correction(double limit)
{
    limit=abs(1/limit);

    //若全长相对闭合差超限，则无需进行后续改正，直接返回错误
    if(T>limit)
        return -1;

    //对每条边的边长、方位角进行改正
    for(int i=0;i<line_num;i++)
    {
        lines[i].direction=lines[i].direction+theta;

        //将方位角限定在0-2π范围内
        rad_normalize(lines[i].direction);

        lines[i].length=lines[i].length*R;
    }

    //重新计算改正后的每个点坐标
    double dx,dy;
    for(int i=1;i<angle_num+1;i++)
    {
        dx=lines[i-1].length*cos(lines[i-1].direction);
        dy=lines[i-1].length*sin(lines[i-1].direction);
        points[i].x=points[i-1].x+dx;
        points[i].y=points[i-1].y+dy;
    }
    return 0;
}

//根据
QString calculation::output(bool wrong)
{
    //result作为返回的文本字符串
    QString result;

    //通过wrong布尔变量判断是否超出限差
    if(wrong==1)
    {
        result="闭合差超出限差！";
    }

    else
    {
        //若未超出限差，则输出平差结果
        result="平差后结果为：\n";

        //将每个点的坐标按行输出
        for(int i=0;i<angle_num+2;i++)
        {
            //点号+x+y
            result=result+QString("%1: %2,%3")
                                  .arg(points[i].code,
                                       QString::number(points[i].x,'f',3),
                                       QString::number(points[i].y,'f',3));
            result=result+"\n";
        }

        //输出全场相对闭合差
        result=result+"全长相对闭合差为：1/"+QString("%1").arg(int(1/T));
    }
    return result;
}

bool calculation::test()
{
    return wrong;
}
