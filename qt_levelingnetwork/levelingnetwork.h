#ifndef LEVELINGNETWORK_H
#define LEVELINGNETWORK_H
#include<QString>
#include<QObject>

//为水准网类中的已知点和未知点分别建立结构体类型
struct knownpoint
{
    QString number;//点号
    double height;//已知点的高程
};

struct unknownpoint
{
    QString number;//点号
    double height;//未知点高程，即高程估值
    knownpoint *knownneighbor=NULL;//指向用于估算其高程的已知点
    unknownpoint *unknownneighbor=NULL;//指向用于估算其高程的未知点
};//一个未知点结构体中，用于估算其高程的已知点或未知点只需要一个，因此两个指针中可有一个为空指针

class levelingnetwork
{
public:
    void settext(QString inp_text);//向水准网类中读入数据内容并解译的函数
    void calculate_appro_using_knownpoint();//根据观测数据中的已知点计算未知点高程估值
    void calculate_appro_using_unknownpoint();//根据观测数据中的未知点计算剩余未知点高程估值
    QString output_text();//根据计算的未知点高程估值显示所有点的高程信息
    QString show_weight();//根据输入的观测路程计算并输出权矩阵
    void init();//将水准网类中的数值和指针进行初始化操作
    bool judge();//判断输入数据的格式是否有误并选择是否弹出消息框
    ~levelingnetwork();//销毁动态创建的内存
private:
    knownpoint *knownpoints=NULL;//用于指向所有已知点的指针
    unknownpoint *unknownpoints=NULL;//用于指向所有未知点的指针
    int knownpoints_num=0;//水准网类中已知点的个数
    int unknownpoints_num=0;//水准网中未知点的个数
    int observation_num=-1;//观测数据的条数
    QStringList text;//输入的观测文本数据
    double *weights=NULL;//每条观测的权组成的数组
    bool error=0;//用于判断输入数据格式是否正确的布尔变量
};

#endif // LEVELINGNETWORK_H
