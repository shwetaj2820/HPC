#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<queue>
#include<omp.h>
using namespace std;

class BFS{
  vector<vector<unsigned long long int>>arr; //adjacency matrix
  vector<unsigned long long int>q; //queue used for bfs
  unsigned long long e1,e2,front,back;

  public:
        BFS(){
          front=0;
          back=0;
        }

        void input(const vector<pair<unsigned long long int, unsigned long long int>>& edges);
        // void output();
        void bfs();
};

void BFS::input(const vector<pair<unsigned long long int, unsigned long long int>>& edges){
  unsigned long long int nodes = 0;
  for(const auto& edge: edges){
    nodes = max({nodes, edge.first, edge.second});
  }
  nodes++; //for 0-based indexing

  arr.assign(nodes, vector<unsigned long long int>(nodes, 0));

        // Populate the adjacency matrix based on the input edges
        for (const auto& edge : edges) {
            arr[edge.first][edge.second] = 1;
            arr[edge.second][edge.first] = 1; // For undirected graph
        }
}

// void BFS::output(){
//   for(const auto& row:arr){
//     for(const auto& element:row){
//       cout<<element<<" ";
//     }
//     cout<<endl;
//   }
// }

void BFS::bfs(){
  vector<bool> visited(arr.size(), false); // Mark all vertices as not visited
    queue<unsigned long long int> q; // Create a queue for BFS
    unsigned long long int startVertex = 0; // Assuming we start BFS from vertex 0
    visited[startVertex] = true;
    q.push(startVertex);

    while (!q.empty()) {
        // Get the current vertex from the queue and print it
        unsigned long long int v = q.front();
        cout << v << " ";
        q.pop();

        // Process adjacent vertices of the current vertex in parallel
#pragma omp parallel for
        for (unsigned long long int i = 0; i < arr.size(); ++i) {
            // Check if vertex i is adjacent to v and has not been visited yet
            if (arr[v][i] && !visited[i]) {
                // Mark vertex i as visited and enqueue it
// #pragma omp critical
                // {
                    visited[i] = true;
                    q.push(i);
                // }
            }
        }
    }
}

int main(){

  // opening file to read edges:
  ifstream infile("com-youtube.ungraph.txt");
  if(!infile){
    cerr<<"file not found";
    return 1;
  }

  unsigned long long int u,v;
  vector<pair<unsigned long long int,unsigned long long int>>edges;
  while(infile >> u >> v){
    edges.push_back({u,v});
  }
  infile.close();

  BFS obj;
  obj.input(edges);
  // obj.output();
  cout<<"output is:\n";
  obj.bfs();
}