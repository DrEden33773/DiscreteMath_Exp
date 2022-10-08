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
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

using intMat = Tool::Matrix<int>;

class undirected_graph {
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
    ~undirected_graph() {
        if (if_will_be_moved) {
            return;
        } else {
            delete DataMat;
            // delete GraphInfo;
        }
    }
    undirected_graph() = delete; // something has to delete to avoid error
    /// @brief move constructor
    undirected_graph(undirected_graph&& another) noexcept {
        DataMat = another.DataMat;
    }
    /// @brief copy constructor
    undirected_graph(const undirected_graph& another) {
        DataMat = new intMat(another.DataMat);
    }
    /// @brief move assignment
    undirected_graph& operator=(undirected_graph&& another) noexcept {
        DataMat = another.DataMat;
        return *this;
    }
    /// @brief copy assignment
    undirected_graph& operator=(const undirected_graph& another) {
        DataMat = new intMat(another.DataMat);
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
        if (!if_symmetric_of_main_diagonal()) {
            delete DataMat;
            throw std::logic_error("Input Matrix is not symmetric of the main diagonal!");
        };
    }
    explicit undirected_graph(std::vector<
                              std::vector<int>>& initMat) {
        DataMat = new intMat(initMat);
        if (!check_DataMat(DataMat)) {
            delete DataMat;
            throw std::logic_error("Input Matrix doesn't have the same num of row and col!");
        };
        if (!if_symmetric_of_main_diagonal()) {
            delete DataMat;
            throw std::logic_error("Input Matrix is not symmetric of the main diagonal!");
        };
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
        if (!if_symmetric_of_main_diagonal()) {
            delete DataMat;
            throw std::logic_error("Input Matrix is not symmetric of the main diagonal!");
        };
    }

