/**
 * @file Matrix.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief Matrix
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <bits/stdc++.h>

namespace Tool {

template <typename T = int> // default type is int
class Matrix {
private:
    std::vector<std::vector<T>> Data;
    std::vector<T>              RowCache;

    T      TypeIdentifier;
    size_t SizeOf_Row    = 0;
    size_t SizeOf_Column = 0;

    void buildZeroMat(size_t& row, size_t& column) {
        SizeOf_Row    = row;
        SizeOf_Column = column;
        Data.reserve(row);
        for (int i = 0; i < row; ++i) {
            Data.emplace_back(RowCache);
        }
        for (auto& currRow : Data) {
            currRow.reserve(column);
            for (int i = 0; i < column; ++i) {
                currRow.emplace_back(0);
            }
        }
    }
    bool initMat_check(std::initializer_list<
                       std::initializer_list<T>>& initMat) {
        bool   ifOK       = true;
        size_t tmp_Column = initMat.begin()->size();
        for (auto& initColumn : initMat) {
            if (initColumn.size() != tmp_Column) {
                ifOK = false;
                break;
            }
        }
        return ifOK;
    }
    bool initMat_check(std::vector<
                       std::vector<T>>& initMat) {
        bool   ifOK       = true;
        size_t tmp_Column = initMat.begin()->size();
        for (auto& initColumn : initMat) {
            if (initColumn.size() != tmp_Column) {
                ifOK = false;
                break;
            }
        }
        return ifOK;
    }
    Matrix() = default; // only used while creating a zero mat

public:
    static bool ifEmpty(Matrix& input) {
        return input.SizeOf_Column == 0 || input.SizeOf_Row == 0;
    }
    static bool ifZero(Matrix& input) {
        assert(!ifEmpty(input));
        bool res = true;
        for (auto& currRow : input) {
            for (auto& currElem : currRow) {
                if (currElem == 0) {
                    res = false;
                    break;
                }
            }
        }
        return res;
    }
    static bool if_same_type(Matrix& A, Matrix& B) {
        using TypeA = decltype(A.TypeIdentifier);
        using TypeB = decltype(B.TypeIdentifier);
        return std::is_same<TypeA, TypeB>::value;
    }
    static bool addable(Matrix& A, Matrix& B) {
        bool if_same_row    = A.SizeOf_Row == B.SizeOf_Row;
        bool if_same_column = A.SizeOf_Column == B.SizeOf_Column;
        bool if_same_type   = Matrix::if_same_type(A, B);
        return if_same_column && if_same_row && if_same_type;
    }
    static bool multipliable(Matrix& A, Matrix& B) {
        bool if_col_eq_row = A.SizeOf_Column == B.SizeOf_Row;
        bool if_same_type  = Matrix::if_same_type(A, B);
        return if_col_eq_row && if_same_type;
    }
    static Matrix CreateZeroMat(size_t& row, size_t& column) {
        Matrix ZeroMat;
        ZeroMat.buildZeroMat(row, column);
        return ZeroMat;
    }
    static Matrix A_add_B(Matrix& A, Matrix& B) {
        if (!Matrix::addable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not addable!");
        }
        Matrix<decltype(A.TypeIdentifier)> res = Matrix::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        for (int row = 0; row < A.SizeOf_Row; ++row) {
            for (int col = 0; row < A.SizeOf_Column; ++col) {
                res(row, col) = A(row, col) + B(row, col);
            }
        }
        return res;
    }
    static Matrix A_sub_B(Matrix& A, Matrix& B) {
        if (!Matrix::addable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not addable!");
        }
        Matrix<decltype(A.TypeIdentifier)> res = Matrix::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        for (int row = 0; row < A.SizeOf_Row; ++row) {
            for (int col = 0; row < A.SizeOf_Column; ++col) {
                res(row, col) = A(row, col) - B(row, col);
            }
        }
        return res;
    }
    static Matrix A_multiply_B(Matrix& A, Matrix& B) {
        if (!Matrix::multipliable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not multipliable!");
        }
        Matrix<decltype(A.TypeIdentifier)> res = Matrix::CreateZeroMat(
            A.SizeOf_Row,
            B.SizeOf_Column
        );
        for (int row = 0; row < A.SizeOf_Row; ++row) {
            for (int col = 0; col < B.SizeOf_Column; ++col) {
                for (int k = 0; k < A.SizeOf_Column; ++k) {
                    res(row, col) = A(row, k) * B(k, col);
                }
            }
        }
        return res;
    }

    Matrix(std::initializer_list<
           std::initializer_list<T>>&& initMat) {
        // 1. assertion
        assert(initMat.size() != 0);
        assert(initMat_check(initMat));
        // 2. init zero matrix
        SizeOf_Row    = initMat.size();
        SizeOf_Column = initMat.begin()->size();
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        unsigned short currRow = 0;
        for (auto& initRow : initMat) {
            unsigned short currCol = 0;
            for (auto& initNum : initRow) {
                Data[currRow][currCol] = initNum;
                ++currCol;
            }
            ++currRow;
        }
    }
    explicit Matrix(std::vector<
                    std::vector<T>>& initMat) {
        // 1. assertion
        assert(initMat.size() != 0);
        assert(initMat_check(initMat));
        // 2. init zero matrix
        SizeOf_Row    = initMat.size();
        SizeOf_Column = initMat.begin()->size();
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        unsigned short currRow = 0;
        for (auto& initRow : initMat) {
            unsigned short currCol = 0;
            for (auto& initNum : initRow) {
                Data[currRow][currCol] = initNum;
                ++currCol;
            }
            ++currRow;
        }
    }
    ~Matrix() = default;

    T& operator()(const size_t& row, const size_t& col) {
        if (row > SizeOf_Row || col > SizeOf_Column) {
            throw std::out_of_range("input {row} or {col} is out of range!");
        }
        if (Matrix::ifEmpty(*this)) {
            throw std::logic_error("{this} matrix is empty!");
        }
        return Data[row][col];
    }
    friend Matrix operator+(Matrix& A, Matrix& B) {
        return Matrix::A_add_B(A, B);
    }
    friend Matrix operator-(Matrix& A, Matrix& B) {
        return Matrix::A_sub_B(A, B);
    }
    friend Matrix operator*(Matrix& A, Matrix& B) {
        return Matrix::A_multiply_B(A, B);
    }
};

} // namespace Tool
