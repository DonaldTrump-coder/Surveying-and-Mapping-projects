#include "adjustment.h"
#include <QStringList>
#include <QDebug>
#include "tools.h"
#include <cmath>

adjustment::adjustment()
{

}

//平差计算类内存管理
adjustment::~adjustment()
{
    delete[] control_points;
    delete[] unknown_points;
    delete[] line_observations;
    delete[] angle_observations;
    delete[] post_angle_observations;
    delete[] coarsepoints;
    delete[] coarselines;
    control_points=nullptr;
    unknown_points=nullptr;
    line_observations=nullptr;
    angle_observations=nullptr;
    post_angle_observations=nullptr;
    coarsepoints=nullptr;
    coarselines=nullptr;
}

//类中的变量初始化，将动态内存全部清除
void adjustment::init()
{
    //删除动态数组，指针置为空
    delete[] control_points;
    delete[] unknown_points;
    delete[] line_observations;
    delete[] angle_observations;
    delete[] post_angle_observations;
    delete[] coarsepoints;
    delete[] coarselines;

    control_points=nullptr;
    unknown_points=nullptr;
    line_observations=nullptr;
    angle_observations=nullptr;
    post_angle_observations=nullptr;
    coarsepoints=nullptr;
    coarselines=nullptr;

    //相关的点数都置为0
    control_point_num=0;
    unknown_point_num=0;
    coarse_point_num=0;
    line_obs_num=0;
    angle_obs_num=0;
    angle_num=0;
    base_num=0;
}

