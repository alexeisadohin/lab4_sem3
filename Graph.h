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
            adjList[finish_vertex].Append(reverseEdge);
        }

        Edge<T> edge(start_vertex, finish_vertex, distance);
        if (!adjList.ContainsKey(start_vertex)) {
            adjList.Add(start_vertex, ArraySequence<Edge<T>>());
        }
        adjList[start_vertex].Append(edge);
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

    ShrdPtr<ArraySequence<Edge<T>>> GetAdjList(int vertex) const{
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

    HashTableDictionary<int, int> ShortestPaths(int start_vertex) {
        HashTableDictionary<int, T> distances;
        HashTableDictionary<int, bool> visited;
        PriorityQueue<int, T> pq;

        auto allItems = adjList.GetAllItems();
        for (int i = 0; i < allItems->GetLength(); ++i) {
            distances.Add((*allItems)[i].first, std::numeric_limits<T>::max());
            visited.Add((*allItems)[i].first, false);
        }

        distances[start_vertex] = 0;
        pq.Enqueue(start_vertex, 0);

        while (pq.GetLength() > 0) {
            int current = pq.Dequeue();

            if (visited[current]) continue;
            visited[current] = true;

            auto& edges = adjList[current];
            for (int i = 0; i < edges.GetLength(); ++i) {
                const auto& edge = edges[i];
                T newDist = distances[current] + edge.distance;
                if (newDist < distances[edge.finish]) {
                    distances[edge.finish] = newDist;
                    pq.Enqueue(edge.finish, newDist);
                }
            }

        }

        return distances;
    }

    ArraySequence<ArraySequence<int>> ConnectedComponents() {
        ArraySequence<ArraySequence<int>> components;
        HashTableDictionary<int, bool> visited;

        auto allItems = adjList.GetAllItems();
        for (int i = 0; i < allItems->GetLength(); ++i) {
            visited.Add((*allItems)[i].first, false);
        }

        // Исправление итерации
        for (int i = 0; i < allItems->GetLength(); ++i) {
            int vertex = (*allItems)[i].first;
            if (!visited[vertex]) {
                ArraySequence<int> component;
                PriorityQueue<int, int> queue;

                queue.Enqueue(vertex, 0);
                visited[vertex] = true;

                while (queue.GetLength() > 0) {
                    int current = queue.Dequeue();
                    component.Append(current);

                    for (int j = 0; j < adjList[current].GetLength(); ++j) {
                        const auto& edge = adjList[current][j];
                        if (!visited[edge.finish]) {
                            queue.Enqueue(edge.finish, 0);
                            visited[edge.finish] = true;
                        }
                    }
                }
                components.Append(component);
            }
        }
        return components;
    }


    HashTableDictionary<int, int> VertexColoring() {
        HashTableDictionary<int, int> colors;
        auto allItems = adjList.GetAllItems();

        // Устанавливаем начальный цвет для всех вершин
        for (int i = 0; i < allItems->GetLength(); ++i) {
            const auto& key = (*allItems)[i];
            colors.Add(key.first, -1);
        }

        // Раскраска вершин
        for (int i = 0; i < allItems->GetLength(); ++i) {
            const auto& key = (*allItems)[i];
            HashTableDictionary<int, bool> usedColors;

            // Проверяем цвета соседей
            const auto& edgeList = adjList[key.first];
            for (int j = 0; j < edgeList.GetLength(); ++j) {
                const auto& edge = edgeList[j];
                if (colors.ContainsKey(edge.finish) && colors[edge.finish] != -1) {
                    usedColors.Add(colors[edge.finish], true);
                }
            }

            // Выбираем минимальный доступный цвет
            int color = 0;
            while (usedColors.ContainsKey(color)) {
                ++color;
            }

            colors[key.first] = color;
        }

        return colors;
    }


    // Новый метод: Проверка наличия вершины
    bool ContainsVertex(int vertex) const {
        return adjList.ContainsKey(vertex);
    }

    // Новый метод: Проверка наличия ребра
    bool HasEdge(int start_vertex, int finish_vertex) const {
        if (!adjList.ContainsKey(start_vertex)) return false;
        for (const auto& edge : adjList[start_vertex]) {
            if (edge.finish == finish_vertex) return true;
        }
        return false;
    }

    // Новый метод: Получить список всех вершин
    ArraySequence<int> GetAllVertices() const {
        ArraySequence<int> vertices;
        for (const auto& item : adjList.GetAllItems()) {
            vertices.Append(item.first);
        }
        return vertices;
    }

    // Новый метод: Получить список всех рёбер
    ArraySequence<Edge<T>> GetAllEdges() const {
        ArraySequence<Edge<T>> edges;
        for (const auto& item : adjList.GetAllItems()) {
            for (const auto& edge : item.second) {
                edges.Append(edge);
            }
        }
        return edges;
    }

    // Новый метод: Получить степень вершины
    int Degree(int vertex) const {
        if (!adjList.ContainsKey(vertex)) return 0;
        return adjList[vertex].GetLength();
    }

    // Новый метод: Проверка на связность
    bool IsConnected() {
        auto components = ConnectedComponents();
        return components.GetLength() == 1;
    }

    // Новый метод: Транспонирование графа
    Graph<T> Transpose() const {
        Graph<T> transposed(true);
        for (const auto& item : adjList.GetAllItems()) {
            for (const auto& edge : item.second) {
                transposed.AddEdge(edge.finish, edge.start, edge.distance);
            }
        }
        return transposed;
    }

};
#endif // GRAPH_H
