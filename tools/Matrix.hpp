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

    size_t SizeOf_Row    = 0;
    size_t SizeOf_Column = 0;

    void buildZeroMat(const size_t& row, const size_t& column) {
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
    static bool ifEmpty(const Matrix& input) {
        return input.SizeOf_Column == 0 || input.SizeOf_Row == 0;
    }
    static bool ifZero(const Matrix& input) {
    }
    static bool addable_with(const Matrix& A, const Matrix& B) {
        bool if_same_row    = A.SizeOf_Row == B.SizeOf_Row;
        bool if_same_column = A.SizeOf_Column == B.SizeOf_Column;
        return if_same_column && if_same_row;
    }
    static bool multipliable_with(const Matrix& A, const Matrix& B) {
        bool if_col_eq_row = A.SizeOf_Column == B.SizeOf_Row;
        return if_col_eq_row;
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

    static Matrix& CreateZeroMat(const size_t& row, const size_t& column) {
        Matrix ZeroMat;
        ZeroMat.buildZeroMat(row, column);
        return ZeroMat;
    }
};

} // namespace Tool
