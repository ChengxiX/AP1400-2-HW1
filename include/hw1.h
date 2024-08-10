#ifndef AP_HW1_H
#define AP_HW1_H


#endif //AP_HW1_H

#include<vector>
namespace algebra
{
    using Vector = std::vector<double>;
    using Matrix = std::vector<Vector>;

    struct AlgebraException : public std::exception {};
    struct Incorrespondence : public AlgebraException {};
    struct MultiplyIncorrespondence : public Incorrespondence {};
    struct SumIncorrespondence : public Incorrespondence {};
    struct NotSquareMatrix : public Incorrespondence {};
    struct SingularMatrix : public AlgebraException {};


    Matrix zeros(int n, int m);
    Matrix ones(int n, int m);
    Matrix random(int n, int m, double min, double max);
    void show(const Matrix& matrix);
    Matrix multiply(const Matrix& matrix, double c);
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2);
    Vector multiply(const Matrix& matrix, const Vector& v);
    Vector multiply(const Vector& vector, double c);
    Matrix sum(const Matrix& matrix, double c);
    Matrix sum(const Matrix& matrix1, const Matrix& matrix2);
    Vector sum(const Vector& vector1, const Vector& vector2);
    Matrix transpose(const Matrix& matrix);
    Matrix minor(const Matrix& matrix, int n, int m);
    double determinant(const Matrix& matrix);
    Matrix inverse(const Matrix& matrix);
}