//解译文本
void adjustment::get_data(QString text)
{
    //默认：控制边都能找到、数据没问题
    b_control_points_wrong=0;
    b_data_wrong=0;

    //判断数据类型是否出错的变量
    bool ok;

    //分割所有文本
    QStringList textlist=text.split('\n');

    //行索引
    int lineindex=0;

    //多次判断是否越界，越界就是观测数据不完整
    if(lineindex>=textlist.length())
    {
        warning("观测数据不完整！");
        b_data_wrong=1;
        return;
    }

    //获得控制点数
    control_point_num=textlist[lineindex].toInt(&ok);

    //控制点数必须是整数
    if(!ok)
    {
        warning("请输入正确的控制点数！");
        b_data_wrong=1;
        return;
    }

    //创建控制点数组
    control_points=new point[control_point_num];

    lineindex++;

    //读取控制点坐标数据
    for(int i=0;i<control_point_num;i++)
    {
        //遍历每一行控制点数据
        //判断是否越界
        if(lineindex>=textlist.length())
        {
            warning("观测数据不完整！");
            b_data_wrong=1;
            return;
        }

        //判断控制点数据是否是指定格式
        QStringList controlpoint=textlist[lineindex].split(',');
        if(controlpoint.length()!=3)
        {
            warning("控制点数据格式或个数不正确！");
            b_data_wrong=1;
            return;
        }

        //为每个控制点的点名、x、y赋值
        control_points[i].name=controlpoint[0];
        control_points[i].x=controlpoint[1].toDouble(&ok);
        if(!ok)
        {
            //x、y不能是字符
            warning("请检查第"+QString::number(i)+"个控制点的x坐标！");
            b_data_wrong=1;
            return;
        }
        control_points[i].y=controlpoint[2].toDouble(&ok);
        if(!ok)
        {
            warning("请检查第"+QString::number(i)+"个控制点的y坐标！");
            b_data_wrong=1;
            return;
        }
        lineindex++;
    }

    //判断是否越界
    if(lineindex>=textlist.length())
    {
        warning("观测数据不完整！");
        b_data_wrong=1;
        return;
    }
    //判断是否有多出来的控制点
    if(textlist[lineindex].split(',').length()==3)
    {
        warning("控制点个数不正确！");
        b_data_wrong=1;
        return;
    }

    //读取待求点个数
    unknown_point_num=textlist[lineindex].toInt(&ok);
    if(!ok)
    {
        //待求点个数必须是整数
        warning("请输入正确的待求点数！");
        b_data_wrong=1;
        return;
    }

    //为待求点创建动态数组
    unknown_points=new point[unknown_point_num];
    lineindex++;

    //读取待求点点名
    if(lineindex>=textlist.length())
    {
        warning("观测数据不完整！");
        b_data_wrong=1;
        return;
    }
    QStringList unknownpointnames=textlist[lineindex].split(',');
    if(unknownpointnames.length()!=unknown_point_num)
    {
        //检查待求点个数是否对的上
        warning("待求点个数不正确！");
        b_data_wrong=1;
        return;
    }

    //为待求点点名赋值
    for(int i=0;i<unknown_point_num;i++)
    {
        unknown_points[i].name=unknownpointnames[i];
    }
    lineindex++;

    if(lineindex>=textlist.length())
    {
        warning("观测数据不完整！");
        b_data_wrong=1;
        return;
    }

    //读取边长观测个数
    line_obs_num=textlist[lineindex].toInt(&ok);
    if(!ok)
    {
        //边长观测个数必须是整数
        warning("请输入正确的边长观测数！");
        b_data_wrong=1;
        return;
    }

    //建立边长观测值数组
    line_observations=new line_obs[line_obs_num+1];
    lineindex++;

    //为边长观测数组赋值，建立边长观测的数组
    for(int i=0;i<line_obs_num;i++)
    {
        if(lineindex>=textlist.length())
        {
            warning("观测数据不完整！");
            b_data_wrong=1;
            return;
        }

        //取出边长观测的各元素
        QStringList line_obs_text=textlist[lineindex].split(',');
        if(line_obs_text.length()!=3)
        {
            //确保观测值的格式正确
            warning("请输入正确的边长观测数据格式或个数！");
            b_data_wrong=1;
            return;
        }

        //读取观测值的起点、终点、边长
        line_observations[i+1].start=line_obs_text[0];
        line_observations[i+1].end=line_obs_text[1];
        line_observations[i+1].length=line_obs_text[2].toDouble(&ok);
        if(!ok)
        {
            //边长观测值不能是字符
            warning("请检查第"+QString::number(i+1)+"个边长观测数据的观测值！");
            b_data_wrong=1;
            return;
        }
        if(line_observations[i+1].length<=0)
        {
            //边长观测值必须大于0
            warning("请检查第"+QString::number(i+1)+"个边长观测数据的观测值！");
            b_data_wrong=1;
            return;
        }
        lineindex++;
    }
    if(lineindex>=textlist.length())
    {
        warning("观测数据不完整！");
        b_data_wrong=1;
        return;
    }

    //检查边长观测值个数是否多余
    if(textlist[lineindex].split(',').length()==3)
    {
        warning("请输入正确的边长观测数据个数！");
        b_data_wrong=1;
        return;
    }

    //读入方向观测个数
    angle_obs_num=textlist[lineindex].toInt(&ok);
    if(!ok)
    {
        //方向观测个数必须是整数
        warning("请输入正确的方位观测数据个数！");
        b_data_wrong=1;
        return;
    }
    angle_observations=new ang_obs_data[angle_obs_num];
    lineindex++;

    //读入方向观测值
    for(int i=0;i<angle_obs_num;i++)
    {
        //判断是否越界
        if(lineindex>=textlist.length())
        {
            warning("请输入正确的方位观测数据个数！");
            b_data_wrong=1;
            return;
        }

        //拆分方向观测值的各元素
        QStringList angle_obs_text=textlist[lineindex].split(',');
        if(angle_obs_text.length()!=3)
        {
            warning("请输入正确的方位观测数据格式或个数！");
            b_data_wrong=1;
            return;
        }

        //将方向观测的起点、终点、角度进行赋值
        angle_observations[i].start=angle_obs_text[0];
        angle_observations[i].end=angle_obs_text[1];
        angle_observations[i].angle=angle_obs_text[2].toDouble(&ok);
        if(!ok)
        {
            //方向观测数据不能是字符类型
            warning("请检查第"+QString::number(i+1)+"个方位观测数据的观测值！");
            b_data_wrong=1;
            return;
        }
        lineindex++;
    }

    //检查后面是否还有数据多余（空行不算多余）
    if(lineindex<textlist.length()&&textlist[lineindex]!="")
    {
        warning("数据多余，请检查！");
        b_data_wrong=1;
        return;
    }

    //为边长观测值补充控制点组成的边（用来充当起算数据）
    //便于后续搜索
    for(int i=0;i<angle_obs_num;i++)
    {
        //只有包含在方向观测中的控制边才能作为计算数据使用，因此
        //先遍历所有的方向观测值，搜索包含了两个控制点的方向观测值
        for(int j=0;j<control_point_num;j++)
        {
            //只有某一方向观测值包含了至少一个控制点时才开始搜索
            //分两种情况进行匹配
            if(control_points[j].name==angle_observations[i].start)
            {
                for(int k=0;k<control_point_num;k++)
                {
                    //如果另一控制点能匹配上，就记录控制点的索引，将控制点及其距离放入观测数组的首位
                    if(control_points[k].name==angle_observations[i].end)
                    {
                        control_point_index1=j;
                        control_point_index2=k;
                        line_observations[0].start=control_points[j].name;
                        line_observations[0].end=control_points[k].name;
                        line_observations[0].length=get_length(control_points[j].x,
                                                                 control_points[j].y,
                                                                 control_points[k].x,
                                                                 control_points[k].y);

                        //匹配上之后，完成操作就可以结束该程序，无需再进行额外的控制点匹配了
                        return;
                    }
                }
            }
            else if(control_points[j].name==angle_observations[i].end)
            {
                for(int k=0;k<control_point_num;k++)
                {
                    if(control_points[k].name==angle_observations[i].start)
                    {
                        control_point_index1=k;
                        control_point_index2=j;
                        line_observations[0].end=control_points[j].name;
                        line_observations[0].start=control_points[k].name;
                        line_observations[0].length=get_length(control_points[j].x,
                                                                 control_points[j].y,
                                                                 control_points[k].x,
                                                                 control_points[k].y);
                        return;
                    }
                }
            }
        }
    }

    //搜索完控制点之后，若还没退出函数，说明没匹配上，需要弹出警告且不可进行概算
    warning("未找到合适的控制边！");
    b_control_points_wrong=1;
}

