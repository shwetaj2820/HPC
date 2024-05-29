// optimization 1:
// 1. using adjacency list (memory efficient for sparse matrix)
// 2. 2 queues (for managing BFS levels)
// 3. #pragma omp parallel , #pragma omp for to paralelize the processing of nodes at current level.
// (every thread processes a subset of nodes and collects the nodes for next level in thread-local set to avoid race conditions)
// 4. synchronisation- #pragma omp critical (to safely update data structures)

// achieved - can process large graphs because of using chunk_size
// PROBLEM1- order in which nodes enqueued and dequeued in the parallel  region (hence wrong traversal even in small graph of 5 edges)
// PROBLEM2- large number of 1's printed at the end of output
// due to :improper handling of the queue and synchronization in the OpenMP parallel region. 
// Specifically, the parallel processing of the queue and the use of #pragma omp critical not be managing the state of the currentLevel queue correctly.
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <omp.h>
#include <unordered_set>

using namespace std;

class BFS {
    vector<vector<int>> adjList; // adjacency list
    int numNodes;

public:
    BFS(int numNodes) : numNodes(numNodes) //initializer list
    {
        adjList.resize(numNodes);
    }

    void input(const vector<pair<int, int>>& edges);
    void bfs(int startVertex);
};

// undirected graph
void BFS::input(const vector<pair<int, int>>& edges) {
    for (const auto& edge : edges) {
        adjList[edge.first].push_back(edge.second);
        adjList[edge.second].push_back(edge.first);
    }
}

void BFS::bfs(int startVertex) {
    vector<bool> visited(numNodes, false);
    queue<int> currentLevel, nextLevel; 

    currentLevel.push(startVertex); //pushing start vertex on the queue currentLevel and marking it as visited
    visited[startVertex] = true;

    while (!currentLevel.empty()) {
        #pragma omp parallel
        {
            unordered_set<int> localNextLevel;

            #pragma omp for
            for (int i = 0; i < currentLevel.size(); ++i) {
                int v;
                #pragma omp critical
                {
                    if (!currentLevel.empty()) {
                        v = currentLevel.front();
                        currentLevel.pop();
                    } else {
                        continue;
                    }
                }

                cout << v << " ";

                for (int neighbor : adjList[v]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                localNextLevel.insert(neighbor);
                            }
                        }
                    }
                }
            }

            #pragma omp critical
            {
                for (int node : localNextLevel) {
                    nextLevel.push(node);
                }
            }
        }

        swap(currentLevel, nextLevel);
        while (!nextLevel.empty()) {
            nextLevel.pop();
        }
    }
}

int main() {
    // Opening file to read edges
    ifstream infile("com-youtube.ungraph.txt");
    if (!infile) {
        cerr << "File not found";
        return 1;
    }

    const int CHUNK_SIZE = 10000; // processing graph in chunks (to perform BFS on each chunk)
    int u, v;
    vector<pair<int, int>> edges;
    int maxNodeId =0;
    // Read the first edge to determine the starting node
    if (!(infile >> u >> v)) {
        cerr << "No edges found in the file";
        return 1;
    }

    int startNode = u;
    edges.push_back({u, v});
    maxNodeId = max(u, v);

    while (infile >> u >> v) {
        edges.push_back({u, v});
        maxNodeId = max(maxNodeId, max(u, v));
        if (edges.size() >= CHUNK_SIZE) {
            BFS obj(maxNodeId + 1); // Create BFS object with maximum node ID seen so far
            obj.input(edges);
            obj.bfs(startNode);
            edges.clear();
        }
    }
    infile.close();

    // Process remaining edges if any
    if (!edges.empty()) {
        BFS obj(maxNodeId + 1); // Create BFS object with maximum node ID seen
        obj.input(edges);
        obj.bfs(startNode); // Assuming starting BFS from node 0
    }

    return 0;
}
