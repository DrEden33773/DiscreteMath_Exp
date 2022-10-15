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

#pragma once

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace Tool {

template <typename T>
concept arithmetic
    = std::is_arithmetic<T>::value;

template <typename T>
concept notChar = !(
    std::is_same<T, char>::value
    || std::is_same<T, char*>::value
    || std::is_same<T, char&>::value
    || std::is_same<T, char &&>::value
    || std::is_same<T, const char>::value
    || std::is_same<T, const char*>::value
    || std::is_same<T, const char&>::value
    || std::is_same<T, const char &&>::value
);

template <typename T = int> // default type is int
requires arithmetic<T> && notChar<T>
class Matrix {
    friend class undirected_graph;
    friend class info;

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
        for (auto&& currRow : Data) {
            currRow.reserve(column);
            for (int i = 0; i < column; ++i) {
                currRow.emplace_back(0);
            }
        }
    }
    /// @brief check initMat's size
    /// @param initMat
    /// @return initMat.size() != 0 (bool)
    constexpr bool initMatSize_check(std::initializer_list<
                                     std::initializer_list<T>>& initMat) {
        return initMat.size() != 0;
    }
    constexpr bool initMatSize_check(std::vector<
                                     std::vector<T>>& initMat) {
        return initMat.size() != 0;
    }
    /// @brief check if each row in the initMat has the same size
    /// @param initMat
    /// @return bool
    constexpr bool initMat_check(std::initializer_list<
                                 std::initializer_list<T>>& initMat) {
        bool   ifOK       = true;
        size_t tmp_Column = initMat.begin()->size();
        for (auto&& initColumn : initMat) {
            if (initColumn.size() != tmp_Column) {
                ifOK = false;
                break;
            }
        }
        return ifOK;
    }
    constexpr bool initMat_check(std::vector<
                                 std::vector<T>>& initMat) {
        bool   ifOK       = true;
        size_t tmp_Column = initMat.begin()->size();
        for (auto&& initColumn : initMat) {
            if (initColumn.size() != tmp_Column) {
                ifOK = false;
                break;
            }
        }
        return ifOK;
    }
    Matrix() = default; // only used while creating a zero mat

public:
    static constexpr bool ifEmpty(Matrix& input) {
        return input.SizeOf_Column == 0 || input.SizeOf_Row == 0;
    }
    static constexpr bool ifZero(Matrix& input) {
        static_assert(!ifEmpty(input));
        bool res = true;
        for (auto&& currRow : input) {
            for (auto&& currElem : currRow) {
                if (currElem == 0) {
                    res = false;
                    break;
                }
            }
        }
        return res;
    }
    static constexpr bool if_same_type(Matrix& A, Matrix& B) {
        using TypeA = decltype(A.TypeIdentifier);
        using TypeB = decltype(B.TypeIdentifier);
        return std::is_same<TypeA, TypeB>::value;
    }
    static constexpr bool addable(Matrix& A, Matrix& B) {
        bool if_same_row    = A.SizeOf_Row == B.SizeOf_Row;
        bool if_same_column = A.SizeOf_Column == B.SizeOf_Column;
        bool if_same_type   = Matrix::if_same_type(A, B);
        return if_same_column && if_same_row && if_same_type;
    }
    static constexpr bool multipliable(Matrix& A, Matrix& B) {
        bool if_col_eq_row = A.SizeOf_Column == B.SizeOf_Row;
        bool if_same_type  = Matrix::if_same_type(A, B);
        return if_col_eq_row && if_same_type;
    }
    static constexpr bool subable(Matrix& A, Matrix& B) {
        bool if_addable = addable(A, B);

        using theType              = decltype(A.TypeIdentifier);
        bool if_unsigned_short     = std::is_same<theType, unsigned short>::value;
        bool if_unsigned_int       = std::is_same<theType, unsigned int>::value;
        bool if_unsigned_long      = std::is_same<theType, unsigned long>::value;
        bool if_unsigned_long_long = std::is_same<theType, unsigned long long>::value;

        return if_addable
            && !if_unsigned_short
            && !if_unsigned_int
            && !if_unsigned_long
            && !if_unsigned_long_long;
    }
    static constexpr bool assignable(Matrix& A, Matrix& B) {
        bool if_same_row = A.SizeOf_Row == B.SizeOf_Row;
        bool if_same_col = A.SizeOf_Column == B.SizeOf_Column;
        return if_same_row && if_same_col;
    }

    static Matrix<T> CreateZeroMat(size_t row, size_t column) {
        Matrix<T> ZeroMat;
        ZeroMat.buildZeroMat(row, column);
        return ZeroMat;
    }
    static Matrix<T> CreateIdentityMat(size_t row, size_t column) {
        Matrix<T> IdentityMat;
        IdentityMat.buildZeroMat(row, column);
        for (int row = 1; row <= IdentityMat.SizeOf_Row; ++row) {
            IdentityMat(row, row) = 1;
        }
        return IdentityMat;
    }

    static constexpr auto A_add_B(Matrix& A, Matrix& B)
        -> Matrix<decltype(A.TypeIdentifier)> {
        if (!Matrix::addable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not addable!");
        }
        using resMatType = decltype(A.TypeIdentifier);
        auto res         = Matrix<resMatType>::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        for (int row = 1; row <= A.SizeOf_Row; ++row) {
            for (int col = 1; col <= A.SizeOf_Column; ++col) {
                res(row, col) = A(row, col) + B(row, col);
            }
        }
        return res;
    }
    static constexpr auto A_sub_B(Matrix& A, Matrix& B)
        -> Matrix<decltype(A.TypeIdentifier)> {

        if (!Matrix::subable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not addable!");
        }
        using resMatType = decltype(A.TypeIdentifier);
        auto res         = Matrix<resMatType>::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        for (int row = 1; row <= A.SizeOf_Row; ++row) {
            for (int col = 1; col <= A.SizeOf_Column; ++col) {
                res(row, col) = A(row, col) - B(row, col);
            }
        }
        return res;
    }
    static constexpr auto A_multiply_B(Matrix& A, Matrix& B)
        -> Matrix<decltype(A.TypeIdentifier)> {

        if (!Matrix::multipliable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not multipliable!");
        }
        using resMatType = decltype(A.TypeIdentifier);
        auto res         = Matrix<resMatType>::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );

        // for (int row = 1; row <= A.SizeOf_Row; ++row) { // slow
        //     for (int col = 1; col <= B.SizeOf_Column; ++col) {
        //         for (int cross = 1; cross <= A.SizeOf_Column; ++cross) {
        //             res(row, col) += A(row, cross) * B(cross, col);
        //         }
        //     }
        // }

        for (int row = 1; row <= A.SizeOf_Row; ++row) { // a little bit faster
            for (int cross = 1; cross <= A.SizeOf_Column; ++cross) {
                auto& tmp = A(row, cross);
                for (int col = 1; col <= B.SizeOf_Column; ++col) {
                    res(row, col) += tmp * B(cross, col);
                }
            }
        }
        /// @ref https://zhuanlan.zhihu.com/p/146250334

        return res;
    }
    template <typename I>
    requires arithmetic<I>
    static constexpr auto A_multiply_B(Matrix& A, I B)
        -> Matrix<decltype(B)> {
        assert(!ifEmpty(A));

        auto res = Matrix<I>::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        for (int row = 1; row <= A.SizeOf_Row; ++row) {
            for (int col = 1; col <= A.SizeOf_Column; ++col) {
                res(row, col) = static_cast<I>(A(row, col)) * B;
            }
        }
        return res;
    }
    static constexpr auto A_q_pow_N(Matrix A, size_t N)
        -> Matrix<decltype(A.TypeIdentifier)> { // A should not be changed
        if (!Matrix::multipliable(A, A)) {
            throw std::logic_error("Matrix {A} and {A} is not multipliable!");
        }
        using resMatType = decltype(A.TypeIdentifier);
        auto res         = Matrix<resMatType>::CreateIdentityMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        while (N) {
            if (N & 1) {
                res *= A;
            }
            A *= A;
            N >>= 1;
        }
        return res;
    }
    static constexpr auto A_assigned_by_B(Matrix& A, Matrix& B)
        -> Matrix<decltype(A.TypeIdentifier)> {
        if (!Matrix::assignable(A, B)) {
            throw std::logic_error("Matrix {A} and {B} is not assignable!");
        }
        using resMatType = decltype(A.TypeIdentifier);
        auto res         = Matrix<resMatType>::CreateZeroMat(
            A.SizeOf_Row,
            A.SizeOf_Column
        );
        for (int row = 1; row <= A.SizeOf_Row; ++row) {
            for (int col = 1; col <= A.SizeOf_Column; ++col) {
                A(row, col)   = B(row, col);
                res(row, col) = B(row, col);
            }
        }
        return res;
    }
    static constexpr bool A_eq_B(Matrix& A, Matrix& B) {
        if (!Matrix::assignable(A, B)) {
            // throw std::logic_error("Matrix {A} and {B} is incomparable!");
            return false; // we could still assert that two incomparable matrix is not equal
        }
        for (int row = 1; row <= A.SizeOf_Row; ++row) {
            for (int col = 1; col <= A.SizeOf_Column; ++col) {
                if (A(row, col) != B(row, col)) {
                    return false;
                }
            }
        }
        return true;
    }
    static constexpr bool A_eq_B(Matrix* A, Matrix* B) {
        Matrix<decltype(A->TypeIdentifier)>& MatA = (*A);
        Matrix<decltype(B->TypeIdentifier)>& MatB = (*B);
        if (!Matrix::assignable(MatA, MatB)) {
            return false;
        }
        for (int row = 1; row <= MatA.SizeOf_Row; ++row) {
            for (int col = 1; col <= MatA.SizeOf_Column; ++col) {
                if (MatA(row, col) != MatB(row, col)) {
                    return false;
                }
            }
        }
        return true;
    }
    constexpr auto transposition()
        -> Matrix<decltype(this->TypeIdentifier)> {
        Matrix& toOpt = *this;
        assert(!ifEmpty(toOpt));

        using resMatType = decltype(toOpt.TypeIdentifier);
        auto res         = Matrix<resMatType>::CreateZeroMat(
            toOpt.SizeOf_Column,
            toOpt.SizeOf_Row
        );

        for (int row = 1; row <= toOpt.SizeOf_Row; ++row) {
            for (int col = 1; col <= toOpt.SizeOf_Column; ++col) {
                res(col, row) = toOpt(row, col);
            }
        }
        return res;
    }

    Matrix(std::initializer_list<std::initializer_list<T>>&& initMat) {
        // 1. assertion
        assert(initMatSize_check(initMat));
        assert(initMat_check(initMat));
        // 2. init zero matrix
        SizeOf_Row    = initMat.size();
        SizeOf_Column = initMat.begin()->size();
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        unsigned short currRowNum = 0;
        for (auto&& initRow : initMat) {
            unsigned short currColNum = 0;
            for (auto&& initNum : initRow) {
                Data[currRowNum][currColNum] = initNum;
                ++currColNum;
            }
            ++currRowNum;
        }
    }
    Matrix(std::initializer_list<std::initializer_list<T>>& initMat) {
        // 1. assertion
        assert(initMatSize_check(initMat));
        assert(initMat_check(initMat));
        // 2. init zero matrix
        SizeOf_Row    = initMat.size();
        SizeOf_Column = initMat.begin()->size();
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        unsigned short currRowNum = 0;
        for (auto&& initRow : initMat) {
            unsigned short currColNum = 0;
            for (auto&& initNum : initRow) {
                Data[currRowNum][currColNum] = initNum;
                ++currColNum;
            }
            ++currRowNum;
        }
    }
    explicit Matrix(std::vector<std::vector<T>>& initMat) {
        // 1. assertion
        assert(initMatSize_check(initMat));
        assert(initMat_check(initMat));
        // 2. init zero matrix
        SizeOf_Row    = initMat.size();
        SizeOf_Column = initMat.begin()->size();
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        unsigned short currRowNum = 0;
        for (auto&& initRow : initMat) {
            unsigned short currColNum = 0;
            for (auto&& initNum : initRow) {
                Data[currRowNum][currColNum] = initNum;
                ++currColNum;
            }
            ++currRowNum;
        }
    }
    explicit Matrix(std::vector<std::vector<T>>&& initMat) {
        // 1. assertion
        assert(initMatSize_check(initMat));
        assert(initMat_check(initMat));
        // 2. init zero matrix
        SizeOf_Row    = initMat.size();
        SizeOf_Column = initMat.begin()->size();
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        unsigned short currRowNum = 0;
        for (auto&& initRow : initMat) {
            unsigned short currColNum = 0;
            for (auto&& initNum : initRow) {
                Data[currRowNum][currColNum] = initNum;
                ++currColNum;
            }
            ++currRowNum;
        }
    }
    explicit Matrix(Matrix<T>* initPtr) {
        // 1. assertion
        assert(initPtr != nullptr);          // could dismiss
        assert(initPtr->SizeOf_Row != 0);    // could dismiss
        assert(initPtr->SizeOf_Column != 0); // could dismiss
        // 2. init zero matrix
        SizeOf_Row    = initPtr->SizeOf_Row;
        SizeOf_Column = initPtr->SizeOf_Column;
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        for (int row = 1; row <= SizeOf_Row; ++row) {
            for (int col = 1; col <= SizeOf_Column; ++col) {
                Data[row - 1][col - 1] = (*initPtr)(row, col);
            }
        }
    }
    Matrix(Matrix<T>&& initMat) noexcept {
        // 1. assertion
        assert(initMat.SizeOf_Row != 0);    // could dismiss
        assert(initMat.SizeOf_Column != 0); // could dismiss
        // 2. init zero matrix
        SizeOf_Row    = initMat.SizeOf_Row;
        SizeOf_Column = initMat.SizeOf_Column;
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        for (int row = 1; row <= SizeOf_Row; ++row) {
            for (int col = 1; col <= SizeOf_Column; ++col) {
                Data[row - 1][col - 1] = initMat(row, col);
            }
        }
    }
    Matrix(Matrix<T>& initMat) {
        // 1. assertion
        assert(initMat.SizeOf_Row != 0);    // could dismiss
        assert(initMat.SizeOf_Column != 0); // could dismiss
        // 2. init zero matrix
        SizeOf_Row    = initMat.SizeOf_Row;
        SizeOf_Column = initMat.SizeOf_Column;
        buildZeroMat(SizeOf_Row, SizeOf_Column);
        // 3. write to matrix
        for (int row = 1; row <= SizeOf_Row; ++row) {
            for (int col = 1; col <= SizeOf_Column; ++col) {
                Data[row - 1][col - 1] = initMat(row, col);
            }
        }
    };

    void echo() {
        for (auto& currRow : Data) {
            for (auto& currElem : currRow) {
                std::cout << currElem << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    constexpr T sum() {
        assert(!ifEmpty(*this));
        T res = Data[0][0];
        for (auto& currRow : Data) {
            for (auto& currElem : currRow) {
                res += currElem;
            }
        }
        res -= Data[0][0];
        return res;
    }
    constexpr T sum_of_row(size_t input_row) {
        if (input_row > SizeOf_Row) {
            throw std::out_of_range("input row > SizeOf row");
        }
        T res = Data[0][0];

        size_t input_row_index = input_row - 1;
        for (size_t curr_col_index = 0;
             curr_col_index < SizeOf_Column;
             ++curr_col_index) {
            res += Data[input_row_index][curr_col_index];
        }
        res -= Data[0][0];
        return res;
    }
    constexpr T sum_of_col(size_t input_col) {
        if (input_col > SizeOf_Column) {
            throw std::out_of_range("input col > SizeOf col");
        }
        T res = Data[0][0];

        size_t input_col_index = input_col - 1;
        for (size_t curr_row_index = 0;
             curr_row_index < SizeOf_Row;
             ++curr_row_index) {
            res += Data[curr_row_index][input_col_index];
        }
        res -= Data[0][0];
        return res;
    }
    constexpr size_t get_sizeof_row() {
        return SizeOf_Row;
    }
    constexpr size_t get_sizeof_col() {
        return SizeOf_Column;
    }
    static bool if_have_zero_integer(
        Matrix<int>& input
    ) {
        for (int row = 1; row <= input.SizeOf_Row; ++row) {
            for (int col = 1; col <= input.SizeOf_Column; ++col) {
                if (input(row, col) == 0) {
                    return true;
                }
            }
        }
        return false;
    }
    static bool if_symmetric_of_main_diagonal(
        Matrix& input
    ) {
        if (input.SizeOf_Column == 1 && input.SizeOf_Row == 1) {
            return true;
        }
        for (int row = 1; row <= input.SizeOf_Row; ++row) {
            for (int col = row + 1; col <= input.SizeOf_Column; ++col) {
                if (input(row, col) != input(col, row)) {
                    return false;
                }
            }
        }
        return true;
    }

    ~Matrix() = default;

    /// @brief row, col => start from `1`
    /// @param row
    /// @param col
    /// @return
    constexpr T& operator()(const size_t& row, const size_t& col) {
        if (row > SizeOf_Row
            || col > SizeOf_Column
            || row < 1
            || col < 1) {
            throw std::out_of_range("input {row} or {col} is out of range!");
        }
        if (Matrix::ifEmpty(*this)) {
            throw std::logic_error("{this} matrix is empty!");
        }
        return Data[row - 1][col - 1]; // remember to sub 1
    }
    friend constexpr auto operator+(Matrix<T>& A, Matrix& B) {
        return Matrix::A_add_B(A, B);
    }
    friend constexpr auto operator-(Matrix& A, Matrix& B) {
        return Matrix::A_sub_B(A, B);
    }
    friend constexpr auto operator*(Matrix& A, Matrix& B) {
        return Matrix::A_multiply_B(A, B);
    }
    friend constexpr auto operator*(Matrix& A, arithmetic auto B) {
        return Matrix::A_multiply_B(A, B);
    }
    friend constexpr auto operator^(Matrix& A, size_t N) {
        return Matrix::A_q_pow_N(A, N);
    }
    friend constexpr bool operator==(Matrix& A, Matrix& B) {
        return Matrix::A_eq_B(A, B);
    }
    constexpr auto operator=(Matrix& B) {
        return Matrix::A_assigned_by_B(*this, B);
    }
    friend constexpr auto operator+=(Matrix& A, Matrix& B) {
        auto added = Matrix::A_add_B(A, B);
        return Matrix::A_assigned_by_B(A, added);
    }
    friend constexpr auto operator-=(Matrix& A, Matrix& B) {
        auto subbed = Matrix::A_sub_B(A, B);
        return Matrix::A_assigned_by_B(A, subbed);
    }
    friend constexpr auto operator*=(Matrix& A, Matrix& B) {
        auto multiplied = Matrix::A_multiply_B(A, B);
        return Matrix::A_assigned_by_B(A, multiplied);
    }
    friend constexpr auto operator*=(Matrix& A, arithmetic auto B) {
        auto multiplied = Matrix::A_multiply_B(A, B);
        return Matrix::A_assigned_by_B(A, multiplied);
    }
    friend constexpr auto operator^=(Matrix& A, size_t N) {
        auto powered = Matrix::A_q_pow_N(A, N);
        return Matrix::A_assigned_by_B(A, powered);
    }
};

} // namespace Tool
