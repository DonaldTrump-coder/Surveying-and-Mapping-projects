#include "polygon_calculate.h"
#include "./ui_polygon_calculate.h"
#include<QMessageBox>

polygon_calculate::polygon_calculate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::polygon_calculate)
{
    ui->setupUi(this);
}

polygon_calculate::~polygon_calculate()
{
    delete ui;
}

//构造函数，将用户输入的用于计算的数据文本内容以正确的格式传入内存空间
calculation::calculation(QString text)
{
    QStringList list=text.split('\n');
    //将用户输入的大段文本根据换行符分割，
    //提取出每一行并存在QStringList类型的变量中

    //使用检查函数判断拆分后的每一行是否都符合格式要求，便于计算
    test(list);

    //若输入文本不符合格式要求，则无法进行计算，需进行错误处理
    if(wrong==1)
    {
        warn();//警告函数，弹出对话框
        N=0;
        p=NULL;//将变量成员N和p都置为默认值，避免报错
        return;//格式不符合要求，就无需继续处理数据了
    }

    //输入的第一行是所有点的个数，将输入的第一行存入变量N中
    N=list[0].toInt();

    //根据点个数建立存储点坐标的动态数组
    p=new Point[N];

    //将下面每一行的点坐标数据都读入动态数组中
    //对于每一行，第一个逗号前为点号，两个逗号之间为x坐标，第二个逗号之后为y坐标
    for(int i=0;i<N;i++)
    {
        p[i].x=list[i+1].split(',')[1].toDouble();
        p[i].y=list[i+1].split(',')[2].toDouble();
    }
}

//用于实现计算功能的主要函数
double calculation::calculate()
{
    if(wrong==1)
        return 0;
    //计算前判断格式是否正确
    //若不正确，则无需进行计算，将面积结果置为默认值0

    double result=0;
    //变量result是计算得到的面积结果，初始值为0，在循环中累加

    //点坐标按顺序存储在动态数组p中
    //公式中前n-1个点的下标符合规律，可直接按公式中的下标循环，不会出现下标越界的问题
    for(int i=0;i<N-1;i++)
        result=result+0.5*(p[i+1].x+p[i].x)*(p[i+1].y-p[i].y);

    //对于最后一项，公式中的i+1变成1，
    //此处不在每一次循环中判断是否为最后一项，而选择在整个循环结束后单独表示并加上最后一项
    result=result+0.5*(p[0].x+p[N-1].x)*(p[0].y-p[N-1].y);
    //在数组中，第一个点在程序语言中的下标为0，第n个点的下标为n-1

    return abs(result);
    //点的排列顺序可能是逆时针，也可能是顺时针
    //逆时针排列输入公式的点和顺时针排列输入公式的点计算后的结果互为相反数，但值的大小都是正确的
    //为同时支持两种顺序的输入方式，最后输出的面积结果需要取绝对值
    //对于同样的一组点，以顺时针或逆时针的顺序输入程序，最后的结果都是一样的
}

//用于检查输入数据的格式的函数
void calculation::test(QStringList list)
{
    wrong=0;//类中用于判断格式正确性的变量初始先置为0，即无误
    //在判断中发现错误时再置为1

    //第一行输入的点数不可能是0或负数
    //要组成多边形，至少要有三个点
    if (list[0].toInt()<=2)
    {
        wrong=1;
        return;
    }
    //为减小计算机的计算量，每次检查出错误之后就可结束检查程序，无需进行后续其它的判断

    //检查点数与输入的点坐标数量是否匹配
    //检查函数使用在初始化变量成员之前，因此要表示点数只能用list[0].toInt()，不能用N
    if((list.size()-1)!=list[0].toInt())
    {
        wrong=1;
        return;
    }

    //检查输入点坐标的行中，是否有使用两个逗号将三个值分隔开
    for(int i=1;i<=list[0].toInt();i++)
    {
        if(list[i].split(',').size()!=3)
        {
            wrong=1;
            return;
        }
    }
}

calculation::~calculation()
{
    delete[] p;//析构函数中将动态数组的指针p指向的内存释放
    p=NULL;
}

//用于弹出警告框的警告函数
void calculation::warn()
{
    QMessageBox warning;//建立消息框对象
    warning.setText("请输入正确的格式");//在弹出的消息框中添加文本
    warning.exec();//显示消息框
}

//当用户按下“计算”按钮时，执行计算并显示的程序
void polygon_calculate::on_pushButton_clicked()
{
    calculation a(ui->textEdit->toPlainText());//将输入进多行文本框textEdit的内容传入建立的对象中

    ui->textBrowser->setText(QString::number(a.calculate(),'f',6));
    //对对象中的对应数据进行计算并显示在textBrowser中
}

//当用户按下“重新输入”按钮时，将清空所有的输入、输出内容，便于用户重新输入
void polygon_calculate::on_pushButton_2_clicked()
{
    ui->textBrowser->clear();
    ui->textEdit->clear();
}

