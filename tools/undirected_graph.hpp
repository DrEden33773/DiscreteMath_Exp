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

class undirected_graph {
private:
    std::unique_ptr<Tool::Matrix<int>> DataMat; // a safe pointer

public:
    ~undirected_graph() = default;
    undirected_graph()  = delete; // something has to delete to avoid error

    /// @brief move constructor
    undirected_graph(undirected_graph&& another) noexcept {
        DataMat.reset(another.DataMat.release());
        another.DataMat.reset(nullptr);
    }
    /// @brief copy constructor
    undirected_graph(const undirected_graph& another) {
        DataMat.reset(
            new Tool::Matrix<int>(*(another.DataMat))
        );
    }
    /// @brief copy assignment
    undirected_graph& operator=(const undirected_graph& another) {
        DataMat.reset(
            new Tool::Matrix<int>(*(another.DataMat))
        );
        return *this;
    }
    /// @brief move assignment
    undirected_graph& operator=(undirected_graph&& another) noexcept {
        DataMat.reset(another.DataMat.release());
        another.DataMat.reset(nullptr);
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

    static undirected_graph&& create_trivial() {
        undirected_graph res = { { 0 } };
        return std::move(res);
    }
};