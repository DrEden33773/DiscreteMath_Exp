#include "../tests/EulerTest_directed.hpp"
#include "../tests/EulerTest_undirected.hpp"
#include "../tests/MatrixTest.hpp"
#include "../tests/UndirectedGraphTest.hpp"
#include "./GraphFactory.hpp"

int main(int argc, char** argv) {
    // Tests below could be recalled!

    // Test::MatrixTest();
    // Test::UndirectedGraphTest();
    // Test::EulerTest_undirected();
    // Test::EulerTest_directed();

    Graph the_graph = GraphFactory::CreateGraph();
    the_graph.show_euler_circle_set_H();
    the_graph.show_euler_circle_set_F();
}
