#ifndef GRAPH_H
#define GRAPH_H
#include "ArraySequence.h"
#include "HashTableDictionary.h"
#include "PriorityQueue.h"
#include <limits>
template<typename T>
class Graph {
private:
    template<typename TDistance>
    class Edge {
    public:
        int start;
        int finish;
        TDistance distance;
        Edge() : start(0), finish(0), distance(TDistance()) {}
        Edge(int start_vertex, int finish_vertex, TDistance dist) : start(start_vertex), finish(finish_vertex), distance(dist) {}
    };

    HashTableDictionary<int, ArraySequence<Edge<T>>> adjList;
    bool isDirected;
public:
    explicit Graph(bool directed = false) : isDirected(directed) {}

    ArraySequence<Edge<T>>& operator[](int vertex) {
        if (!adjList.ContainsKey(vertex)) {
            adjList.Add(vertex, ArraySequence<Edge<T>>());
        }
        return adjList[vertex];
    }

    void AddEdge(int start_vertex, int finish_vertex, T distance) {
        if (!isDirected) {
            Edge<T> reverseEdge(finish_vertex, start_vertex, distance);
            if (!adjList.ContainsKey(finish_vertex)) {
                adjList.Add(finish_vertex, ArraySequence<Edge<T>>());
            }
            adjList[finish_vertex] = adjList[finish_vertex].Append(reverseEdge);
        }

        Edge<T> edge(start_vertex, finish_vertex, distance);
        if (!adjList.ContainsKey(start_vertex)) {
            adjList.Add(start_vertex, ArraySequence<Edge<T>>());
        }
        adjList[start_vertex] = adjList[start_vertex].Append(edge);
    }

    void RemoveEdge(int start_vertex, int finish_vertex) {
        if (adjList.ContainsKey(start_vertex)) {
            for (int i = 0; i < adjList[start_vertex].GetLength(); ++i) {
                if (adjList[start_vertex][i].finish == finish_vertex) {
                    adjList[start_vertex].RemoveAt(i);
                    break;
                }
            }
        }

        if (!isDirected && adjList.ContainsKey(finish_vertex)) {
            for (int i = 0; i < adjList[finish_vertex].GetLength(); ++i) {
                if (adjList[finish_vertex][i].finish == start_vertex) {
                    adjList[finish_vertex].RemoveAt(i);
                    break;
                }
            }
        }
    }

    T GetDistance(int start_vertex, int finish_vertex) {
        if (!adjList.ContainsKey(start_vertex)) {
            throw std::runtime_error("Start vertex not found in the graph");
        }

        auto& edges = adjList[start_vertex];
        for (int i = 0; i < edges.GetLength(); ++i) {
            if (edges[i].finish == finish_vertex) {
                return edges[i].distance;
            }
        }

        if (!isDirected && adjList.ContainsKey(finish_vertex)) {
            auto& reverseEdges = adjList[finish_vertex];
            for (int i = 0; i < reverseEdges.GetLength(); ++i) {
                if (reverseEdges[i].finish == start_vertex) {
                    return reverseEdges[i].distance;
                }
            }
        }

        throw std::runtime_error("Edge not found");
    }

    ShrdPtr<ArraySequence<Edge<T>>> GetAdjList(int vertex) const {
        if (adjList.ContainsKey(vertex)) {
            return ShrdPtr<ArraySequence<Edge<T>>>(new ArraySequence<Edge<T>>(adjList[vertex]));
        } else {
            return ShrdPtr<ArraySequence<Edge<T>>>(new ArraySequence<Edge<T>>());
        }
    }

    const ArraySequence<Edge<T>>& operator[](int vertex) const {
        if (!adjList.ContainsKey(vertex)) {
            static ArraySequence<Edge<T>> emptySequence;
            return emptySequence;
        }
        return adjList[vertex];
    }

    HashTableDictionary<int, int> VertexColoring() {
        HashTableDictionary<int, int> colors;
        auto allItems = adjList.GetAllItems();

        for (int i = 0; i < allItems->GetLength(); ++i) {
            const auto& key = (*allItems)[i];
            colors.Add(key.first, -1);
        }

        for (int i = 0; i < allItems->GetLength(); ++i) {
            const auto& key = (*allItems)[i];
            HashTableDictionary<int, bool> usedColors;

            const auto& edgeList = adjList[key.first];
            for (int j = 0; j < edgeList.GetLength(); ++j) {
                const auto& edge = edgeList[j];
                if (colors.ContainsKey(edge.finish) && colors[edge.finish] != -1) {
                    usedColors.Add(colors[edge.finish], true);
                }
            }

            int color = 0;
            while (usedColors.ContainsKey(color)) {
                ++color;
            }

            colors[key.first] = color;
        }

        return colors;
    }

    bool ContainsVertex(int vertex) const {
        return adjList.ContainsKey(vertex);
    }

    bool HasEdge(int start_vertex, int finish_vertex) const {
        if (!adjList.ContainsKey(start_vertex)) return false;

        auto allItems = adjList.GetAllItems();
        const auto& edges = adjList[start_vertex];
        for (int i = 0; i < edges.GetLength(); ++i) {
            if (edges[i].finish == finish_vertex) return true;
        }
        return false;
    }