//将方向观测值处理为角度观测值
void adjustment::get_ang_obs()
{
    //重新处理之前先删除之前的动态内存
    delete post_angle_observations;

    //测站数，从1开始计数，方向观测数据的第一条就是第一个测站
    base_num=1;

    //记录测站点号，用于匹配
    QString base=angle_observations[0].start;
    for(int i=0;i<angle_obs_num;i++)
    {
        //在后续的逐点比对过程中，若起始观测点与之前的测站不一样，
        //说明换了新测站，此时增加测站数，将测站点号进行更新
        if(angle_observations[i].start!=base)
        {
            base_num++;
            base=angle_observations[i].start;
        }
    }

    //角度个数等于总观测数减去测站数
    angle_num=angle_obs_num-base_num;

    //创建角度的数组
    post_angle_observations=new ang_obs[angle_num];
    int angleindex=0;//角度的索引

    //每个角度的起始边都应该是测站基准边
    //base是测站（观测起始点），object是观测终止点
    base=angle_observations[0].start;
    QString object=angle_observations[0].end;

    //遍历每一个方向观测值
    for(int i=1;i<angle_obs_num;i++)
    {
        //如果方向观测值的起点与当前测站匹配得上，说明不是起始边，可直接得出终止边和角度
        if(angle_observations[i].start==base)
        {
            post_angle_observations[angleindex].start=get_line(base,
                                                                 object);
            post_angle_observations[angleindex].end=get_line(base,
                                                               angle_observations[i].end);

            //此处的角度用弧度表示，方便计算
            post_angle_observations[angleindex].angle=dms2rad(angle_observations[i].angle);

            angleindex++;
        }

        //如果方向观测值得起点与当前测站不能匹配上
        else
        {
            base=angle_observations[i].start;
            object=angle_observations[i].end;
        }
    }
}

