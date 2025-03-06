#ifndef CASS_DLG_H
#define CASS_DLG_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class cass_dlg;
}
QT_END_NAMESPACE

class cass_dlg : public QMainWindow
{
    Q_OBJECT

public:
    cass_dlg(QWidget *parent = nullptr);
    ~cass_dlg();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::cass_dlg *ui;
};
#endif // CASS_DLG_H