    ArraySequence<int> GetAllVertices() const {
        ArraySequence<int> vertices;
        auto items = adjList.GetAllItems();
        for (int i = 0; i < items->GetLength(); ++i) {
            vertices.Append((*items)[i].first);
        }
        return vertices;
    }

    ArraySequence<Edge<T>> GetAllEdges() const {
        ArraySequence<Edge<T>> edges;
        auto items = adjList.GetAllItems();
        for (int i = 0; i < items->GetLength(); ++i) {
            const auto& edgeList = (*items)[i].second;
            for (int j = 0; j < edgeList.GetLength(); ++j) {
                edges.Append(edgeList[j]);
            }
        }
        return edges;
    }

    int Degree(int vertex) const {
        if (!adjList.ContainsKey(vertex)) return 0;
        return adjList[vertex].GetLength();
    }

    Graph<T> Transpose() const {
        Graph<T> transposed(true);
        auto items = adjList.GetAllItems();
        for (int i = 0; i < items->GetLength(); ++i) {
            const auto& edgeList = (*items)[i].second;
            for (int j = 0; j < edgeList.GetLength(); ++j) {
                const auto& edge = edgeList[j];
                transposed.AddEdge(edge.finish, edge.start, edge.distance);
            }
        }
        return transposed;
    }


    HashTableDictionary<int, T> ShortestPaths(int start_vertex) {
    HashTableDictionary<int, T> distances;
    PriorityQueue<int, T> pq;
    auto vertices = GetAllVertices();

    for (int i = 0; i < vertices.GetLength(); ++i) {
        int vertex = vertices[i];
        distances.Add(vertex, std::numeric_limits<T>::max());
    }
    distances[start_vertex] = T();
    pq.Enqueue(start_vertex, T());

    while (pq.GetLength() > 0) {
        int current = pq.Dequeue();
        T current_distance = distances[current];

        for (int j = 0; j < adjList[current].GetLength(); ++j) {
            auto& edge = adjList[current][j];
            T new_distance = current_distance + edge.distance;
            if (new_distance < distances[edge.finish]) {
                distances[edge.finish] = new_distance;
                pq.Enqueue(edge.finish, new_distance);
            }
        }
    }
    return distances;
}

ArraySequence<ArraySequence<int>> ConnectedComponents() {
    HashTableDictionary<int, bool> visited;
    auto vertices = GetAllVertices();
    ArraySequence<ArraySequence<int>> components;

    for (int i = 0; i < vertices.GetLength(); ++i) {
        int vertex = vertices[i];
        visited.Add(vertex, false);
    }

    for (int i = 0; i < vertices.GetLength(); ++i) {
        int vertex = vertices[i];
        if (!visited[vertex]) {
            ArraySequence<int> component;
            ArraySequence<int> queue;
            queue = queue.Append(vertex);

            while (queue.GetLength() > 0) {
                int current = queue[0];
                queue.RemoveAt(0);

                if (!visited[current]) {
                    visited[current] = true;
                    component = component.Append(current);

                    for (int j = 0; j < adjList[current].GetLength(); ++j) {
                        auto& edge = adjList[current][j];
                        if (!visited[edge.finish]) {
                            queue = queue.Append(edge.finish);
                        }
                    }
                }
            }
            components = components.Append(component);
        }
    }
    return components;
}

ArraySequence<ArraySequence<int>> StronglyConnectedComponents() {
    ArraySequence<int> order;
    HashTableDictionary<int, bool> visited;
    auto vertices = GetAllVertices();

    for (int i = 0; i < vertices.GetLength(); ++i) {
        int vertex = vertices[i];
        visited.Add(vertex, false);
    }

    // Первый проход DFS для записи порядка выхода
    auto dfs1 = [&](int v, auto&& dfs1_ref) -> void {
        visited[v] = true;
        for (int j = 0; j < adjList[v].GetLength(); ++j) {
            auto& edge = adjList[v][j];
            if (!visited[edge.finish]) {
                dfs1_ref(edge.finish, dfs1_ref);
            }
        }
        order = order.Append(v);
    };

    for (int i = 0; i < vertices.GetLength(); ++i) {
        int vertex = vertices[i];
        if (!visited[vertex]) {
            dfs1(vertex, dfs1);
        }
    }

    // Транспонирование графа
    Graph<T> transposed = Transpose();

    // Второй проход DFS на транспонированном графе
    for (int i = 0; i < vertices.GetLength(); ++i) {
        int vertex = vertices[i];
        visited[vertex] = false;
    }

    ArraySequence<ArraySequence<int>> components;
    auto dfs2 = [&](int v, auto&& dfs2_ref, ArraySequence<int>& component) -> void {
        visited[v] = true;
        component = component.Append(v);
        for (int j = 0; j < transposed[v].GetLength(); ++j) {
            auto& edge = transposed[v][j];
            if (!visited[edge.finish]) {
                dfs2_ref(edge.finish, dfs2_ref, component);
            }
        }
    };

    for (int i = order.GetLength() - 1; i >= 0; --i) {
        int vertex = order[i];
        if (!visited[vertex]) {
            ArraySequence<int> component;
            dfs2(vertex, dfs2, component);
            components = components.Append(component);
        }
    }

    return components;
}
};
#endif // GRAPH_H
