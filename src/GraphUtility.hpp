/**
 * @file GraphFactory.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief A Factory to construct graph
 * @note
        @b type_of_graph @e (undirected_or_directed) could be judged by the input matrix
            @b BUT , sometimes you still need to @p designate the @b type
                @b {{0,1},{1,0}} should be a example
                    @e could_be_designated_as_directed_graph
                    @e will_be_declared_as_undirected_graph
            @b RULE_OF_JUDGE:
                @b if the @e Matrix is @b Non_Symmetrical => @p Restricted_as @e directed
                @b if the @e Matrix is @b {{0}} => @p Restricted_as @e Undirected
                @b if the @e Matrix is @b {{2n+1}} => @p Restricted_as @e directed
                @b if the @e Matrix has @b 2n+1 @b on_major_diagonal => @p Restricted_as @e directed
                @b if the @e Matrix is @b Axially_Symmetrical => @p Judged_as @e Undirected
                    @b if you've @p designated the @e type => @p Abandon_Judgement

 * @version 0.1
 * @date 2022-10-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "../tools/Matrix.hpp"
#include "../tools/directed_graph.hpp"
#include "../tools/undirected_graph.hpp"

class GraphManager;
class GraphFactory;

class GraphManager {
    friend class GraphFactory;

    enum class Type : unsigned short {
        undirected = 0,
        directed   = 1,
    };
    directed_graph*   directed   = nullptr;
    undirected_graph* undirected = nullptr;
    Type              graph_type = Type::undirected; // default as undirected

public:
    std::string return_graph_type() {
        std::string res;
        if (this->graph_type == Type::directed) {
            res = "directed_graph";
        } else {
            res = "undirected_graph";
        }
        return res;
    }
    ~GraphManager() {
        if (directed) {
            delete directed;
        }
        if (undirected) {
            delete undirected;
        }
    }

private:
    GraphManager(std::vector<std::vector<int>>&& initMat, Type type_of_graph) {
        using origin = std::vector<std::vector<int>>;
        graph_type   = type_of_graph;
        if (graph_type == Type::undirected) {
            undirected = new undirected_graph(
                std::forward<origin>(initMat)
            );
        } else {
            directed = new directed_graph(
                std::forward<origin>(initMat)
            );
        }
    }

public:
    std::vector<std::string>
    return_euler_circle_set_H() {
        if (directed == nullptr && undirected == nullptr) {
            std::cout << std::endl;
            throw std::runtime_error("Graph is not initialized. ");
        }

        std::vector<std::string> res;

        if (graph_type == Type::undirected) {
            res = undirected_graph::return_euler_circle_set_H(*undirected);
        } else {
            res = directed_graph::return_euler_circle_set_H(*directed);
        }

        return res;
    }

    std::vector<std::string>
    return_euler_circle_set_F() {
        if (directed == nullptr && undirected == nullptr) {
            std::cout << std::endl;
            throw std::runtime_error("Graph is not initialized. ");
        }

        std::vector<std::string> res;

        if (graph_type == Type::undirected) {
            res = undirected_graph::return_euler_circle_set_H(*undirected);
        } else {
            res = directed_graph::return_euler_circle_set_H(*directed);
        }

        return res;
    }

    void show_euler_circle_set_H() {
        std::cout << "Hierholzer Algorithm => " << std::endl;
        std::cout << std::endl;

        std::vector<std::string> output;
        output = this->return_euler_circle_set_H();

        for (auto&& vec : output) {
            for (auto&& str : vec) {
                std::cout << str;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void show_euler_circle_set_F() {
        std::cout << "Fleury Algorithm (With Matrix Calculation) => " << std::endl;
        std::cout << std::endl;

        std::vector<std::string> output;
        output = this->return_euler_circle_set_F();

        for (auto&& vec : output) {
            for (auto&& str : vec) {
                std::cout << str;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

class GraphFactory {
    static constexpr bool if_any_less_than_zero(Tool::Matrix<int>& inputMat) {
        size_t row = inputMat.get_sizeof_row();
        size_t col = inputMat.get_sizeof_col();
        for (size_t trow = 1; trow <= row; ++trow) {
            for (size_t tcol = 1; tcol <= row; ++tcol) {
                if (inputMat(row, col) < 0) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    GraphFactory()  = delete;
    ~GraphFactory() = default;
    static GraphManager CreateGraph() {
        /// @brief @b graph_info
        int                num_of_v   = 0;
        GraphManager::Type graph_type = GraphManager::Type::undirected;
        /* note that => graph_type's default value should be `undirected` */

        /// @brief @b data
        Tool::Matrix<int>*            TheMat = nullptr;
        std::vector<std::vector<int>> initMat;
        std::vector<int>              initRow;

        /// @brief @p input @b vertex
        while (true) {
            std::cout << std::endl;
            std::cout << "Input num of vertex (>0) => ";

            std::cin >> num_of_v;

            if (num_of_v <= 0) {
                std::cout << std::endl;
                std::cout << "Input num should >0, please do it again. " << std::endl;
            } else {
                break;
            }
        }

        initMat.reserve(num_of_v);
        initRow.reserve(num_of_v);

        /// @brief @p input @b matrix
        std::cout << std::endl;
        std::cout << "Input each element in the Adjacency Matrix => " << std::endl;
        for (size_t row = 0; row < num_of_v; ++row) {
            for (size_t col = 0; col < num_of_v; ++col) {
                int tmp;
                std::cin >> tmp;
                initRow.emplace_back(tmp);
            }
            initMat.emplace_back(initRow);
            initRow.clear();
        }

        TheMat          = new Tool::Matrix<int>(initMat); // cannot move it!
        auto& TheMatRef = *TheMat;

        /// @brief @e logic_error_check_point
        if (if_any_less_than_zero(TheMatRef)) {
            std::cout << std::endl;
            throw std::logic_error("There's element <0 in the Matrix. ");
        }

        bool if_need_to_confirm_type = false;

        /// @brief @p judge/restrict @b type
        if (num_of_v != 1) {
            if (!Tool::Matrix<int>::
                    if_symmetric_of_main_diagonal(TheMatRef)) {
                graph_type = GraphManager::Type::directed;
                std::cout << std::endl;
                std::cout << "Type of Graph is restricted as {directed_graph} " << std::endl;
            } else {
                bool if_break = false;
                for (int index = 0; index < num_of_v; ++index) {
                    if (initMat[index][index] % 2 != 0) {
                        graph_type = GraphManager::Type::directed;
                        std::cout << std::endl;
                        std::cout << "Type of Graph is restricted as {directed_graph} " << std::endl;
                        if_break = true;
                        break;
                    }
                }
                if (!if_break) {
                    if_need_to_confirm_type = true;
                }
            }
        } else {
            int val = initMat[0][0];
            if (val % 2 != 0) {
                graph_type = GraphManager::Type::directed;
                std::cout << std::endl;
                std::cout << "Type restricted as {directed}. " << std::endl;
            } else if (val == 0) {
                graph_type = GraphManager::Type::undirected;
                std::cout << std::endl;
                std::cout << "Trivial Graph! Type judged as {undirected}. " << std::endl;
            }
        }

        /// @brief @p confirm @b type
        if (if_need_to_confirm_type) {
            std::cout << std::endl;
            std::cout << "You need confirm the Type of Graph. " << std::endl;

            std::cout << std::endl;
            std::cout << R"(If you want a {directed_graph}, input "Y" or "y". )"
                      << std::endl;
            std::cout << R"(If you want a {undirected_graph}, input anything except Y/y. )"
                      << std::endl;
            std::cout << std::endl;
            std::cout << "=> ";

            std::string flag;
            std::cin >> flag;

            if (flag == "Y" || flag == "y") {
                graph_type = GraphManager::Type::directed;
                std::cout << std::endl;
                std::cout << "Type confirmed as {directed}. " << std::endl;
            } else {
                graph_type = GraphManager::Type::undirected;
                std::cout << std::endl;
                std::cout << "Type confirmed as {undirected}. " << std::endl;
            }
        }

        /// @brief @p construct
        GraphManager res(std::move(initMat), graph_type);

        /// @brief @p show @b success_info_with_type
        std::cout << std::endl;
        if (graph_type == GraphManager::Type::undirected) {
            std::cout << "Successfully created a {undirected} graph" << std::endl;
        } else {
            std::cout << "Successfully created a {directed} graph" << std::endl;
        }

        /// @brief @p delete @b unused_mat
        delete TheMat;

        std::cout << std::endl;
        return res;
    }
};