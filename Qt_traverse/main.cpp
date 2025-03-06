#include "traverse_dlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    traverse_dlg w;
    w.show();
    return a.exec();
}
