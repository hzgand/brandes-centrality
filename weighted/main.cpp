#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

void generateGridGraph(int rows, int cols, string filename) {
  srand((unsigned) time(NULL));
  int weight;
  ofstream fout(filename);
  int N = rows*cols;
  int M = (rows-1)*cols + (cols-1)*rows;
  fout << N << " " << M << endl;
  for (int row=1; row<=rows; row++) {
    for (int col=1; col<=cols; col++) {
      int cur = (row-1)*cols + col;
      int right = (row-1)*cols + col+1; 
      int down = (row)*cols + col;
      weight = rand() % 9 + 1;
      if (col != cols) fout << cur << " " << right << " " << weight << endl;
      weight = rand() % 9 + 1;
      if (row != rows) fout << cur << " " << down << " " << weight << endl;
    }
  }
  fout.close();
}

int main() {
  string inFile = "shortcount.in";
  string outFile = "running_time.csv";

  ofstream fout(outFile);

  fout << "N, Running Time" << endl;

  srand((unsigned) time(NULL));

  for(int graphRows = 6; graphRows <= 60; graphRows += 2) {
    for(int graphCols = graphRows; graphCols <= 60; graphCols += 2) {
      int trials = 10;
      while (trials--) {
    
        // Generate new graph
        generateGridGraph(graphRows, graphCols, inFile);
        
        // BEGIN TIME - Algorithm
        const clock_t begin_time = clock();
    
        // Read in graph data
        fstream fin(inFile);
        int N, M;
        fin >> N >> M;
      
        vector<pair<int, int>> adj[N];
        for (int i=0; i<M; i++) {
          int u, v, w;
          fin >> u >> v >> w;
          u--; v--;
          adj[u].push_back(make_pair(v, w));
          adj[v].push_back(make_pair(u, w));
        }
    
        fin.close();
    
        // Randomly select nodes s and t
        int s = rand() % N;
        int t;
        do {
          t = (rand() % N);
        } while (s==t);
    
        // Count shortest paths from s to t
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
        
        int d[N];
        int sigma[N];
        for (int i=0; i<N; i++) {
          d[i] = INT32_MAX;
          sigma[i] = 0;
        }
        
        d[s] = 0;
        sigma[s] = 1;

        Q.push(make_pair(d[s], s));
        
        // Dijkstra's Algorithm
        while (!Q.empty()) {
          pair<int, int> length_node = Q.top(); Q.pop();
          int v = length_node.second;
          int length = length_node.first;
          if(length != d[v]) continue; // This entry in Q is outdated

          for (pair<int, int> node_weight : adj[v]) {
            int w = node_weight.first;
            int edgeWeight = node_weight.second;

            // new shortest path to w via v?
            if (d[w] > d[v] + edgeWeight) {
              d[w] = d[v] + edgeWeight;
              sigma[w] = 0;
              Q.push(make_pair(d[w], w));
            }

            // current shortest path from w via v?
            if (d[w] == d[v] + edgeWeight) {
              sigma[w] = sigma[w] + sigma[v];
            }
          }
        }

        int ans = sigma[t];
        
        // END TIME - Algorithm
        const clock_t end_time = clock();
        float elapsedTimeMicro = float(end_time - begin_time)/CLOCKS_PER_SEC * 1e6;
    
        fout << N << ", " << elapsedTimeMicro << endl;
        
      }
    }
  }
  fout.close();
  return 0;
  
}