/**
 * @file general_graph_tool_set.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief A set tools used in undirected and directed graph
 * @version 0.1
 * @date 2022-10-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "Matrix.hpp"
#include <stdexcept>
#include <unordered_set>
#include <utility>

namespace Tool {

class GeneralGraphToolSet {
protected:
    GeneralGraphToolSet() = default;

    /// @brief @b connectivity
    static bool if_connective(Tool::Matrix<int> inputDataMat) {
        auto num_of_nodes = inputDataMat.get_sizeof_row();
        auto final
            = Tool::Matrix<int>::CreateZeroMat(
                num_of_nodes,
                num_of_nodes
            );

        for (size_t pow_num = 0; // A^0 + A^1 + A^2 + ... + A^(n-1)
             pow_num < num_of_nodes;
             ++pow_num) {
            auto powered = inputDataMat ^ pow_num;
            auto added   = final + powered;
            final        = added;
        }

        if (final.if_have_zero_integer(final)) {
            return false;
        }
        return true;
    }
    static bool if_partial_connective(
        Tool::Matrix<int>&          inputDataMat,
        std::unordered_set<size_t>& ignore_v_set // default => empty list
    ) {
        if (ignore_v_set.empty()) {
            return if_connective(inputDataMat);
        }

        bool res = true;

        size_t  MatSize = inputDataMat.get_sizeof_row();
        size_t& MatRow  = MatSize;
        size_t& MatCol  = MatSize;

        std::vector<std::vector<int>> new_init_list;
        std::vector<int>              new_init_row;
        new_init_list.reserve(MatSize);
        new_init_row.reserve(MatSize);
        for (size_t row = 1; row <= MatRow; ++row) {
            if (ignore_v_set.contains(row)) {
                continue;
            }
            for (size_t col = 1; col <= MatCol; ++col) {
                if (ignore_v_set.contains(col)) {
                    continue;
                }
                auto& curr_elem = inputDataMat(row, col);
                new_init_row.emplace_back(curr_elem);
            }
            new_init_list.emplace_back(new_init_row);
            new_init_row.clear();
        }

        Tool::Matrix<int> new_data_mat(new_init_list);
        res = if_connective(new_data_mat);

        return res;
    }

    /// @brief methods about iterating of vertex and edge

    /**
     * @brief return first iterable vertex
     *
     * @param inputDataMat
     * @param vertex
     * @return size_t @b first_iterable_vertex
     */
    static constexpr size_t return_first_iterable(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex
    ) {
        size_t res        = 0;
        size_t num_of_col = inputDataMat.get_sizeof_col();
        for (size_t col = 1; col <= num_of_col; ++col) {
            if (inputDataMat(vertex, col) != 0) {
                res = col;
                break;
            }
        }
        return res;
    }
    /**
     * @brief cut_an_undirected_edge_of
     *
     * @param inputDataMat
     * @param vertex
     * @param col
     * @return size_t @b subbed_value
     */
    static size_t cut_an_undirected_edge_of(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex,
        size_t             col
    ) {
        size_t num_of_col = inputDataMat.get_sizeof_row();
        if (inputDataMat(vertex, col) == 0) {
            throw std::logic_error("No edge between two vertexes!");
        }
        int subbed_value = 1;
        inputDataMat(vertex, col) -= subbed_value;
        inputDataMat(col, vertex) -= subbed_value;
        return subbed_value;
    }
    /**
     * @brief cut_an_directed_edge_of
     *
     * @param inputDataMat
     * @param vertex
     * @param col
     * @return size_t @b subbed_value
     */
    static size_t cut_an_directed_edge_of(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex,
        size_t             col
    ) {
        size_t num_of_col = inputDataMat.get_sizeof_row();
        if (inputDataMat(vertex, col) == 0) {
            throw std::logic_error("No edge between two vertexes!");
        }
        int subbed_value = 1;
        inputDataMat(vertex, col) -= subbed_value;
        return subbed_value;
    }
    /**
     * @brief add_an_undirected_edge_of
     *
     * @param inputDataMat
     * @param vertex
     * @param col
     * @return size_t @b added_value
     */
    static size_t add_an_undirected_edge_of(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex,
        size_t             col
    ) {
        size_t num_of_col  = inputDataMat.get_sizeof_row();
        int    added_value = 1;
        inputDataMat(vertex, col) += added_value;
        inputDataMat(col, vertex) += added_value;
        return added_value;
    }
    /**
     * @brief add_an_directed_edge_of
     *
     * @param inputDataMat
     * @param vertex
     * @param col
     * @return size_t @b added_value
     */
    static size_t add_an_directed_edge_of(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex,
        size_t             col
    ) {
        size_t num_of_col  = inputDataMat.get_sizeof_row();
        int    added_value = 1;
        inputDataMat(vertex, col) += added_value;
        return added_value;
    }
    /**
     * @brief cut_first_iterable_undirected_edge_of
     *
     * @param inputDataMat
     * @param vertex
     * @return size_t @b first_iterable_vertex
     */
    static size_t cut_first_iterable_undirected_edge_of(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex
    ) {
        size_t res_col    = 0;
        size_t num_of_col = inputDataMat.get_sizeof_row();
        for (size_t col = 1; col <= num_of_col; ++col) {
            if (inputDataMat(vertex, col) != 0) {
                res_col = col;
                break;
            }
        }
        int subbed_value = 1;
        inputDataMat(vertex, res_col) -= subbed_value;
        inputDataMat(res_col, vertex) -= subbed_value;
        return res_col; // return value could be discarded
    }
    /**
     * @brief cut_first_iterable_directed_edge_of
     *
     * @param inputDataMat
     * @param vertex
     * @return size_t @b first_iterable_vertex
     */
    static size_t cut_first_iterable_directed_edge_of(
        Tool::Matrix<int>& inputDataMat,
        size_t             vertex
    ) {
        size_t res_col    = 0;
        size_t num_of_col = inputDataMat.get_sizeof_row();
        for (size_t col = 1; col <= num_of_col; ++col) {
            if (inputDataMat(vertex, col) != 0) {
                res_col = col;
                break;
            }
        }
        int subbed_value = 1;
        inputDataMat(vertex, res_col) -= subbed_value;
        return res_col; // return value could be discarded
    }
};

} // namespace Tool