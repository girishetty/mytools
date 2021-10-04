// Program that runs Graph functionalities.
// https://www.hackerrank.com/challenges/unique-colors/problem
// You are given an unrooted tree of n nodes numbered from 1 to n. Each node i has a color, C[i].
// Let d[i, j] be the number of different colors in the path between node i and node j.
// For each node i, calculate the value of sum[i], defined as follows:
// sum[i] = sumof(d[i,j]) where 1 <= j <= n.

/**
Sample Input:
5
1 2 3 2 3
1 2
2 3
2 4
1 5

Sample Output:
10
9
11
9
12


Sample Input:
10
10 8 1 8 5 8 2 9 7 9
8 9
8 6
8 4
9 1
4 7
9 3
4 10
10 5
5 2

Sample Output:
36
33
36
26
32
27
34
24
28
26
*/

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Graph.h"

using std::cin;
using std::min;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::streamsize;
using std::numeric_limits;

#ifdef USE_GRAPH
vector<int> solve(const vector<int>& c, const vector<vector<int>>& tree) {
  Graph g(c);
  for (auto& edge : tree) {
    g.addEdge(edge[0] - 1, edge[1] - 1);
    g.addEdge(edge[1] - 1, edge[0] - 1);
  }

  vector<int> soln;
  auto graphSize = g.getSize();
  vector<vector<size_t>> dp (graphSize, vector<size_t>(graphSize, 0));
  for (size_t s = 0; s < graphSize; s++) {
    int sum = 0;
    for (size_t d = 0; d < graphSize; d++) {
      if (d >= s) {
        dp[s][d] = g.countUniqueVerticies(s, d);
        // Its a mirror martix
        dp[d][s] = dp[s][d];
      }
      sum += dp[s][d];
    }
    soln.push_back(sum);
  }
  return soln;
}
#else
vector<int> solve(const vector<int>& c, const vector<vector<int>>& tree) {
  Graph g(c);
  for (auto& edge : tree) {
    g.addEdge(edge[0] - 1, edge[1] - 1);
    g.addEdge(edge[1] - 1, edge[0] - 1);
  }

  vector<int> soln;
  auto graphSize = g.getSize();
  vector<vector<size_t>> dp (graphSize, vector<size_t>(graphSize, 0));
  for (size_t s = 0; s < graphSize; s++) {
    int sum = 0;
    for (size_t d = 0; d < graphSize; d++) {
      if (d >= s) {
        dp[s][d] = g.countUniqueVerticies(s, d);
        // Its a mirror martix
        dp[d][s] = dp[s][d];
      }
      sum += dp[s][d];
    }
    soln.push_back(sum);
  }
  return soln;
}
#endif

void readFromConsole(vector<int>& c, vector<vector<int>>& tree);
void readFromFile(const char* file, vector<int>& c, vector<vector<int>>& tree);
vector<string> split_string(string);

int main(int argc, char** argv) {
  vector<int> c;
  vector<vector<int>> tree;

  if (argc == 3) {
    readFromFile(argv[2], c, tree);
  } else {
    readFromConsole(c, tree);
  }
  vector<int> result = solve(c, tree);

  for (int result_itr = 0; result_itr < result.size(); result_itr++) {
    cout << result[result_itr];

    if (result_itr != result.size() - 1) {
      cout << "\n";
    }
  }

  cout << "\n";

  return 0;
}

void readFromFile(const char* file, vector<int>& c, vector<vector<int>>& tree) {
  int c_count;
  std::ifstream fileIn(file, std::ifstream::binary);
  if (!fileIn.is_open()) {
    return readFromConsole(c, tree);
  }
  fileIn >> c_count;
  fileIn.ignore(numeric_limits<streamsize>::max(), '\n');

  string c_temp_temp;
  getline(fileIn, c_temp_temp);

  vector<string> c_temp = split_string(c_temp_temp);

  c.resize(c_count);

  for (int c_itr = 0; c_itr < c_count; c_itr++) {
    int c_item = stoi(c_temp[c_itr]);

    c[c_itr] = c_item;
  }

  tree.resize(c_count - 1);
  for (int tree_row_itr = 0; tree_row_itr < c_count - 1; tree_row_itr++) {
    tree[tree_row_itr].resize(2);

    for (int tree_column_itr = 0; tree_column_itr < 2; tree_column_itr++) {
      fileIn >> tree[tree_row_itr][tree_column_itr];
    }

    fileIn.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

void readFromConsole(vector<int>& c, vector<vector<int>>& tree) {
  int c_count;
  cin >> c_count;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  string c_temp_temp;
  getline(cin, c_temp_temp);

  vector<string> c_temp = split_string(c_temp_temp);

  c.resize(c_count);

  for (int c_itr = 0; c_itr < c_count; c_itr++) {
    int c_item = stoi(c_temp[c_itr]);

    c[c_itr] = c_item;
  }

  tree.resize(c_count - 1);
  for (int tree_row_itr = 0; tree_row_itr < c_count - 1; tree_row_itr++) {
    tree[tree_row_itr].resize(2);

    for (int tree_column_itr = 0; tree_column_itr < 2; tree_column_itr++) {
      cin >> tree[tree_row_itr][tree_column_itr];
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

vector<string> split_string(string input_string) {
  string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
    return x == y and x == ' ';
  });

  input_string.erase(new_end, input_string.end());

  while (input_string[input_string.length() - 1] == ' ') {
    input_string.pop_back();
  }

  vector<string> splits;
  char delimiter = ' ';

  size_t i = 0;
  size_t pos = input_string.find(delimiter);

  while (pos != string::npos) {
    splits.push_back(input_string.substr(i, pos - i));

    i = pos + 1;
    pos = input_string.find(delimiter, i);
  }

  splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

  return splits;
}
