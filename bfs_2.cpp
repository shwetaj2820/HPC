// optimization 2: use of string buffer
// use of queueMutex: to avoid race conditions and control access to the shared queue use of "lock_guard<mutex> lock(queueMutex)" 
// reading graph in small chunk sizes and applying parallel bfs on it. 
//Each thread checks and updates the visited status of a node atomically, and only if the node has not been visited before, it proceeds to enqueue the node for traversal.
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<unordered_set>
#include<mutex>
#include<sstream>
#include<chrono>
#include<omp.h>

using namespace std;
using namespace std::chrono;

class BFS {
    vector<vector<int>> adjList; // adjacency list
    unordered_set<int> nodes; // set to keep track of encountered nodes
    mutex queueMutex; // Mutex for synchronizing access to the queue

public:
    void input(const vector<pair<int, int>>& edges);
    double bfs(int startVertex);
    // void bfs(int startVertex);
};

void BFS::input(const vector<pair<int, int>>& edges) {
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;

        nodes.insert(u);
        nodes.insert(v);

        if (u >= adjList.size()) {
            adjList.resize(u + 1);
        }
        if (v >= adjList.size()) {
            adjList.resize(v + 1);
        }

        adjList[u].push_back(v);
        adjList[v].push_back(u); 
    }
}

// double BFS::bfs(int startVertex) {
double BFS::bfs(int startVertex){
    auto bfs_start = high_resolution_clock::now(); // Start measuring BFS computation time
    if (startVertex >= adjList.size()) {
        cerr << "Start vertex not found in the graph" << endl;
        return 0.0;
    }

    vector<bool> visited(adjList.size(), false);
    queue<int> q;
    vector<int> result;

    q.push(startVertex);
    visited[startVertex] = true;

    while (!q.empty()) {
        int v;
        {
            lock_guard<mutex> lock(queueMutex);
            if (q.empty()) continue;
            v = q.front();
            q.pop();
        }

        result.push_back(v);

        #pragma omp parallel for
        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                bool expected = false;
                if (visited[neighbor] == expected) {
                    visited[neighbor] = true;
                    lock_guard<mutex> lock(queueMutex);
                    q.push(neighbor);
                }
            }
        }
    }
    // Printing BFS traversal order
    std::ostringstream buffer;
    for(int node:result){
        buffer<<node<<" ";
    }
    cout<<buffer.str()<<endl;
    // cout << endl;
    auto bfs_stop = high_resolution_clock::now(); // Stop measuring BFS computation time
    auto bfs_duration = duration_cast<milliseconds>(bfs_stop - bfs_start);
    return bfs_duration.count()/60000.0; // return BFS computation time in minutes
}

int main() {
    // Opening file to read edges:
    ifstream infile("as-skitter.txt");
    if (!infile) {
        cerr << "File not found" << endl;
        return 1;
    }

    const int CHUNK_SIZE = 10000; // Adjusting chunk size (10000 edges) to read edges
    int u, v;
    vector<pair<int, int>> edges;
    double total_bfs_time = 0.0;

    while (infile >> u >> v) {
        edges.push_back({u, v});

        if (edges.size() >= CHUNK_SIZE) {
            BFS obj;
            obj.input(edges);
            total_bfs_time += obj.bfs(u); // accumulating BFS computation time
            obj.bfs(u);
            edges.clear();
        }
    }
    infile.close();

    // Processing remaining edges at last if any
    if (!edges.empty()) {
        BFS obj;
        obj.input(edges);
        total_bfs_time += obj.bfs(u); // accumulating BFS computation time of remaining edges
        obj.bfs(u);
    }

    cout << "Total BFS computation time: " << total_bfs_time << " minutes" << endl;

    return 0;
}
