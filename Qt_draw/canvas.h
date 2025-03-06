#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "input.h"

namespace Ui {
class canvas;
}

class canvas : public QWidget
{
    Q_OBJECT

public:
    explicit canvas(QWidget *parent = nullptr);
    ~canvas();
    void paintEvent(QPaintEvent *event);//重载paintEvent函数

private:
    Ui::canvas *ui;
};

#endif // CANVAS_H