//给定端点名，得到观测边的指针
line_obs* adjustment::get_line(QString start,QString end)
{
    //遍历所有观测边，可以是起点和终点匹配，也可以是起点和起点匹配
    for(int i=0;i<line_obs_num+1;i++)
    {
        if(line_observations[i].start==start && line_observations[i].end==end)
            return &line_observations[i];
        if(line_observations[i].start==end && line_observations[i].end==start)
            return &line_observations[i];
    }

    //如果没搜索到，返回空指针
    return nullptr;
}

//给定端点名，得到概算边指针，与前面类似
line_coarse* adjustment::get_coarse_line(QString start,QString end)
{
    for(int i=0;i<line_obs_num+1;i++)
    {
        if(coarselines[i].start==start && coarselines[i].end==end)
            return &coarselines[i];
        if(coarselines[i].start==end && coarselines[i].end==start)
            return &coarselines[i];
    }
    return nullptr;
}

//给定点坐标，获得方位角
double adjustment::get_orientation(double x1,
                                   double y1,
                                   double x2,
                                   double y2)
{
    double dx=x2-x1;
    double dy=y2-y1;
    double rad=std::atan(dabs(dy)/dabs(dx));
    if(dx>0&&dy>=0)
        return rad;
    else if(dx<0&&dy>=0)
        return pi-rad;
    else if(dx<0&&dy<0)
        return pi+rad;
    else if(dx>0&&dy<0)
        return 2*pi-rad;
    else if(dx==0&&dy>0)
        return pi/2;
    else if(dx==0&&dy<0)
        return pi*3/2;
    else
    {
        warning("方位角计算可能存在问题！请不要再进行平差了！");
        b_wrong=1;
        return 0;
    }
}

//给定点坐标，计算两点距离
double adjustment::get_length(double x1,
                              double y1,
                              double x2,
                              double y2)
{
    double dx=x2-x1;
    double dy=y2-y1;
    return sqrt(dx*dx+dy*dy);
}

//根据指定点名搜索得到概算点指针
coarse_point* adjustment::get_coarse_point_with_name(QString name)
{
    for(int i=0;i<coarse_point_num;i++)
    {
        if(coarsepoints[i].name==name)
            //返回概算点地址
            return &coarsepoints[i];
    }

    //如果没搜索到，则返回空指针
    return nullptr;
}

//在已知概算边方位角和其中一个概算点的情况下
//将指定概算边中的概算点都置为已知，并为其赋坐标值
void adjustment::set_points_known(line_coarse* line)
{
    //先搜索得到概算边对应的概算点
    QString start=line->start;
    QString end=line->end;
    coarse_point* start_point=get_coarse_point_with_name(start);
    coarse_point* end_point=get_coarse_point_with_name(end);

    //计算xy方向边长
    double dx=line->length*cos(line->orientation);
    double dy=line->length*sin(line->orientation);
    if(start_point->known==0&&end_point->known==1)
    {
        //已知方位角、已知终点，计算起点
        start_point->x=end_point->x-dx;
        start_point->y=end_point->y-dy;
        start_point->known=1;
    }
    else if(start_point->known==1&&end_point->known==0)
    {
        //已知方位角、已知起点，计算终点
        end_point->x=start_point->x+dx;
        end_point->y=start_point->y+dy;
        end_point->known=1;
    }
}

//进一步封装，用给定的端点名称搜索并得到边的方位角
//边从start指向end
double adjustment::get_coarse_orientation_with_names(QString start,
                                                     QString end)
{
    double startx=0;
    double starty=0;
    double endx=0;
    double endy=0;

    //遍历并为xy赋值
    for(int i=0;i<coarse_point_num;i++)
    {
        if(coarsepoints[i].name==start)
        {
            startx=coarsepoints[i].x;
            starty=coarsepoints[i].y;
        }
        else if(coarsepoints[i].name==end)
        {
            endx=coarsepoints[i].x;
            endy=coarsepoints[i].y;
        }
    }

    //调取计算方位角的函数
    return get_orientation(startx,
                           starty,
                           endx,
                           endy);
}

