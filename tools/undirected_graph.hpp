/**
 * @file Graph.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief Graph
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Matrix.hpp"
#include <memory>
#include <stdexcept>
#include <utility>

using intMat = Tool::Matrix<int>;

struct info {
    size_t num_of_nodes = 1;
    size_t num_of_edges = 0;

    info()                         = default;
    info(const info& i)            = default;
    info(info&& i)                 = default;
    info& operator=(const info& i) = default;
    info& operator=(info&& i)      = default;
    ~info()                        = default;

    explicit info(info* i) {
        num_of_nodes = i->num_of_nodes;
        num_of_edges = i->num_of_edges;
    }

    explicit info(intMat* initMat) {
        assert(initMat->get_sizeof_row() == initMat->get_sizeof_col());
        num_of_nodes = initMat->get_sizeof_row();
        num_of_edges = initMat->sum();
    }
};

class undirected_graph {
private:
    intMat* DataMat          = nullptr; // unsafe pointer
    info*   GraphInfo        = nullptr; // unsafe pointer
    bool    if_will_be_moved = false;

    constexpr bool check_DataMat(intMat* DataMat) {
        return DataMat->get_sizeof_row() == DataMat->get_sizeof_col();
    }

public:
    ~undirected_graph() {
        if (if_will_be_moved) {
            return;
        } else {
            delete DataMat;
            delete GraphInfo;
        }
    }
    undirected_graph() = delete; // something has to delete to avoid error
    /// @brief move constructor
    undirected_graph(undirected_graph&& another) noexcept {
        DataMat           = another.DataMat;
        GraphInfo         = another.GraphInfo;
        another.DataMat   = nullptr;
        another.GraphInfo = nullptr;
    }
    /// @brief copy constructor
    undirected_graph(const undirected_graph& another) {
        DataMat   = new intMat(another.DataMat);
        GraphInfo = new info(another.GraphInfo);
    }
    /// @brief move assignment
    undirected_graph& operator=(undirected_graph&& another) noexcept {
        DataMat           = another.DataMat;
        GraphInfo         = another.GraphInfo;
        another.GraphInfo = nullptr;
        another.DataMat   = nullptr;
        return *this;
    }
    /// @brief copy assignment
    undirected_graph& operator=(const undirected_graph& another) {
        DataMat   = new intMat(another.DataMat);
        GraphInfo = new info(another.GraphInfo);
        return *this;
    }
    undirected_graph(std::initializer_list<
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
        GraphInfo = new info();
    }
    explicit undirected_graph(std::vector<
                              std::vector<int>>& initMat) {
        DataMat = new intMat(initMat);
        if (!check_DataMat(DataMat)) {
            delete DataMat;
            throw std::logic_error("Input Matrix doesn't have the same num of row and col!");
        };
        GraphInfo = new info(DataMat);
    }
    explicit undirected_graph(std::vector<
                              std::vector<int>>&& initMat) {
        DataMat = new intMat(
            std::forward<
                std::vector<std::vector<int>>>(initMat)
        );
        if (!check_DataMat(DataMat)) {
            delete DataMat;
            throw std::logic_error("Input Matrix doesn't have the same num of row and col!");
        };
        GraphInfo = new info(DataMat);
    }

    static undirected_graph&& create_trivial() {
        return create_zero();
        // undirected_graph res = { { 0 } };
        // res.if_will_be_moved = true;
        // return std::move(res);
    }
    static undirected_graph&& create_zero(size_t num_of_nodes = 1) {
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
        undirected_graph res(std::move(initMat));
        res.if_will_be_moved = true;
        return std::move(res); // call delete func here
    }
    static constexpr bool is_same(
        const undirected_graph& lhs,
        const undirected_graph& rhs
    ) {
        auto l_mat = lhs.DataMat;
        auto r_mat = rhs.DataMat;
        return Tool::Matrix<int>::A_eq_B(l_mat, r_mat);
    }
};