/**
 * @file GraphTest.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief GraphTest
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
    auto zero_graph_5  = undirected_graph::create_zero(5);
    auto trivial_graph = undirected_graph::create_trivial();
    auto zero_graph_1  = undirected_graph::create_zero();
}

}