#include "func.h"
//自定义函数的函数体部分

double torad(double angle)
{
    //输入的形式是double，整数部分为度，小数点后前两位为分，后面剩余部分为秒
    //将单位为度、分、秒d的部分都取出
    double hour=floor(angle);//向下取整
    double min=floor(100*(angle-hour));
    double sec=100*(100*(angle-hour)-min);

    double temp=hour+min/60+sec/3600;//用变量temp表示以度为单位的角度值
    return (temp*pi)/180;//将以度为单位的角度值变为以弧度为单位的角度值
}

double cot(double angle)
{
    return 1/tan(angle);//余切值为对应正切值的倒数
}

double todegminsec(double rad)
{
    double angle=rad*180/pi;
    //先将弧度转换为以度为单位的角度

    double hour=floor(angle);//向下取整，取出度的部分
    double min=floor((angle-hour)*60);//剩余的部分转换为以分为单位，再对分取整
    double sec=((angle-hour)*60-min)*60;//剩余的所有部分都转换为以秒为单位

    return hour+min/100+sec/10000;//将以度分秒为单位表示的角度以double类型返回
}

void warn(QString text)//弹出不同内容的消息框，内容为text参数
{
    QMessageBox box;
    box.setText(text);//将消息框对象的内容置为text
    box.exec();//弹出消息框
}
