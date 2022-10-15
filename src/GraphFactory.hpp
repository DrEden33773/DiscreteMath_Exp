/**
 * @file GraphFactory.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief A Factory to construct graph
 * @note
        @b type_of_graph @e (undirected_or_directed) could be judged by the input matrix
            @b BUT , sometimes you still need to @p designate the @b type
                @b {{0,1},{1,0}} should be a example
                    @e could_be_designated_as_directed_graph
                    @e will_be_declared_as_undirected_graph
            @b RULE_OF_JUDGE:
                @b if the @e Matrix is @b Axially_Symmetrical => @p Judged_as @e Undirected
                @b if the @e Matrix is @b Non_Symmetrical => @p Judged_as @e directed
                @b if you've @p designated the @e type => @p Abandon_Judgement

 * @version 0.1
 * @date 2022-10-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "../tools/Matrix.hpp"
#include "../tools/directed_graph.hpp"
#include "../tools/undirected_graph.hpp"

class GraphFactory {
    enum class graph_type : unsigned short {
        undirected = 0,
        directed   = 1,
    };
    GraphFactory() = default;

public:
    ~GraphFactory() = default;

    static GraphFactory CreateGraphInterface() {
    }
};