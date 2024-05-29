//code for solving problems in optimization 1:

// PROBLEM1- wrong traversal in small graph of 5 nodes:
// solution - using mutex lock to manage concurrent access.
// persisting problem in this solution- still there are trailing 1's in the end of traversal output when we input large graphs 

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<omp.h>
#include<mutex>
#include<unordered_set>

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
    mutex queueMutex;
    currentLevel.push(startVertex); //pushing start vertex on the queue currentLevel and marking it as visited
    visited[startVertex] = true;

    while (!currentLevel.empty()) {
        vector<int>temp_currentLevel;

        // extracting all nodes from current level
        while(!currentLevel.empty()){
            temp_currentLevel.push_back(currentLevel.front());
            currentLevel.pop();
        }

            #pragma omp parallel for
            for (int i = 0; i < temp_currentLevel.size(); ++i) {
                int v = temp_currentLevel[i];
                #pragma omp critical
                {
                    cout << v << " ";
                }


            // processing all the adjacent nodes
                for (int neighbor : adjList[v]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                queueMutex.lock();
                                nextLevel.push(neighbor);
                                queueMutex.unlock();
                            }
                        }
                    }
                }
            }


        swap(currentLevel, nextLevel);
    }
}

int main() {
    // Opening file to read edges
    ifstream infile("edgelist.txt");
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
        obj.bfs(startNode); 
    }

    return 0;
}
