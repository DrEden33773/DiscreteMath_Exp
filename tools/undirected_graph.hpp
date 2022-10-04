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
#include <utility>

using intMat = Tool::Matrix<int>;

class undirected_graph {
private:
    struct info {
        size_t num_of_nodes = 0;
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
    };
    std::unique_ptr<intMat> DataMat   = nullptr; // safe pointer
    std::unique_ptr<info>   GraphInfo = nullptr; // safe pointer

public:
    ~undirected_graph() = default;
    undirected_graph()  = delete; // something has to delete to avoid error
    /// @brief move constructor
    undirected_graph(undirected_graph&& another) noexcept {
        DataMat.reset(another.DataMat.release());
        another.DataMat.reset(nullptr);
        GraphInfo.reset(another.GraphInfo.release());
        another.GraphInfo.reset(nullptr);
    }
    /// @brief copy constructor
    undirected_graph(const undirected_graph& another) {
        DataMat.reset(
            new intMat(another.DataMat.get())
        );
        GraphInfo.reset(
            new info(another.GraphInfo.get())
        );
    }
    /// @brief move assignment
    undirected_graph& operator=(undirected_graph&& another) noexcept {
        DataMat.reset(another.DataMat.release());
        another.DataMat.reset(nullptr);
        GraphInfo.reset(another.GraphInfo.release());
        another.GraphInfo.reset(nullptr);
        return *this;
    }
    /// @brief copy assignment
    undirected_graph& operator=(const undirected_graph& another) {
        DataMat.reset(
            new intMat(another.DataMat.get())
        );
        GraphInfo.reset(
            new info(another.GraphInfo.get())
        );
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

        DataMat = std::make_unique<Tool::Matrix<int>>(initVec);
    }
    explicit undirected_graph(std::vector<
                              std::vector<int>>& initMat) {
        DataMat = std::make_unique<Tool::Matrix<int>>(initMat);
    }
    explicit undirected_graph(std::vector<
                              std::vector<int>>&& initMat) {
        DataMat = std::make_unique<Tool::Matrix<int>>(initMat);
    }

    static undirected_graph&& create_trivial() {
        undirected_graph res = { { 0 } };
        return std::move(res);
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
        return std::move(res);
    }
    static constexpr bool is_same(
        const undirected_graph& lhs,
        const undirected_graph& rhs
    ) {
        return *(lhs.DataMat) == *(rhs.DataMat);
    }
};