    static undirected_graph&& create_trivial() {
        return create_zero();
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

    /// @brief judge if has a euler circle
    static bool if_has_euler_circle(undirected_graph& input) {
        bool res              = true;
        bool if_is_connective = if_connective(input);

        size_t curr_vertex = 1;
        auto   num_of_node = input.return_num_of_nodes();

        while (curr_vertex <= num_of_node) {
            size_t curr_deg = input.DataMat->sum_of_row(curr_vertex);
            if (curr_deg % 2 != 0) {
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
    static bool if_connective(undirected_graph& input) {
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
    static bool if_trivial(undirected_graph& input) {
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

    /// @brief try to return all euler circle
    static std::vector<std::string>
    return_euler_circle_set_H_fastest(undirected_graph& input) {
        std::vector<std::string> res = {};

        size_t all_vertex = input.return_num_of_nodes();

        if (!input.if_has_euler_circle(input)) {
            std::string an_euler_circle = {};
            an_euler_circle += "NO euler circle! ";
            res.push_back(an_euler_circle);
            return res;
        }
        for (size_t curr_vertex = 1;
             curr_vertex <= all_vertex;
             ++curr_vertex) {
            std::string an_euler_circle
                = input.return_an_euler_circle_H_fastest(input, curr_vertex);
            res.push_back(an_euler_circle);
        }

        return res;
    }
    static std::vector<std::string>
    return_euler_circle_set_H(undirected_graph& input) {
        std::vector<std::string> res = {};

        size_t all_vertex = input.return_num_of_nodes();

        if (!input.if_has_euler_circle(input)) {
            std::string an_euler_circle = {};
            an_euler_circle += "NO euler circle! ";
            res.push_back(an_euler_circle);
            return res;
        }
        for (size_t curr_vertex = 1;
             curr_vertex <= all_vertex;
             ++curr_vertex) {
            std::string an_euler_circle
                = input.return_an_euler_circle_H(input, curr_vertex);
            res.push_back(an_euler_circle);
        }

        return res;
    }

    /// @brief Fleury_Algorithm ( @b discarded )
    /// @warning @b This_Fleury-liked_function_is_discarded
    static std::string
    return_an_euler_circle_F(undirected_graph& input, size_t vertex) {
        std::string res = {};

        res += "Fleury_Algorithm has been discarded! Won't find euler path!";
        return res;

        if (!input.if_has_euler_circle(input)) {
            res += "NO euler circle! ";
            return res;
        }
        if (input.if_trivial(input)) {
            res += "Trivial -> " + std::to_string(vertex) + " -> fin. ";
            return res;
        }

        Tool::Matrix<int>& inputDataMat = *(input.DataMat);

        // Fleury Algorithm
        size_t curr_vertex           = vertex;
        size_t original_input_vertex = vertex;
        bool   if_start              = false;
        while (!(if_start && curr_vertex == vertex)) {
            if_start = true;

            auto num_of_col = input.DataMat->get_sizeof_col();
            for (size_t col = 1;
                 col <= num_of_col;
                 ++col) {
                auto& curr_elem = inputDataMat(curr_vertex, col);
                if (curr_elem == 0) {
                    continue;
                } else {
                    // start to operate
                    // try to -1 each time and then judge the connectivity
                    --inputDataMat(curr_vertex, col);
                    --inputDataMat(col, curr_vertex);
                    auto curr_vertex_degree
                        = inputDataMat.sum_of_row(curr_vertex);
                    if (curr_vertex_degree != 0) {
                        if (!input.if_connective(inputDataMat)) {
                            ++inputDataMat(curr_vertex, col);
                            ++inputDataMat(col, curr_vertex);
                            continue;
                        } else {
                            res += std::to_string(curr_vertex);
                            res += " -> ";
                            curr_vertex = col;
                            break;
                        }
                    } else {
                        res += std::to_string(curr_vertex);
                        res += " -> ";
                        curr_vertex = col;
                        break;
                    }
                }
            }
        }

        res += std::to_string(original_input_vertex);

        return res;
    }

    /// @brief Hierholzer Algorithm, T(n)=O(n), fastest
    /// @ref https://www.jianshu.com/p/8394b8e5b878
    /// @attention this is a reference, not the original work of me!
    static std::string
    return_an_euler_circle_H_fastest(undirected_graph& input, size_t vertex) {
        std::string        res;
        std::stack<size_t> path;
        std::stack<size_t> loop_vertex;

        if (!input.if_has_euler_circle(input)) {
            res += "NO euler circle! ";
            return res;
        }
        if (input.if_trivial(input)) {
            res += "Trivial -> " + std::to_string(vertex) + " -> fin. ";
            return res;
        }

        size_t            curr_vertex = vertex;
        Tool::Matrix<int> inputDataMat(input.DataMat); // no ref
        path.push(curr_vertex);
        size_t num_of_col = inputDataMat.get_sizeof_col();
        while (!path.empty()) {
            size_t curr_deg = inputDataMat.sum_of_row(curr_vertex);
            if (curr_deg != 0) {
                // curr_vertex can reach other vertex
                // we haven't found the ring
                path.push(curr_vertex);
                size_t next_vertex = curr_vertex;
                for (size_t col = 1; col <= num_of_col; ++col) {
                    auto curr_edge_num = inputDataMat(curr_vertex, col);
                    if (curr_edge_num > 0) {
                        --inputDataMat(curr_vertex, col);
                        --inputDataMat(col, curr_vertex);
                        next_vertex = col;
                        break;
                    }
                }
                curr_vertex = next_vertex;
            } else {
                // curr_vertex cannot reach another vertex
                // that means we've found a ring
                loop_vertex.push(curr_vertex);
                curr_vertex = path.top();
                path.pop();
            }
        };

        while (!loop_vertex.empty()) {
            size_t curr = loop_vertex.top();
            loop_vertex.pop();
            res += std::to_string(curr);
            res += " -> ";
        }
        res += "fin.";

        return res;
    }

    /// @brief Hierholzer Algorithm, T(n)=O(n*n) [could become O(n)]
    /// @brief This is slower, but easier to comprehend
    /// @e This_one_is_totally_originally_written_by_me
    /// @e Hierholzer_Algorithm_YYDS
    static std::string
    return_an_euler_circle_H(undirected_graph& input, size_t vertex) {
        std::string        res;
        std::stack<size_t> path;

        if (!input.if_has_euler_circle(input)) {
            res += "NO euler circle! ";
            return res;
        }
        if (input.if_trivial(input)) {
            res += "Trivial -> " + std::to_string(vertex) + " -> fin. ";
            return res;
        }

        size_t            curr_vertex = vertex;
        Tool::Matrix<int> inputDataMat(input.DataMat); // no ref
        size_t            curr_edge_sum = input.return_num_of_edges();
        size_t            num_of_col    = inputDataMat.get_sizeof_col();

        /**
         * @param if_compensate
         * @brief
                If you've stepped into an @b alone_node , while you still haven't pass
                all edges, then you need to compensate the latest-deleted edge.

                But, you shouldn't compensate immediately, because
                after you backtraced to @e get @b vertex_nearest_the_alone_one ,
                you hate to relate @b it to the @b previous_alone_node
                |
                |
                |-> @brief this phenomenon is definite, because you have @b already
                    linked to that @b alone_node , which shows that the
                    @p index_of_that_alone_node is smaller than any other node linked to
                    @b vertex_nearest_the_alone_one (smaller_index node always get linked first)

                You should compensate after @e linking @b vertex_nearest_the_alone_one
                with @b the_vertex_next_to_alone_one
         */
        bool if_compensate = false;

        size_t compensated_vertex = curr_vertex; // only as default value
        size_t compensated_col    = 1;           // only as default value

        // the opt ahead costs more time => T(n) = O(n*n)
        while (curr_edge_sum > 0) {
            size_t curr_deg = inputDataMat.sum_of_row(curr_vertex);
            if (curr_deg > 0) {
                // curr_vertex can reach other vertex
                // we haven't found the ring
                path.push(curr_vertex);
                size_t next_vertex = curr_vertex; // only as default value
                for (size_t col = 1; col <= num_of_col; ++col) {
                    auto curr_edge_num = inputDataMat(curr_vertex, col);
                    if (curr_edge_num > 0) {
                        // remove edge
                        --inputDataMat(curr_vertex, col);
                        --inputDataMat(col, curr_vertex);
                        // update the compensated (if not used, it's OK)
                        compensated_vertex = curr_vertex;
                        compensated_col    = col;
                        // update vertex
                        next_vertex = col;
                        break;
                    }
                }
                curr_vertex = next_vertex;
                --curr_edge_sum; // update sum_of_edge
                if (if_compensate) {
                    // relink the edge
                    ++inputDataMat(compensated_vertex, compensated_col);
                    ++inputDataMat(compensated_col, compensated_vertex);
                    // compensate sum_of_edge
                    ++curr_edge_sum;
                    // reset the compensate flag
                    if_compensate = false;
                }
            } else {
                // curr_vertex cannot reach another vertex
                // that means we've found a ring
                curr_vertex = path.top();
                path.pop(); // because it will be pushed into it again!
                if_compensate = true;
            }
        }

        while (!path.empty()) {
            size_t curr = path.top();
            path.pop();
            res += std::to_string(curr);
            res += " -> ";
        }
        res = std::to_string(vertex) + " -> " + res;
        res += "fin. ";

        return res;
    }

    /// @brief operator overloads
    friend bool operator==(undirected_graph& lhs, undirected_graph& rhs) {
        bool if_data_mat_same = *(lhs.DataMat) == *(rhs.DataMat);
        return if_data_mat_same;
    }
    friend bool operator!=(undirected_graph& lhs, undirected_graph& rhs) {
        return !(lhs == rhs);
    }
};