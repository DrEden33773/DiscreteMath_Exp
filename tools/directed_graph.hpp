/**
 * @file directed_graph.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief directed_graph (alias => graph)
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Matrix.hpp"
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

using intMat = Tool::Matrix<int>;

class directed_graph;
using graph = directed_graph;

class directed_graph {
private:
    intMat* DataMat          = nullptr; // unsafe pointer
    bool    if_will_be_moved = false;

    constexpr bool check_DataMat(intMat* DataMat) {
        return DataMat->get_sizeof_row() == DataMat->get_sizeof_col();
    }
    bool if_symmetric_of_main_diagonal() {
        return Tool::Matrix<int>::if_symmetric_of_main_diagonal(*DataMat);
    }

    size_t return_num_of_edges() {
        return DataMat->sum() / 2;
    }
    size_t return_num_of_nodes() {
        return DataMat->get_sizeof_row();
    }

public:
    ~directed_graph() {
        if (if_will_be_moved) {
            return;
        } else {
            delete DataMat;
            // delete GraphInfo;
        }
    }
    directed_graph() = delete; // something has to delete to avoid error
    /// @brief move constructor
    directed_graph(directed_graph&& another) noexcept {
        DataMat = another.DataMat;
    }
    /// @brief copy constructor
    directed_graph(const directed_graph& another) {
        DataMat = new intMat(another.DataMat);
    }
    /// @brief move assignment
    directed_graph& operator=(directed_graph&& another) noexcept {
        DataMat = another.DataMat;
        return *this;
    }
    /// @brief copy assignment
    directed_graph& operator=(const directed_graph& another) {
        DataMat = new intMat(another.DataMat);
        return *this;
    }
    directed_graph(std::initializer_list<
                   std::initializer_list<int>>&& initMat) {
        std::vector<std::vector<int>> initVec;
        std::vector<int>              initVec_inner;

        initVec.reserve(initMat.size());
        initVec_inner.reserve(initMat.begin()->size());

        for (auto&& currRow : initMat) {
            for (auto&& currElem : currRow) {
                initVec_inner.emplace_back(currElem);
            }
            initVec.emplace_back(initVec_inner);
            initVec_inner.clear();
        }

        DataMat = new intMat(std::move(initVec));
        if (!check_DataMat(DataMat)) {
            delete DataMat;
            throw std::logic_error("Input Matrix doesn't have the same num of row and col!");
        };
    }
    explicit directed_graph(std::vector<
                            std::vector<int>>& initMat) {
        DataMat = new intMat(initMat);
        if (!check_DataMat(DataMat)) {
            delete DataMat;
            throw std::logic_error("Input Matrix doesn't have the same num of row and col!");
        };
    }
    explicit directed_graph(std::vector<
                            std::vector<int>>&& initMat) {
        DataMat = new intMat(
            std::forward<
                std::vector<std::vector<int>>>(initMat)
        );
        if (!check_DataMat(DataMat)) {
            delete DataMat;
            throw std::logic_error("Input Matrix doesn't have the same num of row and col!");
        };
    }

    static directed_graph&& create_trivial() {
        return create_zero();
    }
    static directed_graph&& create_zero(size_t num_of_nodes = 1) {
        std::vector<int> initRaw;
        initRaw.reserve(num_of_nodes);
        for (size_t i = 0; i < num_of_nodes; ++i) {
            initRaw.emplace_back(0);
        }
        std::vector<std::vector<int>> initMat;
        initMat.reserve(num_of_nodes);
        for (size_t i = 0; i < num_of_nodes; ++i) {
            initMat.emplace_back(initRaw);
        }
        directed_graph res(std::move(initMat));
        res.if_will_be_moved = true;
        return std::move(res); // call delete func here
    }
    static constexpr bool is_same(
        const directed_graph& lhs,
        const directed_graph& rhs
    ) {
        auto l_mat = lhs.DataMat;
        auto r_mat = rhs.DataMat;
        return Tool::Matrix<int>::A_eq_B(l_mat, r_mat);
    }

    /// @brief judge if has a euler circle
    static bool if_has_euler_circle(directed_graph& input) {
        bool res              = true;
        bool if_is_connective = if_connective(input);

        size_t curr_vertex = 1;
        auto   num_of_node = input.return_num_of_nodes();

        while (curr_vertex <= num_of_node) {
            size_t curr_out_deg = input.DataMat->sum_of_row(curr_vertex);
            size_t curr_in_deg  = input.DataMat->sum_of_col(curr_vertex);
            if (curr_out_deg != curr_in_deg) {
                res = false;
                break;
            } else {
                ++curr_vertex;
                continue;
            }
        }

        return res && if_is_connective;
    }

    /// @brief judge if is a connective graph
    static bool if_connective(directed_graph& input) {
        Tool::Matrix<int> inputDataMat = *(input.DataMat);

        auto num_of_nodes = input.return_num_of_nodes();

        auto final
            = Tool::Matrix<int>::CreateZeroMat(
                num_of_nodes,
                num_of_nodes
            );

        for (size_t pow_num = 1;
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
    static bool if_connective(Tool::Matrix<int>& inputDataMat) {
        auto num_of_nodes = inputDataMat.get_sizeof_row();
        auto final
            = Tool::Matrix<int>::CreateZeroMat(
                num_of_nodes,
                num_of_nodes
            );

        for (size_t pow_num = 1;
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

    /// @brief judge if the input graph is a trivial graph
    static bool if_trivial(directed_graph& input) {
        Tool::Matrix<int>& inputDataMat = *(input.DataMat);
        if (inputDataMat.get_sizeof_col() != 1
            || inputDataMat.get_sizeof_row() != 1) {
            return false;
        }
        if (inputDataMat(1, 1) != 0) {
            return false;
        }
        return true;
    }
};
