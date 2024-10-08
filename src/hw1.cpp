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
    if (matrix.size()==0)
    {
        return 1;
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
                res[j][i] = ((i+j)%2==0 ? 1 : -1) * determinant(minor(matrix, j, i)); // 隐含transpose
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

Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0)
{
    if (axis == 0)
    {
        if (matrix1.size()!=matrix2.size())
        {
            Incorrespondence e;
            throw e;
        }
        Matrix res = matrix1;
        for (int i = 0; i < res.size(); i++)
        {
            res[i].insert(res[i].end(), matrix2[i].begin(), matrix2[i].end());
        }
        return res;
    }
    else if (axis == 1)
    {
        if (matrix1[0].size()!=matrix2[0].size())
        {
            Incorrespondence e;
            throw e;
        }
        Matrix res = matrix1;
        res.insert(res.end(), matrix2.begin(), matrix2.end());
        return res;
    }
    else
    {
        CallException e;
        throw e;
    }
}

Matrix ero_swap(const Matrix& matrix, int r1, int r2)
{
    Matrix res = matrix;
    res[r1] = matrix[r2];
    res[r2] = matrix[r1];
    return res;
}

Matrix ero_multiply(const Matrix& matrix, int r, double c)
{
    Matrix res = matrix;
    res[r] = multiply(res[r], c);
    return res;
}

Matrix ero_sum(const Matrix& matrix, int r1, double c, int r2)
{
    Matrix res = matrix;
    res[r2] = sum(matrix[r2], multiply(matrix[r1], c));
    return res;
}

Matrix upper_triangular(const Matrix& matrix)
{
    if (matrix.size()!=matrix[0].size())
    {
        NotSquareMatrix e;
        throw e;
    }
    const int ms = matrix.size();
    Matrix res = matrix;
    for (int i = 0; i < ms-1; i++)
    {
        for (int j = 0; j < ms-i-1; j++)
        {
            res = ero_sum(res, ms-i-1, - res[j][ms-i-1]/res[ms-i-1][ms-i-1], j);
        }
    }
    return res;
}

}

using namespace algebra;

int main()
{
    Matrix a = {{2, 1}, {1, 3}, {2, 1}};
    Matrix b = {{1, 3}, {6, 2}};
    Matrix c = {{2, -3, -2}, {1, -1, 1}, {-1, 2, 2}};
    Matrix d = random(5,5,-10,10);
    show(upper_triangular(d));
    std::cout << determinant(d) <<std::endl<<determinant(upper_triangular(d))<<std::endl;
    //std::cout<<determinant(c)<<std::endl;
    return 0;
}
