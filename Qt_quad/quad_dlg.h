#ifndef QUAD_DLG_H
#define QUAD_DLG_H

#include <QMainWindow>
#include "quad.h"

extern quadList d;

QT_BEGIN_NAMESPACE
namespace Ui {
class quad_dlg;
}
QT_END_NAMESPACE

class quad_dlg : public QMainWindow
{
    Q_OBJECT

public:
    quad_dlg(QWidget *parent = nullptr);
    ~quad_dlg();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::quad_dlg *ui;
};
#endif // QUAD_DLG_H

void warn(QString text);
