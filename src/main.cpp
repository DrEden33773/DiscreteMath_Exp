#include "../tools/Matrix.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "hello world!" << std::endl;

    Tool::Matrix<int> test = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    };
}
