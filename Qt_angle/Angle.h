#ifndef ANGLE_H
#define ANGLE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Angle;
}
QT_END_NAMESPACE

class Angle : public QMainWindow
{
    Q_OBJECT

public:
    Angle(QWidget *parent = nullptr);
    ~Angle();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Angle *ui;
};
#endif // ANGLE_H
