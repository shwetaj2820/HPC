#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string.h>
#include <climits>
#include "./generated_omp/PageRankDSLV3.cc"
int main() {
    char filePath[] = "./sample_graphs/sample_graph.txt";
    graph g(filePath);
    g.parseGraph();
     std::cout << "Number of nodes: " << g.num_nodes() << std::endl;
    std::cout << "Number of edges: " << g.num_edges() << std::endl;
    float beta = 1e-6; // Convergence threshold for large graphs
    float delta = 0.85; // Damping factor
    int maxIter = 150; // Maximum number of iterations

    float* pageRank = new float[g.num_nodes()]; // Allocate memory for PageRank array

    double starttime = omp_get_wtime();
   ComputePageRank(g, beta, delta, maxIter, pageRank);
   double endtime = omp_get_wtime();
    for (int i = 0; i < g.num_nodes(); i++) {
        std::cout << "Node " << i << ": " << pageRank[i] << std::endl;
    }
    std::cout<<"Time taken : "<<endtime-starttime<<std::endl;
    return 0;
}
