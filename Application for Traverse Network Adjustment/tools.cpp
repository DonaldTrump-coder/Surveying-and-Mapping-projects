#include "tools.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

QString getopenfilename()
{
    QFileDialog filedialog;

    //获取文件路径名
    QString result=filedialog.getOpenFileName(nullptr,
                                              "打开文件",
                                              "C:\\Users\\10527\\Desktop\\遥感程序设计课程实践资料",
                                              "Text(*.dat *.txt)");
    return result;
}

//打开文件，返回文件中的文本内容
QString getopenfile(QString filename)
{
    if(filename.isEmpty())
    {
        warning("未选择文件！");
        return "";
    }
    if(!filename.endsWith(".txt")&&!filename.endsWith(".dat"))
    {
        warning("暂不支持该文件类型！");
        return "";
    }
    QFile file(filename);
    bool ret=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(ret==0)
    {
        warning("请检查文件名是否正确！");
        file.close();
        return "";
    }
    QTextStream in(&file);
    QString result=in.readAll();
    file.close();
    if(result.isEmpty())
    {
        warning("请检查文件内容是否正确！");
        return "";
    }
    return result;
}

QString getsavefilename()
{
    QFileDialog filedialog;
    QString result=filedialog.getSaveFileName(nullptr,"保存文件","","Text(*.txt,*.dat)");
    return result;
}

QString getsavefolder()
{
    QFileDialog filedialog;
    QString path=filedialog.getExistingDirectory(nullptr,"保存文件","C:\\Users\\10527\\Desktop\\遥感程序设计课程实践资料");
    return path;
}

void savefile(QString result,QString filename)
{
    QFile file(filename);
    bool ret=file.open(QIODevice::WriteOnly|QIODevice::Text);
    if(ret==0)
    {
        return;
    }

    //用textstream保存文本文件
    QTextStream out(&file);
    out<<result;
    file.close();
}

double dabs(double a)
{
    if(a>0)
        return a;
    else
        return -a;
}

double dms2rad(double dms)
{
    double deg=(int)dms;
    double min=(int)((dms-deg)*100);
    double sec=((dms-deg)*100-min)*100;
    double degree=deg+min/60+sec/3600;
    return degree*pi/180;
}

double rad_abs(double rad)
{
    while(rad<0||rad>=2*pi)
    {
        if(rad<0)
            rad+=2*pi;
        else
            rad-=2*pi;
    }
    return rad;
}

double cot(double rad)
{
    return 1/tan(rad);
}

double sec_abs(double sec)
{
    //先将秒值化归至0-2pi之间
    while(sec<0||sec>2*pi*rou)
    {
        if(sec<0)
            sec+=2*pi*rou;
        else
            sec-=2*pi*rou;
    }

    //将接近2pi的秒减至0附近
    if((sec-2*pi*rou<100)&&(sec-2*pi*rou>-100))
        sec-=2*pi*rou;
    return sec;
}

double rad2dms(double rad)
{
    double degree=rad*180/pi;
    double deg=(int)degree;
    double min=(int)((degree-deg)*60);
    double sec=((degree-deg)*60-min)*60;
    return deg+min/100+sec/10000;
}

void warning(QString content)
{
    //弹出消息框，消息框中的内容为传入的参数
    QMessageBox messagebox;
    messagebox.setText(content);
    messagebox.show();
    messagebox.exec();
}
