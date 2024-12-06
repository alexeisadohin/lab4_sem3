#include <cassert>
#include <iostream>
#include "Graph.h"
#include "Test.h"
void TestAddEdge() {
    Graph<int> graph;
    graph.AddEdge(1, 2, 10);

    graph.AddEdge(1, 3, 15);
    std::cout<<graph.GetDistance(1, 2)<<std::endl;
    assert(graph.GetDistance(1, 2) == 10);
    assert(graph.GetDistance(1, 3) == 15);
    assert(graph.GetDistance(2, 1) == 10); // Неориентированный граф
    std::cout << "TestAddEdge passed!" << std::endl;
}

void TestRemoveEdge() {
    Graph<int> graph;
    graph.AddEdge(1, 2, 10);
    graph.RemoveEdge(1, 2);

    bool exceptionThrown = false;
    try {
        graph.GetDistance(1, 2);
    } catch (const std::runtime_error& error) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        graph.GetDistance(2, 1);
    } catch (const std::runtime_error& error) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "TestRemoveEdge passed!" << std::endl;
}

void TestShortestPaths() {
    Graph<int> graph(true); // Ориентированный граф
    graph.AddEdge(1, 2, 5);
    graph.AddEdge(2, 3, 10);
    graph.AddEdge(1, 3, 20);
    auto distances = graph.ShortestPaths(1);
    assert(distances[1] == 0);
    assert(distances[2] == 5);
    assert(distances[3] == 15);
    std::cout << "TestShortestPaths passed!" << std::endl;
}

void TestConnectedComponents() {
    Graph<int> graph(false); // Неориентированный граф
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(3, 4, 1);

    auto components = graph.ConnectedComponents();
    assert(components.GetLength() == 2);

    auto Contains = [](const ArraySequence<int>& sequence, int value) -> bool {
        for (int i = 0; i < sequence.GetLength(); ++i) {
            if (sequence[i] == value) {
                return true;
            }
        }
        return false;
    };

    // Проверка, что каждая компонента содержит свои вершины
    bool component1_valid = Contains(components[0], 1) && Contains(components[0], 2);
    bool component2_valid = Contains(components[1], 1) && Contains(components[1], 2);
    assert(component1_valid || component2_valid);

    bool component3_valid = Contains(components[0], 3) && Contains(components[0], 4);
    bool component4_valid = Contains(components[1], 3) && Contains(components[1], 4);
    assert(component3_valid || component4_valid);

    std::cout << "TestConnectedComponents passed!" << std::endl;
}



void TestVertexColoring() {
    Graph<int> graph;
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(2, 3, 1);
    graph.AddEdge(3, 4, 1);
    graph.AddEdge(4, 1, 1);
    auto colors = graph.VertexColoring();
    assert(colors[1] != colors[2]);
    assert(colors[2] != colors[3]);
    assert(colors[3] != colors[4]);
    assert(colors[4] != colors[1]);
    std::cout << "TestVertexColoring passed!" << std::endl;
}

void Test() {
    TestAddEdge();
    TestRemoveEdge();
    TestShortestPaths();
    TestConnectedComponents();
    // TestMinimumSpanningTree();
    TestVertexColoring();
    std::cout << "All tests passed!" << std::endl;
}
