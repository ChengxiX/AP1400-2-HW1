#include "hw1.h"

#include<random>
#include<iostream>
#include<iomanip>
namespace algebra
{
Matrix zeros(int n, int m)
{ // n x m matrix
    const Vector column(n, 0);
    Matrix res(m, column);
    return res;
}

Matrix ones(int n, int m)
{ // n x m matrix
    const Vector column(n, 1);
    Matrix res(m, column);
    return res;
}

Matrix random(int n, int m, double min, double max)
{
    const Vector column(n);
    Matrix res(m, column);
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(min, max);
    for (int i = 0; i< m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[i][j] = (distribution(generator));
        }
    }
    return res;
}

void show(const Matrix& matrix)
{
    for (int i = 0; i < matrix[0].size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            std::cout << std::setw(5) << std::setprecision(3) << matrix[j][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Matrix multiply(const Matrix& matrix, double c)
{
    const Vector column(matrix[0].size());
    Matrix res(matrix.size(), column);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            res[i][j] = matrix[i][j] * c;
        }
    }
    return res;
}

Vector multiply(const Matrix& matrix, const Vector& v)
{
    if (matrix.size() != v.size())
    {
        MultiplyIncorrespondence e;
        throw e;
    }
    Vector res(matrix[0].size(), 0);
    for (int i = 0; i < matrix.size(); i++)
    {
        res = sum(res, multiply(matrix[i], v[i]));
    }
    return res;
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.size() != matrix2[0].size())
    {
        MultiplyIncorrespondence e;
        throw e;
    }
    Vector v(matrix1[0].size(), 0);
    Matrix res(matrix2.size(), v);
    for (int i = 0; i < res.size(); i++)
    {
        res[i] = sum(res[i], multiply(matrix1, matrix2[i]));
    }
    return res;
}

Vector multiply(const Vector& vector, double c)
{
    Vector res = vector;
    for (int i = 0; i < vector.size(); i++)
    {
        res[i] *= c;
    }
    return res;
}

Matrix sum(const Matrix& matrix, double c)
{
    Matrix res = matrix;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            res[i][j] += c;
        }
    }
    return res;
}

Matrix sum(const Matrix& matrix1, const Matrix& matrix2)
{
    if ((matrix1.size()!=matrix2.size()) or (matrix1[0].size()!=matrix2[0].size()))
    {
        SumIncorrespondence e;
        throw e;
    }
    Matrix res = matrix1;
    for (int i = 0; i < matrix1.size(); i++)
    {
        for (int j = 0; j < matrix1[0].size(); j++)
        {
            res[i][j] += matrix2[i][j];
        }
    }
    return res;
}

Vector sum(const Vector& vector1, const Vector& vector2)
{
    if (vector1.size()!=vector2.size()){
        SumIncorrespondence e;
        throw e;
    }
    Vector res = vector1;
    for (int i = 0; i < vector1.size(); i++)
    {
        res[i] += vector2[i];
    }
    return res;
}

Matrix transpose(const Matrix& matrix)
{
    Vector v(matrix.size());
    Matrix res(matrix[0].size(), v);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            res[j][i] = matrix[i][j];
        }
    }
    return res;
}

Matrix minor(const Matrix& matrix, int n, int m)
{
    Vector v(matrix[0].size()-1);
    Matrix res(matrix.size()-1, v);
    int flag1 = 0;
    int flag2 = 0;
    for (int i = 0; i < matrix.size()-1; i++)
    {
        if (flag1==0&&i==m)
        {
            flag1 = 1;
            i--;
            continue;
        }
        flag2 = 0;
        for (int j = 0; j < matrix[1].size()-1; j++)
        {
            if (flag2==0&&j==n)
            {
                flag2 = 1;
                j--;
                continue;
            }
            res[i][j] = matrix[i+flag1][j+flag2];
        }
    }
    return res;
}

double determinant(const Matrix& matrix)
{
    if (matrix.size()!=matrix[0].size())
    {
        NotSquareMatrix e;
        throw e;
    }
    if (matrix.size()==2)
    {
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
    }
    if (matrix.size()==1)
    {
        return matrix[0][0];
    }
    double res = 0;
    for (int i = 0; i < matrix.size(); i++)
    {
        // i%2==0 = i+1 + 1 % 2
        res += (i%2==0 ? 1 : -1) * matrix[0][i] * determinant(minor(matrix, i, 0));
    }
    return res;
}

Matrix inverse(const Matrix& matrix)
{
    if (matrix.size()!=matrix[0].size())
    {
        NotSquareMatrix e;
        throw e;
    }
    Matrix res = matrix;
    double det = 0;
    for (int i = 0; i < matrix.size(); i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                res[j][i] = ((i+j)%2==0 ? 1 : -1) * determinant(minor(matrix, j, i));
                det += res[j][i] * matrix[i][j];
            }
        }
        else
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                res[j][i] = ((i+j)%2==0 ? 1 : -1) * determinant(minor(matrix, j, i));
            }
        }
    }
    if (det==0)
    {
        SingularMatrix e;
        throw e;
    }
    return multiply(res, 1/det);
}

}

using namespace algebra;

int main()
{
    //Matrix a = {{2,1 2}, {1, 3}, {2, 1}};
    Matrix b = {{1, 3}, {6, 2}};
    Matrix c = {{1, 3, 9}, {6, 2, 3}, {4, 2, 7}};
    show(inverse(c));
    //std::cout<<determinant(c)<<std::endl;
    return 0;
}
