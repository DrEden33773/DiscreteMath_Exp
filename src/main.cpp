#include "../tools/Matrix.hpp"

using Tool::Matrix;

int main(int argc, char** argv) {
    Matrix<int> test = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    };
    test.echo();

    auto added = test * test;
    added.echo();

    added += test;
    added.echo();

    added -= test;
    added.echo();

    added ^= 2;
    added.echo();
}
