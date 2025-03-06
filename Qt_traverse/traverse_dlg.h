#ifndef TRAVERSE_DLG_H
#define TRAVERSE_DLG_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class traverse_dlg;
}
QT_END_NAMESPACE

class traverse_dlg : public QMainWindow
{
    Q_OBJECT

public:
    traverse_dlg(QWidget *parent = nullptr);
    ~traverse_dlg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::traverse_dlg *ui;
};
#endif // TRAVERSE_DLG_H
