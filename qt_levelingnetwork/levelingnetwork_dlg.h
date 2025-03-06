#ifndef LEVELINGNETWORK_DLG_H
#define LEVELINGNETWORK_DLG_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class levelingnetwork_dlg;
}
QT_END_NAMESPACE

class levelingnetwork_dlg : public QMainWindow
{
    Q_OBJECT

public:
    levelingnetwork_dlg(QWidget *parent = nullptr);
    ~levelingnetwork_dlg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::levelingnetwork_dlg *ui;
};
#endif // LEVELINGNETWORK_DLG_H
