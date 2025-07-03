#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
private:
    int row=0;
    int col=0;
    double** elements=nullptr;
public:
    Matrix(int row,int col);
    Matrix();
    //构造函数

    ~Matrix();
    //析构函数

    Matrix(const Matrix& mat);
    //复制构造函数

    //设置矩阵指定位置的元素
    void SetMatrix_ele(int row,int col,double val);

    //获取矩阵指定位置的元素
    double getMatrix_ele(int row,int col) const;

    //重载矩阵乘法
    Matrix operator*(const Matrix& mat);

    //重载矩阵赋值运算
    void operator=(const Matrix& mat);

    //重载矩阵加减法运算
    Matrix operator+(Matrix& mat);
    Matrix operator-(Matrix& mat);

    //矩阵求转置
    Matrix trans();

    //交换矩阵行
    void line_exc(int a,int b);

    //矩阵求逆
    Matrix inv();

    //打印矩阵
    void print();
};

#endif // MATRIX_H
