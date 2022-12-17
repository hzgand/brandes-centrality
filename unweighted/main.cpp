#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

void generateGridGraph(int rows, int cols, string filename) {
  ofstream fout(filename);
  int N = rows*cols;
  int M = (rows-1)*cols + (cols-1)*rows;
  fout << N << " " << M << endl;
  for (int row=1; row<=rows; row++) {
    for (int col=1; col<=cols; col++) {
      int cur = (row-1)*cols + col;
      int right = (row-1)*cols + col+1; 
      int down = (row)*cols + col;
      if (col != cols) fout << cur << " " << right << endl;
      if (row != rows) fout << cur << " " << down << endl;
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
      
        vector<int> adj[N];
        for (int i=0; i<M; i++) {
          int u, v;
          fin >> u >> v;
          u--; v--;
          adj[u].push_back(v);
          adj[v].push_back(u);
        }
    
        fin.close();
    
        // Randomly select nodes s and t
        int s = (rand() % N);
        int t;
        do {
          t = (rand() % N);
        } while (s==t);
    
        // Count shortest paths from s to t
        int d[N];
        int sigma[N];
        for (int i=0; i<N; i++) {
          d[i] = -1;
          sigma[i] = 0;
        }
        
        d[s] = 0;
        sigma[s] = 1;
        
        queue<int> Q;
        Q.push(s);
        
        while (!Q.empty()) {
          int v = Q.front(); Q.pop();
          for (int w : adj[v]) {
            // w found for the first time?
            if (d[w] < 0) {
              Q.push(w);
              d[w] = d[v] + 1;
            }
    
            // shortest path to w via v?
            if (d[w] == d[v] + 1) {
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