#include <cassert>
#include <iostream>
#include "Graph.h"
#include "Test.h"
void TestAddAndRemoveEdge() {
    Graph<int> graph(false); // Неориентированный граф

    graph.AddEdge(0, 1, 10);
    graph.AddEdge(1, 2, 20);

    assert(graph.HasEdge(0, 1));
    assert(graph.HasEdge(1, 0));
    assert(graph.GetDistance(0, 1) == 10);
    assert(graph.GetDistance(1, 2) == 20);

    graph.RemoveEdge(1, 2);
    assert(!graph.HasEdge(1, 2));
}

void TestShortestPaths() {
    Graph<int> graph(true); // Ориентированный граф

    graph.AddEdge(0, 1, 5);
    graph.AddEdge(0, 2, 2);
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(1, 3, 2);
    graph.AddEdge(2, 3, 8);

    auto distances = graph.ShortestPaths(0);

    assert(distances[0] == 0);
    assert(distances[1] == 5);
    assert(distances[2] == 2);
    assert(distances[3] == 7); // Кратчайший путь: 0 -> 1 -> 3
}

void TestVertexColoring() {
    Graph<int> graph(false);

    graph.AddEdge(0, 1, 1);
    graph.AddEdge(0, 2, 1);
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(1, 3, 1);

    auto coloring = graph.VertexColoring();

    assert(coloring[0] != coloring[1]);
    assert(coloring[1] != coloring[2]);
    assert(coloring[1] != coloring[3]);
}

void TestConnectedComponents() {
    Graph<int> graph(false);

    graph.AddEdge(0, 1, 1);
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(3, 4, 1);

    auto components = graph.ConnectedComponents();

    assert(components.GetLength() == 2);
    assert(components[0].Contains(0) && components[0].Contains(1) && components[0].Contains(2));
    assert(components[1].Contains(3) && components[1].Contains(4));
}

void TestStronglyConnectedComponents() {
    Graph<int> graph(true);

    graph.AddEdge(0, 1, 1);
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(2, 0, 1);
    graph.AddEdge(3, 4, 1);

    auto components = graph.StronglyConnectedComponents();

    assert(components.GetLength() == 2);
    assert(components[0].Contains(0) && components[0].Contains(1) && components[0].Contains(2));
    assert(components[1].Contains(3) && components[1].Contains(4));
}

void TestTranspose() {
    Graph<int> graph(true);

    graph.AddEdge(0, 1, 1);
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(2, 3, 1);

    auto transposed = graph.Transpose();

    assert(transposed.HasEdge(1, 0));
    assert(transposed.HasEdge(2, 1));
    assert(transposed.HasEdge(3, 2));
    assert(!transposed.HasEdge(0, 1));
}

// void TestMinimumSpanningTree() {
//     Graph<int> graph(false);
//
//     graph.AddEdge(0, 1, 1);
//     graph.AddEdge(1, 2, 2);
//     graph.AddEdge(0, 2, 4);
//     graph.AddEdge(2, 3, 3);
//
//     auto mst = graph.MinimumSpanningTree();
//     int totalWeight = 0;
//
//     for (int i = 0; i < mst.GetLength(); ++i) {
//         totalWeight += mst[i].distance;
//     }
//
//
//     assert(totalWeight == 6);
// }

void TestDegree() {
    Graph<int> graph(false);

    graph.AddEdge(0, 1, 1);
    graph.AddEdge(0, 2, 1);

    assert(graph.Degree(0) == 2);
    assert(graph.Degree(1) == 1);
}

void TestContainsVertex() {
    Graph<int> graph(false);

    graph.AddEdge(0, 1, 1);
    assert(graph.ContainsVertex(0));
    assert(graph.ContainsVertex(1));
    assert(!graph.ContainsVertex(2));
}

void TestGetAllVerticesAndEdges() {
    Graph<int> graph(false);

    graph.AddEdge(0, 1, 1);
    graph.AddEdge(1, 2, 2);

    auto vertices = graph.GetAllVertices();
    auto edges = graph.GetAllEdges();

    assert(vertices.Contains(0));
    assert(vertices.Contains(1));
    assert(vertices.Contains(2));
    assert(edges.GetLength() == 4); // Неориентированный граф: ребра в обе стороны
}

void Test() {
    TestAddAndRemoveEdge();
    std::cout << "TestAddAndRemoveEdge passed!\n";

    TestShortestPaths();
    std::cout << "TestShortestPaths passed!\n";

    TestVertexColoring();
    std::cout << "TestVertexColoring passed!\n";

    TestConnectedComponents();
    std::cout << "TestConnectedComponents passed!\n";

    TestStronglyConnectedComponents();
    std::cout << "TestStronglyConnectedComponents passed!\n";

    TestTranspose();
    std::cout << "TestTranspose passed!\n";

    // TestMinimumSpanningTree();
    // std::cout << "TestMinimumSpanningTree passed!\n";

    TestDegree();
    std::cout << "TestDegree passed!\n";

    TestContainsVertex();
    std::cout << "TestContainsVertex passed!\n";

    TestGetAllVerticesAndEdges();
    std::cout << "TestGetAllVerticesAndEdges passed!\n";

}
