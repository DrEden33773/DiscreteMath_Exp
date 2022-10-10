#include "../tests/EulerTest_directed.hpp"
#include "../tests/EulerTest_undirected.hpp"
#include "../tests/MatrixTest.hpp"
#include "../tests/UndirectedGraphTest.hpp"

int main(int argc, char** argv) {
    // Test::MatrixTest();
    // Test::UndirectedGraphTest();
    Test::EulerTest_undirected();
    Test::EulerTest_directed();
}
