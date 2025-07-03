#ifndef HELPING_H
#define HELPING_H

#include <QWidget>

namespace Ui {
class Helping;
}

class Helping : public QWidget
{
    Q_OBJECT

public:
    explicit Helping(QWidget *parent = nullptr);
    ~Helping();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Helping *ui;
    QString File="";
    QString Calculate="";
    QString Precision="";
    QString Result="";
};

#endif // HELPING_H