//用给定的端点名称搜索并得到边的长度
double adjustment::get_coarse_length_with_names(QString start,
                                                QString end)
{
    double startx=0;
    double starty=0;
    double endx=0;
    double endy=0;
    for(int i=0;i<coarse_point_num;i++)
    {
        if(coarsepoints[i].name==start)
        {
            startx=coarsepoints[i].x;
            starty=coarsepoints[i].y;
        }
        else if(coarsepoints[i].name==end)
        {
            endx=coarsepoints[i].x;
            endy=coarsepoints[i].y;
        }
    }
    return get_length(startx,
                      starty,
                      endx,
                      endy);
}

//根据指定点名得到概算点的点号
int adjustment::get_coarse_point_index(QString name)
{
    for(int i=0;i<coarse_point_num;i++)
    {
        if(coarsepoints[i].name==name)
            return i-control_point_num;
    }
    return -1;
}

//根据给定点名判断该点是否为控制点
bool adjustment::is_control_point(QString name)
{
    for(int i=0;i<control_point_num;i++)
    {
        if(control_points[i].name==name)
            return true;
    }

    //如果不是控制点则返回false
    return false;
}

//平差计算
void adjustment::adjust_calc()
{
    //定权
    Matrix a(line_obs_num+angle_obs_num,
             line_obs_num+angle_obs_num);
    P=a;
    for(int i=0;i<angle_obs_num;i++)
    {
        P.SetMatrix_ele(i,i,1);
    }
    for(int i=angle_obs_num;i<line_obs_num+angle_obs_num;i++)
    {
        P.SetMatrix_ele(i,
                        i,
                        100/(line_observations[i-angle_obs_num+1].length));
    }

    //迭代次数
    int times=0;
    while(1)
    {
        if(times>=3)//三次迭代平差
            break;

        //构建参数矩阵
        construct_matrics();

        //计算参数改正数
        deltaX=(A.trans()*P*A).inv()*(A.trans())*P*L;

        //用改正数更新参数
        update();

        //迭代次数+1
        times++;
    }
    //完成迭代后再次构建参数矩阵
    construct_matrics();

    //计算观测值改正数
    v=(A*deltaX)-L;

    //将0-2pi之外的观测值改正数置于0-2pi之间
    for(int i=0;i<angle_obs_num;i++)
    {
        v.SetMatrix_ele(i,0,sec_abs(v.getMatrix_ele(i,0)));
    }
}

//用改正数更新参数中的坐标值
void adjustment::update()
{
    //更新x和y坐标
    double dx;
    double dy;
    for(int i=base_num;i<base_num+unknown_point_num;i++)
    {
        //改正数单位转为m
        dx=deltaX.getMatrix_ele(i,0)/1000;
        dy=deltaX.getMatrix_ele(i+unknown_point_num,0)/1000;
        coarsepoints[i-base_num+2].x+=dx;
        coarsepoints[i-base_num+2].y+=dy;
    }
}

