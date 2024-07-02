#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string.h>
#include <climits>
#include "./generated_omp/v_cover.cc"
int main() {
    char filePath[] = "./sample_graphs/sample_graph.txt";
    graph g(filePath);
    g.parseGraph();
     std::cout << "Number of nodes: " << g.num_nodes() << std::endl;
     std::cout << "Number of edges: " << g.num_edges() << std::endl;
    double starttime = omp_get_wtime();
    v_cover(g);
    double endtime = omp_get_wtime();

    std::cout<<"\nTime taken : "<<endtime-starttime<<std::endl;
    return 0;
    }

