/**
 * @file MatrixTest.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief Test of the function of `Matrix` module
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../tools/Matrix.hpp"
#include <cassert>

namespace Test {

void MatrixTest() {
    Tool::Matrix<int> test = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    };
    test.echo();

    auto added = test ^ 2;
    added.echo();
    // test.echo();

    added += test;
    added.echo();
    // test.echo();

    added -= test;
    added.echo();
    // test.echo();

    added ^= 2;
    added.echo();
    // test.echo();

    added *= 2;
    added.echo();
    // test.echo();

    auto anotherMat = test * 2.1;
    anotherMat.echo();
    // test.echo();

    // the behavior below will be prohibited
    // auto wrongMat = Matrix<char>::CreateZeroMat();

    auto transposition = anotherMat.transposition();
    transposition.echo();
    // test.echo();
}

} // namespace Test