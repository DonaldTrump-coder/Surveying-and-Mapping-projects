#include <QString>
#define pi 3.1415926
#define rou 206265

//获取打开文件的路径
QString getopenfilename();

//获取打开文件的内容
QString getopenfile(QString filename);

//获取保存文件的路径
QString getsavefilename();

//获取保存文件的文件夹
QString getsavefolder();

//保存文件
void savefile(QString result,QString filename);

//度分秒转换为弧度
double dms2rad(double dms);

//double类型取绝对值
double dabs(double a);

//将弧度置为0-2pi之间
double rad_abs(double rad);

//取cot值
double cot(double rad);

//将秒置为0附近
double sec_abs(double sec);

//弧度转换为度分秒
double rad2dms(double rad);

//弹出消息框
void warning(QString content);
