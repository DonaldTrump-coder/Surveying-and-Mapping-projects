#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <QToolBar>
#include <QIcon>
#include <QLabel>
#include <QPointer>
#include "helping.h"
#include "adjustment.h"
#include "graphic_item.h"

namespace Applicationmode
{
    enum mode
{
    Init,
    //程序最初的初始化状态

    Inputed,
    //导入文件之后的状态

    Estimated,
    //概算之后的状态

    Adjusted,
    //平差完成之后的状态

    Precision_got
    //精度评定完成之后的状态
};
}

QT_BEGIN_NAMESPACE
namespace Ui {
class Application;
}
QT_END_NAMESPACE

class Application : public QMainWindow
{
    Q_OBJECT

public:
    Application(QWidget *parent = nullptr);
    ~Application();
    void add_obs_from_text();

private slots:
    //导入观测文件的槽函数
    void FileInput();

    //坐标概算的槽函数
    void Estimation();

    //成果显示的槽函数
    void Display();

    //平差计算的槽函数
    void Adjust();

    //精度评定的槽函数
    void Get_Precision();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    //导出文件的槽函数
    void FileOutput();

    //坐标概算+平差的槽函数
    void calculation();

    //打开帮助页面的槽函数
    void get_helping();

private:
    Ui::Application *ui=nullptr;
    QMenuBar* menubar=nullptr;
    QMenu* menu1=nullptr;
    QMenu* menu2=nullptr;
    QMenu* menu3=nullptr;
    QMenu* menu4=nullptr;
    QAction* action1=nullptr;
    QAction* action2=nullptr;
    QAction* action3=nullptr;
    QAction* action4=nullptr;
    QAction* action5=nullptr;
    QAction* action6=nullptr;
    QAction* action7=nullptr;

    QToolBar* toolbar=nullptr;
    QAction* open_tool=nullptr;
    QAction* save_tool=nullptr;
    QAction* calculate_tool=nullptr;
    QAction* precision_tool=nullptr;
    QAction* display_tool=nullptr;
    QAction* help_tool=nullptr;

    QStatusBar* status_bar=nullptr;
    QLabel* statuslabel=nullptr;
    QPointer<Helping> helping_dialog=nullptr;

    QString filecontext="";
    QString extension_obs="";
    int extension_obs_num=0;

    adjustment adj;

    QGraphicsScene scene;
    graphic_item* item=nullptr;
    Applicationmode::mode applicationmode=Applicationmode::Init;
};
#endif // APPLICATION_H
