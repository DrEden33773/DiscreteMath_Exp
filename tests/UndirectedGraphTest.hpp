/**
 * @file UndirectedGraphTest.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief UndirectedGraphTest
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "../tools/undirected_graph.hpp"

namespace Test {

void UndirectedGraphTest() {
    std::cout << std::endl;

    auto trivial_graph
        = undirected_graph::create_trivial();

    auto zero_graph_5
        = undirected_graph::create_zero(5);

    auto zero_graph_1
        = undirected_graph::create_zero();

    if (undirected_graph::is_same(zero_graph_1, trivial_graph)) {
        std::cout << "zero_graph_with_1_point is same as trivial_graph" << std::endl;
    }

    std::cout << std::endl;
}

} // namespace Test