//精度评定
void adjustment::get_precision()
{
    //计算单位权中误差
    m0=sqrt(((v.trans()*P*v).getMatrix_ele(0,0))/(line_obs_num+angle_obs_num-(base_num+2*unknown_point_num)));

    //计算参数协因数矩阵
    Qx=(A.trans()*P*A).inv();

    //观测值协因数矩阵
    Qf=A*Qx*(A.trans());

    //观测值协方差矩阵
    Mf=Qf;
    for(int i=0;i<line_obs_num+angle_obs_num;i++)
    {
        for(int j=0;j<line_obs_num+angle_obs_num;j++)
        {
            Mf.SetMatrix_ele(i,
                             j,
                             m0*m0*Qf.getMatrix_ele(i,j));
        }
    }

    Matrix result1(unknown_point_num,1);
    Matrix result2(unknown_point_num,1);
    Matrix result3(unknown_point_num,1);
    Matrix result4(unknown_point_num,1);
    Matrix result5(unknown_point_num,1);
    Matrix result6(unknown_point_num,1);

    //点位误差
    double mx,my,mk;

    //协因数元素
    double Qxx,Qxy,Qyy;

    //误差椭圆长轴方位、长轴长度、短轴长度
    double ang,elong,eshort;

    //逐点处理，将数值放入对应的矩阵位置中
    for(int i=0;i<unknown_point_num;i++)
    {
        mx=m0*sqrt(Qx.getMatrix_ele(i+base_num,i+base_num));
        my=m0*sqrt(Qx.getMatrix_ele(i+base_num+unknown_point_num,
                                        i+base_num+unknown_point_num));
        mk=m0*sqrt(mx*mx+my*my);
        result1.SetMatrix_ele(i,0,mx);
        result2.SetMatrix_ele(i,0,my);
        result3.SetMatrix_ele(i,0,mk);

        Qxx=Qx.getMatrix_ele(i+base_num,i+base_num);
        Qyy=Qx.getMatrix_ele(i+base_num+unknown_point_num,
                               i+base_num+unknown_point_num);
        Qxy=Qx.getMatrix_ele(i+base_num,
                               i+base_num+unknown_point_num);
        ang=0.5*atan((2*Qxy)/(Qxx-Qyy));
        elong=m0*sqrt(Qxx+Qxy*tan(ang));
        eshort=m0*sqrt(Qxx+Qxy*tan(ang+0.5*pi));

        //当计算得到的长轴比短轴短时，交换长短轴长度，长轴方位角变化90度
        if(elong<eshort)
        {
            double temp=elong;
            elong=eshort;
            eshort=temp;
            if(ang<0)
                ang+=0.5*pi;
            else
                ang-=0.5*pi;
        }

        result4.SetMatrix_ele(i,0,ang);
        result5.SetMatrix_ele(i,0,elong);
        result6.SetMatrix_ele(i,0,eshort);
    }

    //元素赋给类中矩阵
    Mx=result1;
    My=result2;
    Mk=result3;
    Q=result4;
    E_long=result5;
    E_short=result6;
}

//构建输出文本
void adjustment::output_text()
{
    //先清空文本内容
    result.clear();

    //逐个报表进行构建
    result+="\\\\方向观测成果\n";
    result+="测站,照准,方向值(dms),改正数(s),平差后值(dms),备注\n";
    for(int i=0;i<angle_obs_num;i++)
    {
        result+=angle_observations[i].start+','+angle_observations[i].end+',';
        result+=QString::number(angle_observations[i].angle,'f',6);
        if(angle_observations[i].angle<0.0001&&angle_observations[i].angle>-0.0001)
        {
            result+='\n';
            continue;
        }
        else
        {
            result+=','+QString::number(v.getMatrix_ele(i,0),'f',2)+',';
            result+=QString::number(rad2dms(v.getMatrix_ele(i,0)/rou+dms2rad(angle_observations[i].angle)),
                                      'f',
                                      6)+'\n';
        }
    }
    result+="\\\\距离观测成果\n";
    result+="测站,照准,距离(m),改正数(m),平差后值(m),方位角(dms)\n";
    for(int i=1;i<line_obs_num+1;i++)
    {
        result+=line_observations[i].start+','+line_observations[i].end+',';
        result+=QString::number(line_observations[i].length,
                                  'f',
                                  6)+',';
        result+=QString::number(v.getMatrix_ele(i-1+angle_obs_num,0)/1000,
                                  'f',
                                  6)+',';
        result+=QString::number(v.getMatrix_ele(i-1+angle_obs_num,0)/1000+line_observations[i].length,
                                  'f',
                                  6)+',';
        result+=QString::number(rad2dms(get_coarse_orientation_with_names(line_observations[i].start,
                                                                            line_observations[i].end)),
                                  'f',
                                  6);
        result+='\n';
    }
    result+="\\\\平面点位误差\n";
    result+="点名,长半轴(m),短半轴(m),长轴方位(dms),点位中误差(m),备注\n";
    for(int i=0;i<unknown_point_num;i++)
    {
        result+=coarsepoints[i+2].name+',';
        result+=QString::number(E_long.getMatrix_ele(i,0)/1000,
                                  'f',
                                  4)+',';
        result+=QString::number(E_short.getMatrix_ele(i,0)/1000,
                                  'f',
                                  4)+',';

        double angle=Q.getMatrix_ele(i,0);
        if(angle<0)
            angle+=pi;
        result+=QString::number(rad2dms(angle),
                                  'f',
                                  6)+',';

        result+=QString::number(Mk.getMatrix_ele(i,0)/1000,
                                  'f',
                                  4)+'\n';
    }
    result+="\\\\控制点成果\n";
    result+="点名,X(m),Y(m),H(m),备注\n";
    for(int i=0;i<control_point_num+unknown_point_num;i++)
    {
        result+=coarsepoints[i].name+',';
        result+=QString::number(coarsepoints[i].x,
                                  'f',
                                  6)+',';
        result+=QString::number(coarsepoints[i].y,
                                  'f',
                                  6)+','+',';
        if(i<control_point_num)
            result+="已知点";
        result+='\n';
    }
}

