#ifndef DRAW_DLG_H
#define DRAW_DLG_H

#include <QMainWindow>
#include "input.h"

extern input a;

QT_BEGIN_NAMESPACE
namespace Ui {
class draw_dlg;
}
QT_END_NAMESPACE

class draw_dlg : public QMainWindow
{
    Q_OBJECT

public:
    draw_dlg(QWidget *parent = nullptr);
    ~draw_dlg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::draw_dlg *ui;
};
#endif // DRAW_DLG_H
