#include "levelingnetwork.h"
#include "func.h"

void levelingnetwork::settext(QString inp_text)
{
    QStringList inp_list=inp_text.split('\n');
    //将输入至控件的文本按行拆分，对每一行进行解译

    //输入的行数最少为6，若输入的行数小于6，则一定是输入数据不完整
    //检查是否输入完整
    if(inp_list.length()<6)
    {
        error=1;
        warn("请输入完整的数据！");
        return;
    }

    //第一行表示已知点点数，若该数小于等于零或带有小数点
    //则不是正整数，说明输入有误
    if(inp_list[0].contains('.')||inp_list[0].toInt()<=0)
    {
        error=1;
        warn("已知点数请输入正整数！");
        return;
    }

    //若输入点数正确，将点数转为整数类型，保存在水准网对象中
    //并依据该点数动态创建一个存储已知点的数组
    knownpoints_num=inp_list[0].toInt();
    knownpoints=new knownpoint[knownpoints_num];

    int line;//从第二行开始，用于记录当前行数

    //沿输入的文本数据遍历所有的已知点信息
    for(line=1;line<=knownpoints_num;line++)
    {
        //输入的已知点信息部分只包含一个逗号，分为点号、高程两部分
        //检查是否只有两部分
        if(inp_list[line].split(',').length()!=2)
        {
            error=1;
            warn("请按要求输入已知点点号和高程！");
            return;
        }

        //将每一行的点号和高程都读入水准网对象
        knownpoints[line-1].number=inp_list[line].split(',')[0];
        knownpoints[line-1].height=inp_list[line].split(',')[1].toDouble();
    }

    //已知点后的内容是未知点点数，以同样的方法判断未知点点数是否输入正确
    if(inp_list[line].contains('.')||inp_list[line].toInt()<=0)
    {
        error=1;
        warn("未知点数请输入正整数！");
        return;
    }

    //以同样的方法创建未知点动态数组
    unknownpoints_num=inp_list[line].toInt();
    unknownpoints=new unknownpoint[unknownpoints_num];

    line++;//读下一行

    //该行只包含未知点的点号，因此继续将其拆解并分别读入点号
    QStringList unknownnumtext=inp_list[line].split(',');
    //点号之间以逗号分隔

    //判断未知点拆解后得到的个数与前面的点数是否相等
    if(unknownnumtext.length()!=unknownpoints_num)
    {
        error=1;
        warn("未知点点数与点号个数不相等！");
        return;
    }

    //将未知点的点号都存入水准网对象的未知点数组中
    for(int i=0;i<unknownpoints_num;i++)
    {
        unknownpoints[i].number=unknownnumtext[i];
    }

    line++;//读下一行

    //同样的方法判断观测个数是否正确
    if(inp_list[line].contains('.')||inp_list[line].toInt()<=0)
    {
        error=1;
        warn("观测个数请输入正整数！");
        return;
    }

    //存入观测个数，同时为存储每一个观测对应的权建立动态数组
    observation_num=inp_list[line].toInt();
    weights=new double[observation_num];

    //判断输入的观测数据条数和前面的观测个数是否相等
    if(inp_list.length()-4-knownpoints_num!=observation_num)
    {
        error=1;
        warn("观测数据个数与实际输入的观测数据不匹配！");
        return;
    }

    //遍历所有观测数据
    //将观测数据按行存入水准网对象的text数据成员中
    for(int i=0;i<observation_num;i++)
    {
        line++;
        text.append(inp_list[line]+'\n');
        weights[i]=inp_list[line].split(',')[3].toDouble();
        //将观测距离按观测对应存入数组中，真正的权还需要取倒数
    }
}

//析构函数
levelingnetwork::~levelingnetwork()
{
    //对象销毁时，将已知点、未知点指针指向的对应的点数组销毁
    delete[] knownpoints;
    delete[] unknownpoints;
    knownpoints=NULL;
    unknownpoints=NULL;

    //将权值数组销毁
    delete[] weights;
    weights=NULL;
}

void levelingnetwork::calculate_appro_using_knownpoint()
{
    int iter;//用于遍历每一行观测
    for(iter=0;iter<observation_num;iter++)//遍历每一行观测
    {
        QStringList line_content=text[iter].split(',');//将观测行拆分并读入
        for(int i=0;i<knownpoints_num;i++)//遍历所有已知点，匹配当前观测行中的已知点
        {
            if(knownpoints[i].number==line_content[0])
                //考虑第一个观测值匹配到了已知点的情况
                {
                    for(int k=0;k<unknownpoints_num;k++)
                    //遍历所有未知点
                        if(unknownpoints[k].number==line_content[1])
                            //当第二个观测值是未知点时，才进行下一步
                        {
                            if(unknownpoints[k].knownneighbor==NULL&&
                                unknownpoints[k].unknownneighbor==NULL)
                                //当该未知点是未估算过的，才对该点进行估算
                            {
                                unknownpoints[k].height=knownpoints[i].height+
                                line_content[2].toDouble();
                                //未知点高程-已知点高程=观测值
                                //未知点高程=已知点高程+观测值

                                unknownpoints[k].knownneighbor=&knownpoints[i];
                                //将该未知点的已知点指针指向该已知点，用于标记
                                break;//结束估算后，跳出寻找未知点的循环
                            }
                        }
                    break;
                    //匹配到了一个已知点，那么另一个点此时一定是已估算完的未知点
                    //因此此时可直接跳到下一行观测
                }
            else if(knownpoints[i].number==line_content[1])
                    //考虑第二个观测值匹配到了已知点的情况
                {
                    for(int k=0;k<unknownpoints_num;k++)
                    {
                        if(unknownpoints[k].number==line_content[0])
                        {
                            if(unknownpoints[k].knownneighbor==NULL&&
                                unknownpoints[k].unknownneighbor==NULL)
                            {
                                unknownpoints[k].height=knownpoints[i].height-
                                line_content[2].toDouble();
                                unknownpoints[k].knownneighbor=&knownpoints[i];
                                break;
                            }
                        }
                    }
                    break;
                }
        }
    }
}

