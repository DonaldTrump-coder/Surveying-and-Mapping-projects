#include "quad_dlg.h"
#include "quad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    quad_dlg w;
    w.show();
    return a.exec();
}
