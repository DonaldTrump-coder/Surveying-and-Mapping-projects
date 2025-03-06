#include "draw_dlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    draw_dlg w;
    w.show();
    return a.exec();
}