void levelingnetwork::calculate_appro_using_unknownpoint()
{
    //该方法采用整体多次循环的方式进行估算，以保证所有未知点都能有估值

    bool done=0;//用于判断所有的未知点是否都有估值
    while(done==0)//只有所有未知点都有估值时，才停止该程序
        {
            int iter;//用于遍历每一行观测
            for(iter=0;iter<observation_num;iter++)
            {
                //遍历每一行观测

                QStringList line_content=text[iter].split(',');
                //对观测行进行拆分和读入

                for(int i=0;i<unknownpoints_num;i++)//遍历所有未知点
                {
                    if(unknownpoints[i].knownneighbor!=NULL||unknownpoints[i].unknownneighbor!=NULL)
                    {
                        //判断是否能在第一个点匹配到计算过估值的未知点
                        if(unknownpoints[i].number==line_content[0])
                        {
                            for(int k=0;k<unknownpoints_num;k++)//遍历所有的未知点
                            {
                                //判断是否能在第二个点匹配到无估值的未知点
                                if(unknownpoints[k].number==line_content[1])
                                {
                                    if(unknownpoints[k].knownneighbor==NULL&&
                                        unknownpoints[k].unknownneighbor==NULL)
                                    {
                                        //计算无估值未知点的高程估值，计算方法与前面相似
                                        unknownpoints[k].height=unknownpoints[i].height+
                                        line_content[2].toDouble();

                                        unknownpoints[k].unknownneighbor=&unknownpoints[i];
                                        //记录当前未知点已有估值

                                        break;//结束寻找未知点的过程
                                    }
                                }
                            }
                            break;
                            //匹配到了一个已有估值的未知点，那么另一个点此时一定是已知点或已有估值的未知点
                            //此时应当直接调至下一条观测
                        }

                        //判断是否能在第二个点匹配到计算过估值的未知点
                        else if(unknownpoints[i].number==line_content[1])
                        {
                            for(int k=0;k<unknownpoints_num;k++)
                            {
                                if(unknownpoints[k].number==line_content[0])
                                {
                                    //判断是否能在第一个点匹配到无估值的未知点
                                    if(unknownpoints[k].knownneighbor==NULL&&
                                        unknownpoints[k].unknownneighbor==NULL)
                                    {
                                        unknownpoints[k].height=unknownpoints[i].height-
                                        line_content[2].toDouble();
                                        unknownpoints[k].unknownneighbor=&unknownpoints[i];
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                done=1;//每次进行完未知点的匹配，就将done变量置为1

                //再对未知点完整性进行检查
                for(int m=0;m<unknownpoints_num;m++)
                {
                    if(unknownpoints[m].knownneighbor==NULL&&
                        unknownpoints[m].unknownneighbor==NULL)
                    {
                        done=0;
                        //若还存在未标记过有估值的未知点，则应再遍历一遍观测数据进行点匹配和计算
                    }
                }
                if(done==1)
                    break;
                //若未知点都已有估值，则可以结束该子程序
            }
        }
}

QString levelingnetwork::output_text()
{
    QString output="";//用于输出的初始文本

    //将已知点、未知点的点号和高程都加进用于输出的文本中
    //每个点之间以换行符相隔
    for(int i=0;i<knownpoints_num;i++)
    {
        output=output+knownpoints[i].number+","+
        QString::number(knownpoints[i].height,'f',6)+"\n";
    }
    for(int i=0;i<unknownpoints_num;i++)
    {
        output=output+unknownpoints[i].number+","+
        QString::number(unknownpoints[i].height,'f',6)+"\n";
    }
    return output;//输出文本
}

void levelingnetwork::init()
{
    //初始化时将之前所有的已知点、未知点都删除，便于重新计算
    delete[] unknownpoints;
    delete[] knownpoints;
    unknownpoints=NULL;
    knownpoints=NULL;

    //将水准网对象中的输入文本清空
    text.clear();

    //将权值数组删除
    delete[] weights;
    weights=NULL;

    //将error变量恢复为初始值
    error=0;
}

QString levelingnetwork::show_weight()
{
    QString matrix="";//初始输出文本为空

    //若初始化时输入数据有误，则无需输出权矩阵，无需执行后续程序
    if(error==1)
    {
        warn("请在初始化时输入正确格式的数据！");
        return matrix;
    }

    //创建double类型的动态数组，存储矩阵的每一行数据
    double *elements=new double[observation_num];

    //矩阵只有对角线上有元素，遍历矩阵，将对角线上的元素置为权值
    for(int i=0;i<observation_num;i++)
    {
        for(int k=0;k<observation_num;k++)
        {
            elements[k]=0;//先将每一行上所有元素都置为0
        }
        elements[i]=1/weights[i];//第i行第i个元素是对角线上的元素

        //将矩阵的每一行都写入输出文本中
        for(int k=0;k<observation_num;k++)
        {
            matrix=matrix+QString::number(elements[k],'f',2)+' ';
            //行内每个元素之间间隔一个空格
        }
        matrix=matrix+'\n';
        //每行末尾加上换行符
    }

    //输出文本设置好后销毁存储每一行数据的数组
    delete[] elements;
    elements=NULL;
    return matrix;
    //返回要输出的文本
}

//若当前对象中存储的error变量为1，说明输入数据有误，则也返回1
bool levelingnetwork::judge()
{
    return error;
}
//用于在按钮clicked响应函数中帮助判断是否应该停止后续程序的运行
