#pragma once

#include <vector>
#include <initializer_list>
#include <stdexcept>

namespace sim {
namespace algebra {

template<typename T, int Rows, int Cols>
class Matrix {
    std::vector<T> data;

public:
    Matrix() : data(Rows * Cols) {}

    Matrix(std::initializer_list<T> list) : data(list) {
        if (list.size() != Rows * Cols) {
            throw std::invalid_argument("Initializer list size mismatch");
        }
    }

    T& operator()(int r, int c) {
        return data[r * Cols + c];
    }

    const T& operator()(int r, int c) const {
        return data[r * Cols + c];
    }

    template<int OtherCols>
    Matrix<T, Rows, OtherCols> operator*(const Matrix<T, Cols, OtherCols>& other) const {
        Matrix<T, Rows, OtherCols> result;
        for (int i = 0; i < Rows; ++i) {
            for (int j = 0; j < OtherCols; ++j) {
                T sum = 0;
                for (int k = 0; k < Cols; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    Matrix<T, Rows, Cols> operator+(const Matrix<T, Rows, Cols>& other) const {
        Matrix<T, Rows, Cols> result;
        for (int i = 0; i < Rows * Cols; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }
};

using Vector2d = Matrix<double, 2, 1>;

}
}
