#include <queue>
#include "parser.h"
#include "graph.h"
#include <vector>

using namespace std;

Graph CreateGraph(vector<Configuration> configs) {//вектор содержит информацио о конфигурациях
    Graph graph(configs.size() + 1);
    for (auto config: configs) {
        for (auto parent: config.parents) {
            //ид дочерней конфигурации добавляется в вектор
            if (parent == 0) {
                graph.end_jobs_id.push_back(config.id);
            }
            //для каждой конфигурации конкретного родителя создается узел
            graph.addNode(parent, config.id);
        }
    }

    if (graph.end_jobs_id.empty()) {
        cout << "Graph doesn't have End Jobs\n";
        exit(1);
    }
    return graph;
}

//проверка циклов,используя рекурсивный обход в глубину
int IsCycle(int vertex, Graph &graph, vector<int> &visited) {
	visited[vertex] = 1;
    int result = 0;
	for (auto neighbor: graph.adjacency[vertex]) {
		if (visited[neighbor] == 0) {
			result = IsCycle(neighbor, graph, visited);
            if (result) {
                break;
            }
        }
		else if (visited[neighbor] == 1) {
			result = 1;
            break;
		}
	}
    visited[vertex] = 2;
    return result;
}
 //обход графа в ширину
vector<int> BFS(Graph &graph, vector<int> &visited) {
    vector<int> path;   
    queue<int> line;
    int current_vertex;
    for (auto id: graph.end_jobs_id) {
        line.push(id); // в начало очереди конечные задания из графа 
    }
    while (!line.empty()) {
        current_vertex = line.front();
        line.pop();
        path.push_back(current_vertex);//обрабатываются соседи
        for (int neighbor : graph.adjacency[current_vertex]) {
            if (visited[neighbor] == 0) {
                line.push(neighbor);
                visited[neighbor] = 1;
            }
        }
    }
    return path;
}
