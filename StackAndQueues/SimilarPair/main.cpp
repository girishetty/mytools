#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include <iostream>

using namespace std;

/**
 *
 * A pair of nodes {a, b}, is a similar pair if the following conditions are true:
 * node a is the ancestor of node b.
 * 
 * Given a tree where each node is labeled from 1 to N, find the number of similar pairs in the tree.
 *
 * Function Description
 * Complete the similarPair function in the editor below. It should return an integer that represents the number of pairs meeting the criteria.
 * similarPair has the following parameter(s):
 *  - n: an integer that represents the number of nodes
 *  - k: an integer
 * edges: a two dimensional array where each element consists of two integers that represent connected node numbers
 *
 * Sample Input#1:
 * 5 2
 * 3 2
 * 3 1
 * 1 4
 * 1 5
 *
 * Output#1: 4
 *
 * Sample Input#1:
 * 6 2
 * 1 2
 * 1 3
 * 3 4
 * 3 5
 * 3 6
 *
 * Output#2: 4
 */

// For any given 2 nodes, see if there exists any path.
// This uses DFS to find the path.
bool pathExists(const vector<vector<int>>& edges, int n, int beginNode, int endNode) {
  // Ignore the case when its the same node.
  if (beginNode == endNode) {
    return false;
  }

  //Maintain a list of visited nodes.
  vector<bool> visited (n, false);
  visited[beginNode] = true;

  // Maintain a queue for all the nodes that we are going to visit.
  list<int> queue;
  queue.push_back(beginNode);

  while (!queue.empty()) {
    beginNode = queue.front();
    queue.pop_front();

    // Since we know that our edge list is sorted already,
    // pick up the node "beginNode" and start traversing all its connections.
    auto it = find_if(edges.begin(), edges.end(), [&](const vector<int>& item) {
      return (item[0] == beginNode);
    });

    // Look out for all possible paths from our beginNode.
    for (; it != edges.end() && (*it)[0] == beginNode; it++) {
      auto endVertex = (*it)[1];
      if (endVertex == endNode) {
        // Found a path.
        return true;
      }
      if (!visited[endVertex]) {
        visited[endVertex] = true;
        queue.push_back(endVertex);
      }
    }
  }

  return false;
}

void displayList(const vector<vector<int>>& edges) {
  for (auto edge : edges) {
    cout << "(" << edge[0] << ", " << edge[1] << ")" << endl;
  }
}

bool compFunction(const vector<int>& lhs, const vector<int>& rhs) { 
 return lhs[0] < rhs[0]; 
}

int similarPair(int n, int k, vector<vector<int>> edges) {
  displayList(edges);
  std::sort(edges.begin(), edges.end(), compFunction);
  displayList(edges);
  size_t similarCount = 0;
  for (size_t index = 0; index < edges.size();) {
    int beginNode = edges[index][0];
    for (int endNode = 1; endNode <= n; endNode++) {
      // This can't be a possible edge.
      if (beginNode == endNode) {
        continue;
      }
      // Check is there any path/edge exists for any 2 possible nodes.
      if (pathExists(edges, n, beginNode, endNode) &&
        std::abs(beginNode - endNode) <= k) {
        similarCount++;
      }
    }

    // Go to the next unique node.
    for (; index < edges.size() && beginNode == edges[index][0]; index++);
  }

  return similarCount;
}

vector<string> split_string(string);

int main() {
  string nk_temp;
  getline(cin, nk_temp);

  vector<string> nk = split_string(nk_temp);

  int n = stoi(nk[0]);
  int k = stoi(nk[1]);

  vector<vector<int>> edges(n-1);
  for (int edges_row_itr = 0; edges_row_itr < n-1; edges_row_itr++) {
    edges[edges_row_itr].resize(2);

    for (int edges_column_itr = 0; edges_column_itr < 2; edges_column_itr++) {
      cin >> edges[edges_row_itr][edges_column_itr];
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  int result = similarPair(n, k, edges);
  cout << "Similar Pair Count: " << result << endl;

  return 0;
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
