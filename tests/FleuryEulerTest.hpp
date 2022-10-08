/**
 * @file FleuryEulerTest.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "../tools/undirected_graph.hpp"

namespace Test {

void FleuryEulerTest() {
    undirected_graph TestGraph_1 = {
        { 0, 1, 0, 1 },
        { 1, 0, 1, 2 },
        { 0, 1, 0, 1 },
        { 1, 2, 1, 0 },
    };
    auto H_fastest
        = undirected_graph::return_euler_circle_set_H_fastest(TestGraph_1);
    auto H
        = undirected_graph::return_euler_circle_set_H(TestGraph_1);

    for (auto&& vec : H_fastest) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (auto&& vec : H) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

} // namespace Test
