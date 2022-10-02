#include "../tools/Matrix.hpp"
#include <iostream>

using Tool::Matrix;

int main(int argc, char** argv) {
    std::cout << "hello world!" << std::endl;

    Matrix<> test = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    };

    auto added = test * test;
}
