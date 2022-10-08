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
    auto trivial_graph
        = std::move(undirected_graph::create_trivial());
    auto zero_graph_5
        = std::move(undirected_graph::create_zero(5));
    auto zero_graph_1
        = std::move(undirected_graph::create_zero());

    assert(undirected_graph::is_same(zero_graph_1, trivial_graph));
}

} // namespace Test