//接口函数，访问类中元素
QString adjustment::get_result()
{
    return result;
}

coarse_point* adjustment::get_coarse_points()
{
    return coarsepoints;
}

int adjustment::get_coarse_points_num()
{
    return control_point_num+unknown_point_num;
}

int adjustment::get_control_points_num()
{
    return control_point_num;
}

int adjustment::get_unknown_points_num()
{
    return unknown_point_num;
}

int adjustment::get_line_obs_num()
{
    return line_obs_num;
}

line_obs* adjustment::get_line_observations()
{
    return line_observations;
}

int adjustment::get_angle_obs_num()
{
    return angle_obs_num;
}

ang_obs_data* adjustment::get_ang_obs_data()
{
    return angle_observations;
}

Matrix* adjustment::get_Q()
{
    return &Q;
}

Matrix* adjustment::get_E_long()
{
    return &E_long;
}

Matrix* adjustment::get_E_short()
{
    return &E_short;
}

//添加未知点点名
void adjustment::add_new_unknown_points(QString name)
{
    //如果是控制点或待求点中的已有点，则不进行添加
    for(int i=0;i<control_point_num;i++)
    {
        if(control_points[i].name==name)
            return;
    }
    for(int i=0;i<unknown_point_num;i++)
    {
        if(unknown_points[i].name==name)
            return;
    }

    //待求点个数+1
    unknown_point_num++;

    //建立新的动态数组
    point* temp=new point[unknown_point_num];

    //相当于数组的复制
    for(int i=0;i<unknown_point_num-1;i++)
    {
        temp[i].name=unknown_points[i].name;
    }
    temp[unknown_point_num-1].name=name;

    //删除原有的数组
    delete[] unknown_points;
    unknown_points=temp;
}

void adjustment::add_new_line_obs(QString start,
                                  QString end,
                                  double length)
{
    line_obs_num++;
    line_obs* temp=new line_obs[line_obs_num+1];
    for(int i=0;i<line_obs_num;i++)
    {
        temp[i].start=line_observations[i].start;
        temp[i].end=line_observations[i].end;
        temp[i].length=line_observations[i].length;
    }
    temp[line_obs_num].start=start;
    temp[line_obs_num].end=end;
    temp[line_obs_num].length=length;
    delete[] line_observations;
    line_observations=temp;
}

void adjustment::add_new_angle_obs(QString start,
                                   QString end,
                                   double angle)
{
    angle_obs_num++;
    ang_obs_data* temp=new ang_obs_data[angle_obs_num];
    for(int i=0;i<angle_obs_num-1;i++)
    {
        temp[i].start=angle_observations[i].start;
        temp[i].end=angle_observations[i].end;
        temp[i].angle=angle_observations[i].angle;
    }
    temp[angle_obs_num-1].start=start;
    temp[angle_obs_num-1].end=end;
    temp[angle_obs_num-1].angle=angle;
    delete[] angle_observations;
    angle_observations=temp;
}

bool adjustment::get_control_points_wrong()
{
    return b_control_points_wrong;
}

bool adjustment::get_wrong()
{
    return b_wrong;
}

bool adjustment::get_data_wrong()
{
    return b_data_wrong;
}
