#include "matrix.h"
#include "tools.h"
#include <qdebug.h>

//根据行和列数初始化一个矩阵
Matrix::Matrix(int rows,int cols)
{
    row=rows;
    col=cols;
    elements=new double*[row];
    for(int i=0;i<row;i++)
    {
        elements[i]=new double[col];
    }

    //参数全部置为0
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            SetMatrix_ele(i,j,0);
        }
    }
}

//不带参数的矩阵初始化
Matrix::Matrix()
{
    row=0;
    col=0;
    elements=nullptr;
}

//矩阵的析构函数
Matrix::~Matrix()
{
    //删除所有动态内存
    for(int i=0;i<row;i++)
    {
        delete[] elements[i];
    }
    delete[] elements;
}

//矩阵复制构造函数
Matrix::Matrix(const Matrix& mat)
{
    //先清除原矩阵的动态内存
    for(int i=0;i<row;i++)
    {
        delete[] elements[i];
    }
    delete[] elements;

    col=mat.col;
    row=mat.row;
    elements=new double*[row];
    for(int i=0;i<row;i++)
    {
        elements[i]=new double[col];
    }

    //矩阵元素复制
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            SetMatrix_ele(i,j,mat.elements[i][j]);
        }
    }
}

//设置矩阵元素的内容
void Matrix::SetMatrix_ele(int row,int col,double val)
{
    elements[row][col]=val;
}

//获取矩阵指定位置的元素
double Matrix::getMatrix_ele(int row,int col) const
{
    return elements[row][col];
}

Matrix Matrix::operator*(const Matrix& mat)
{
    Matrix result(row,mat.col);

    //行与列相乘
    for(int i=0;i<row;i++)
    {
        for(int k=0;k<mat.col;k++)
        {
            double sum=0;
            for(int j=0;j<col;j++)
            {
                sum+=getMatrix_ele(i,j)*mat.getMatrix_ele(j,k);
            }
            result.SetMatrix_ele(i,k,sum);
        }
    }
    return result;
}

void Matrix::operator=(const Matrix& mat)
{
    if(this==&mat)
    {
        return;
    }

    for(int i=0;i<row;i++)
    {
        delete[] elements[i];
    }
    delete[] elements;

    row=mat.row;
    col=mat.col;

    elements=new double*[row];
    for(int i=0;i<row;i++)
    {
        elements[i]=new double[col];
    }

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            SetMatrix_ele(i,j,mat.getMatrix_ele(i,j));
        }
    }
}

Matrix Matrix::operator+(Matrix& mat)
{
    Matrix result(row,col);
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            result.SetMatrix_ele(i,j,getMatrix_ele(i,j)+mat.getMatrix_ele(i,j));
        }
    }
    return result;
}

Matrix Matrix::operator-(Matrix& mat)
{
    Matrix result(row,col);
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            result.SetMatrix_ele(i,j,getMatrix_ele(i,j)-mat.getMatrix_ele(i,j));
        }
    }
    return result;
}

//矩阵求转置，返回转置后的矩阵
Matrix Matrix::trans()
{
    Matrix result(col,row);
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            result.SetMatrix_ele(j,i,getMatrix_ele(i,j));
        }
    }
    return result;
}

void Matrix::line_exc(int a,int b)
{
    double temp;
    for(int j=0;j<col;j++)
    {
        temp=getMatrix_ele(a,j);
        SetMatrix_ele(a,j,getMatrix_ele(b,j));
        SetMatrix_ele(b,j,temp);
    }
}

Matrix Matrix::inv()
{
    //构造单位阵
    Matrix result(row,col);
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(i==j)
                result.SetMatrix_ele(i,j,1);
            else
                result.SetMatrix_ele(i,j,0);
        }
    }

    //找主元
    for(int i=0;i<col;i++)
    {
        int maxindex=i;
        double max=getMatrix_ele(i,i);
        for(int j=i;j<row;j++)
        {
            if(dabs(getMatrix_ele(j,i))>dabs(max))
            {
                max=getMatrix_ele(j,i);
                maxindex=j;
            }
        }
        line_exc(i,maxindex);
        result.line_exc(i,maxindex);
    }

    //变为上三角矩阵
    for(int j=0;j<col;j++)
    {
        for(int i=j+1;i<row;i++)
        {
            if(dabs(getMatrix_ele(i,j))<1e-10)
            {
                SetMatrix_ele(i,j,0);
                continue;
            }
            double multi=-getMatrix_ele(j,j)/getMatrix_ele(i,j);
            for(int k=0;k<col;k++)
            {
                SetMatrix_ele(i,k,getMatrix_ele(i,k)*multi+getMatrix_ele(j,k));
                result.SetMatrix_ele(i,k,result.getMatrix_ele(i,k)*multi+result.getMatrix_ele(j,k));
            }
        }
    }

    //变为对角矩阵
    for(int j=1;j<col;j++)
    {
        for(int i=0;i<j;i++)
        {
            if(dabs(getMatrix_ele(i,j))<1e-10)
            {
                SetMatrix_ele(i,j,0);
                continue;
            }
            double multi=-getMatrix_ele(j,j)/getMatrix_ele(i,j);
            for(int k=0;k<col;k++)
            {
                SetMatrix_ele(i,k,getMatrix_ele(i,k)*multi+getMatrix_ele(j,k));
                result.SetMatrix_ele(i,k,result.getMatrix_ele(i,k)*multi+result.getMatrix_ele(j,k));
            }
        }
    }

    //变为单位阵
    for(int i=0;i<row;i++)
    {
        if(dabs(getMatrix_ele(i,i))<1e-10)
        {
            SetMatrix_ele(i,i,0);
            continue;
        }
        double multi=1/getMatrix_ele(i,i);
        SetMatrix_ele(i,i,getMatrix_ele(i,i)*multi);
        for(int j=0;j<col;j++)
        {
            result.SetMatrix_ele(i,j,result.getMatrix_ele(i,j)*multi);
        }
    }

    return result;
}

void Matrix::print()
{
    for(int i=0;i<row;i++)
    {
        QString line="";
        for(int j=0;j<col;j++)
        {
            line+=QString::number(getMatrix_ele(i,j))+" ";
        }
        qDebug()<<line;
    }
}
