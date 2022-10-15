/**
 * @file EulerTest_directed.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "../tools/directed_graph.hpp"

namespace Test {

void EulerTest_directed() {
    /**
     * @brief test of @b un_trivial
     * @param Nontrivial_Test
     */

    directed_graph Nontrivial_Test = {
        { 0, 1, 0, 0 },
        { 0, 0, 1, 1 },
        { 0, 0, 0, 1 },
        { 1, 1, 0, 0 },
    };

    /// @brief @b Hierholzer_Fastest
    auto H_fastest
        = directed_graph::return_euler_circle_set_H_fastest(Nontrivial_Test);
    for (auto&& vec : H_fastest) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /// @brief @b Hierholzer_Purely_Self_Constructed
    auto H
        = directed_graph::return_euler_circle_set_H(Nontrivial_Test);
    for (auto&& vec : H) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /// @brief @b Fleury_like
    auto F
        = directed_graph::return_euler_circle_set_F(Nontrivial_Test);
    for (auto&& vec : F) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /**
     * @brief test of @b un_trivial
     * @param Trivial_Test
     */

    directed_graph Trivial_Test = { { 0 } };

    /// @brief @b Hierholzer_Fastest
    auto H_fastest_trivial
        = directed_graph::return_euler_circle_set_H_fastest(Trivial_Test);
    for (auto&& vec : H_fastest_trivial) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /// @brief @b Hierholzer_Purely_Self_Constructed
    auto H_trivial
        = directed_graph::return_euler_circle_set_H(Trivial_Test);
    for (auto&& vec : H_trivial) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /// @brief @b Fleury_like
    auto F_trivial
        = directed_graph::return_euler_circle_set_F(Trivial_Test);
    for (auto&& vec : F_trivial) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /**
     * @brief test of @b un_trivial
     * @param Trivial_Test
     */

    directed_graph UnEuler_Test = {
        { 0, 1, 1 },
        { 0, 0, 1 },
        { 0, 0, 0 },
    };

    /// @brief @b Hierholzer_Fastest
    auto H_fastest_UnEuler
        = directed_graph::return_euler_circle_set_H_fastest(UnEuler_Test);
    for (auto&& vec : H_fastest_UnEuler) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /// @brief @b Hierholzer_Purely_Self_Constructed
    auto H_UnEuler
        = directed_graph::return_euler_circle_set_H(UnEuler_Test);
    for (auto&& vec : H_UnEuler) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /// @brief @b Fleury
    auto F_UnEuler
        = directed_graph::return_euler_circle_set_F(UnEuler_Test);
    for (auto&& vec : F_UnEuler) {
        for (auto&& str : vec) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

} // namespace Test