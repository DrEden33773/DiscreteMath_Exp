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
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_set>
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

    size_t return_num_of_edges() { // directed_graph
        return DataMat->sum();
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

    static directed_graph create_trivial() {
        return create_zero();
    }
    static directed_graph create_zero(size_t num_of_nodes = 1) {
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
        return res; // call delete func here
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

        for (size_t pow_num = 0; pow_num < num_of_nodes; ++pow_num) {
            // A^0 + A^1 + A^2 + ... + A^(n-1)
            auto powered = inputDataMat ^ pow_num;
            auto added   = final + powered;
            final        = added;
        }

        if (final.if_have_zero_integer(final)) {
            return false;
        }
        return true;
    }
    static bool if_connective(Tool::Matrix<int> inputDataMat) {
        auto num_of_nodes = inputDataMat.get_sizeof_row();
        auto final
            = Tool::Matrix<int>::CreateZeroMat(
                num_of_nodes,
                num_of_nodes
            );

        for (size_t pow_num = 0; pow_num < num_of_nodes; ++pow_num) {
            // A^0 + A^1 + A^2 + ... + A^(n-1)
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
    static Tool::Matrix<int> return_undirected_matrix(
        directed_graph& input
    ) { // will be used in Tarjan Algorithm
        Tool::Matrix<int> res        = *(input.DataMat);
        size_t            num_of_row = res.get_sizeof_row();
        size_t            num_of_col = res.get_sizeof_col();
        // first deal with self ring
        for (size_t row = 1; row <= num_of_row; ++row) {
            res(row, row) *= 2;
        }
        // then deal with other edge
        for (size_t row = 1; row <= num_of_row; ++row) {
            for (size_t col = 1 + row; col <= num_of_col; ++col) {
                int expectation_num = res(row, col) + res(col, row);
                res(row, col)       = expectation_num;
                res(col, row)       = expectation_num;
            }
        }
        return res;
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
        int subbed_value = (vertex == col) ? 2 : 1;
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
        int    added_value = (vertex == col) ? 2 : 1;
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
        int subbed_value = (vertex == res_col) ? 2 : 1;
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

    /// @brief try to return @e all @b euler_circle
    static std::vector<std::string>
    return_euler_circle_set_H_fastest(directed_graph& input) {
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
    return_euler_circle_set_H(directed_graph& input) {
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
    static std::vector<std::string>
    return_euler_circle_set_F(directed_graph& input) {
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
                = input.return_an_euler_circle_F(input, curr_vertex);
            res.push_back(an_euler_circle);
        }

        return res;
    }

    /// @brief @b Fleury_liked_Algorithm ( @b not_recommended )
    /// @warning @b This_Fleury-liked_function_is_not_recommended
    /**
     * @brief @b Fleury_liked_Algorithm
     * @note
            Originally, @e Fleury_algorithm is to @p avoid_passing_the_bridge,
            but I haven't learnt @e Tarjan_algorithm to figure out the bridge.

            Thus, I made an @p alternation => I've found that
                ` the edge to broke the @e connectivity of an @b connective_graph `
            is always the bridge, only if the original graph is an
                ` @b undirected_connective ` one

            So everything is simple =>
            1). |
                |-> In an @e undirected_graph , delete an @b iterable_edge ,
                    if there's only one, then you need to add the start vertex into
                        ` @b ignored_vertex_set `.
                    Then you just judge the @b partial_connectivity of
                        ` @b {DataMat} @p <sub> @b ignored_vertex `
                    (a) If it's @b partial_connective , @e OK! , you just go ahead!
                    (b) Else , that's a wrong edge, you need to add it back
                        |-> then you should find another  @b iterable_edge
            2). |
                |-> In an @e directed_graph , you need to =>
                    |-> (1) transfer the @b DataMat into an @b Undirected_One
                        (2) do @p edge_iteration_with_deletion in
                                ` @b Undirected_Mat ` and ` @b Directed(Original)_Mat `
                            @p AT_THE_SAME_TIME
                        (3) @b ONLY_USE the @b Undirected_Mat for
                                (a) @p BUILD @b ignored_vertex_set
                                (b) @p JUDGE @b partial_connectivity

            But, T(n) = O( (n^2) * log(n) ) [ @b A_BIG_COST ]
                            ^^^    ^^^^^^
                            |           |==> Quick_Power
                            |==> Fast Matrix Multiply

            Hope You could @p Enjoy! @b ~~(^ω^)~~

     * @param input
     * @param vertex
     * @return std::string
     */
    static std::string
    return_an_euler_circle_F(directed_graph& input, size_t vertex) {
        std::string res = {};

        if (!input.if_has_euler_circle(input)) {
            res += "NO euler circle! ";
            return res;
        }
        if (input.if_trivial(input)) {
            res += "Trivial -> " + std::to_string(vertex) + " -> fin. ";
            return res;
        }

        Tool::Matrix<int> inputDataMat = *(input.DataMat); // copy one
        Tool::Matrix<int> undirected_DataMat
            = directed_graph::return_undirected_matrix(input);

        // Fleury Algorithm
        size_t curr_vertex           = vertex;
        size_t original_input_vertex = vertex;
        size_t num_of_col            = inputDataMat.get_sizeof_col();
        size_t num_of_edge           = input.return_num_of_edges();

        std::stack<size_t> path; // res

        std::unordered_set<size_t> ignored_vertex {};
        ignored_vertex.reserve(num_of_col);

        while (num_of_edge > 0) { // must do it ahead (at least for once)
            size_t curr_in_deg  = inputDataMat.sum_of_col(curr_vertex);
            size_t curr_out_deg = inputDataMat.sum_of_row(curr_vertex);
            size_t curr_deg     = curr_in_deg + curr_out_deg;
            size_t related_undirected_curr_deg
                = undirected_DataMat.sum_of_row(curr_vertex);

            if (curr_deg == 0) {
                break;
            }

            if (curr_in_deg == 0) {
                // you cannot step into this vertex
                // then it is `abandoned`
                ignored_vertex.emplace(curr_vertex);
            }

            for (size_t col = 1; col <= num_of_col; ++col) {
                auto& curr_elem = inputDataMat(curr_vertex, col);
                if (curr_elem == 0) {
                    continue;
                } else {
                    path.push(curr_vertex);

                    --inputDataMat(curr_vertex, col);
                    --curr_out_deg;
                    --curr_deg;
                    --num_of_edge;

                    // do the same in related_undirected_mat
                    int subbed_value = (curr_vertex == col) ? 2 : 1; // self-ring judgement
                    undirected_DataMat(curr_vertex, col) -= subbed_value;
                    undirected_DataMat(col, curr_vertex) -= subbed_value;
                    related_undirected_curr_deg -= subbed_value;

                    if (related_undirected_curr_deg == 0) { // don't judge the connectivity
                        // that deleted path is the only path for current vertex
                        // then we have to adapt that path, without considering connectivity
                        ignored_vertex.emplace(curr_vertex);
                        curr_vertex = col;
                        break;
                    } else { // need to judge the connectivity
                        if (!input.if_partial_connective(
                                undirected_DataMat,
                                ignored_vertex
                            )) {
                            path.pop();
                            ++inputDataMat(curr_vertex, col);
                            ++curr_out_deg;
                            ++curr_deg;
                            ++num_of_edge;

                            // do the same in related_undirected_mat
                            undirected_DataMat(curr_vertex, col) += subbed_value;
                            undirected_DataMat(col, curr_vertex) += subbed_value;
                            related_undirected_curr_deg += subbed_value;

                            continue;
                        }
                        curr_vertex = col;
                        break;
                    }
                }
            }
        };

        std::stack<size_t> true_path;

        while (!path.empty()) {
            size_t curr = path.top();
            path.pop();
            true_path.push(curr);
        }
        while (!true_path.empty()) {
            size_t curr = true_path.top();
            true_path.pop();
            res += std::to_string(curr);
            res += " -> ";
        }

        /**
         * @brief
                the last vertex is the start vertex
                but when you try to push it into the stack in the `while` loop,
                you'll find it's impossible because `edge == 0` and you can't get in
                so you have to add it manually
         * @param res
         */
        res += std::to_string(vertex) + " -> ";
        res += "fin. ";

        return res;
    }

    /// @brief Hierholzer Algorithm, T(n)=O(n^2), fastest
    /// @ref https://www.jianshu.com/p/8394b8e5b878
    /// @attention this is a reference, not the original work of me!
    static std::string
    return_an_euler_circle_H_fastest(directed_graph& input, size_t vertex) {
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
            size_t curr_in_deg  = inputDataMat.sum_of_col(curr_vertex);
            size_t curr_out_deg = inputDataMat.sum_of_row(curr_vertex);
            size_t curr_deg     = curr_in_deg + curr_out_deg;
            if (curr_deg > 0) {
                // curr_vertex is not alone
                // we haven't found the ring
                path.push(curr_vertex);
                size_t next_vertex = curr_vertex;
                for (size_t col = 1; col <= num_of_col; ++col) {
                    auto curr_edge_num = inputDataMat(curr_vertex, col);
                    if (curr_edge_num > 0) {
                        --inputDataMat(curr_vertex, col);
                        next_vertex = col;
                        break;
                    }
                }
                curr_vertex = next_vertex;
            } else {
                // curr_vertex is alone
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

    /// @brief Hierholzer Algorithm, T(n)=O(n^2)
    /// @brief take @p if_has_euler_circle's @p if_connective in then T(n)=O((n^2)*log(n))
    /// @brief This is slower, but easier to comprehend
    /// @e This_one_is_totally_originally_written_by_me
    /// @e Hierholzer_Algorithm_YYDS
    static std::string
    return_an_euler_circle_H(directed_graph& input, size_t vertex) {
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
            size_t curr_in_deg  = inputDataMat.sum_of_col(curr_vertex);
            size_t curr_out_deg = inputDataMat.sum_of_row(curr_vertex);
            size_t curr_deg     = curr_in_deg + curr_out_deg;
            if (curr_deg > 0) {
                // curr_vertex is not alone
                // we haven't found the ring
                path.push(curr_vertex);
                size_t next_vertex = curr_vertex; // only as default value
                for (size_t col = 1; col <= num_of_col; ++col) {
                    auto curr_edge_num = inputDataMat(curr_vertex, col);
                    if (curr_edge_num > 0) {
                        // remove edge
                        --inputDataMat(curr_vertex, col);
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
                    // compensate sum_of_edge
                    ++curr_edge_sum;
                    // reset the compensate flag
                    if_compensate = false;
                }
            } else {
                // curr_vertex is alone
                // that means we've found a ring
                curr_vertex = path.top();
                path.pop(); // because it will be pushed into it again!
                if_compensate = true;
            }
        }

        std::stack<size_t> true_path;

        while (!path.empty()) {
            size_t curr = path.top();
            path.pop();
            true_path.push(curr);
        }
        while (!true_path.empty()) {
            size_t curr = true_path.top();
            true_path.pop();
            res += std::to_string(curr);
            res += " -> ";
        }

        /**
         * @brief
                the last vertex is the start vertex
                but when you try to push it into the stack in the `while` loop,
                you'll find it's impossible because `edge == 0` and you can't get in
                so you have to add it manually
         * @param res
         */
        res += std::to_string(vertex) + " -> ";
        res += "fin. ";

        return res;
    }

    /// @brief operator overloads
    friend bool operator==(directed_graph& lhs, directed_graph& rhs) {
        bool if_data_mat_same = *(lhs.DataMat) == *(rhs.DataMat);
        return if_data_mat_same;
    }
    friend bool operator!=(directed_graph& lhs, directed_graph& rhs) {
        return !(lhs == rhs);
    }
};
