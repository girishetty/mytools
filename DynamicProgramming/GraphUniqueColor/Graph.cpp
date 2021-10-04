// Program to print BFS traversal from a given source vertex.
// BFS(int s) traverses vertices reachable from s.

#include <list>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include "Graph.h"

using std::cout;
using std::endl;
using std::list;
using std::setw;
using std::vector;
using std::unordered_map;

Graph::Graph(const vector<int>& colors) : mVertexCount(colors.size()),
    mUniqueCountDP(mVertexCount, vector<int>(mVertexCount, 0)) {
  int vertexID = 0;
  for (auto color : colors) {
    Vertex vertex(vertexID++, color);
    mVertices.push_back(vertex);
  }
}

void Graph::addEdge(int u, int v, int w) {
  // Make sure its with in the range.
  if (u <= mVertexCount && v <= mVertexCount) {
    // Adding v with weight w to u's list.
    Edge node(v, w);
    mVertices[u].addEdge(node);
  }
}

void Graph::BFS(int beginVertex) {
  // Make sure its with in the range.
  if (beginVertex > mVertexCount) {
    return;
  }
  // Mark all the vertices as not visited.
  vector<bool> visited(mVertexCount, false);

  // Create a queue for BFS
  list<int> queue;

  // Mark the current node as visited and enqueue it.
  visited[beginVertex] = true;
  queue.push_back(beginVertex);

  // 'i' will be used to get all adjacent vertices of a vertex.
  vector<Edge>::const_iterator i;

  cout << "BFS: ";
  while (!queue.empty()) {
    // Dequeue a vertex from queue and print it
    beginVertex = queue.front();
    cout << setw(4) << beginVertex << "    ";
    queue.pop_front();

    // Get all adjacent vertices of the dequeued vertex s.
    // If a adjacent has not been visited, then mark it visited and enqueue it.
    const auto& adjList = mVertices[beginVertex].mAdjList;
    for (i = adjList.begin(); i != adjList.end(); ++i) {
      if (!visited[i->getVertex()]) {
        visited[i->getVertex()] = true;
        queue.push_back(i->getVertex());
      }
    }
  }
  cout << endl;
}

void Graph::DFSUtil(int v, vector<bool>& visited, bool print) const {
  // Mark the current node as visited and print it.
  visited[v] = true; 
  if (print) {
    cout << setw(4) << v << "    "; 
  }

  // Recur for all the vertices adjacent to this vertex.
  const auto& adjList = mVertices[v].mAdjList;
  for (auto i = adjList.begin(); i != adjList.end(); ++i) {
    if (!visited[i->getVertex()]) {
      DFSUtil(i->getVertex(), visited, print); 
    }
  }
}

// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
void Graph::DFS(int v) {
  // Make sure its with in the range.
  if (v > mVertexCount) {
    return;
  }
  // Mark all the vertices as not visited
  vector<bool> visited(mVertexCount, false);

  // Call the recursive helper function to print DFS traversal.
  cout << "DFS: ";
  DFSUtil(v, visited);
  cout << endl;
}

void Graph::TopologicalOrderingUtil(int v, vector<bool>& visited, vector<int>& order) const {
  // Mark the current node as visited and print it.
  visited[v] = true; 

  // Recur for all the vertices adjacent to this vertex.
  const auto& adjList = mVertices[v].mAdjList;
  for (auto i = adjList.begin(); i != adjList.end(); ++i) {
    if (!visited[i->getVertex()]) {
      TopologicalOrderingUtil(i->getVertex(), visited, order); 
    }
  }
  order.push_back(v);
}

void Graph::TopologicalOrdering() {
  // Mark all the vertices as not visited
  vector<bool> visited(mVertexCount, false);
  vector<int>  order;

  // Call the recursive helper function to print Topologica lOrdering.
  for (size_t vertex = 0; vertex < mVertexCount; vertex++) {
    if (!visited[vertex]) {
      TopologicalOrderingUtil(0, visited, order);
    }
  }

  cout << "TopologicalOrdering: ";
  for (auto it = order.rbegin(); it != order.rend(); it++) {
    cout << setw(4) << *it << "    ";
  }
  cout << endl;
}

// Returns a mother vertex if exists. Otherwise returns -1.
int Graph::FindMother() {
  // Mark all the vertices as not visited
  vector<bool> visited(mVertexCount, false);

  // To store last finished vertex (or mother vertex)
  int v = -1;

  // Do a DFS traversal and find the last finished vertex.
  for (size_t i = 0; i < mVertexCount; i++) {
    if (visited[i] == false) {
      DFSUtil(i, visited, false);
      v = i;
    }
  }

  // If there exist mother vertex (or vetices) in given graph, then v must be one (or one of them)

  // Now check if v is actually a mother vertex (or graph has a mother vertex).
  // We basically check if every vertex is reachable from v or not.

  // Reset all values in visited[] as false and do DFS beginning from v to check
  // if all vertices are reachable from it or not. 
  fill(visited.begin(), visited.end(), false);
  DFSUtil(v, visited, false);
  for (size_t i = 0; i < mVertexCount; i++) {
    if (visited[i] == false) {
      return -1;
    }
  }
  
  return v; 
}

void printPath(int s, int d, const vector<int>& path) {
  cout << " Path between (" << s << " and " << d << ") of length: " << path.size() << " is : ";
  for (auto item : path) {
    cout << "  " << item;
  }
  cout << endl;
}

bool Graph::findPathDFS(vector<bool>& visited, int s, int d, vector<int>& path) const {
  path.push_back(s);
  // Base case
  if (s == d) {
    return true;
  }
  // Mark the current node as visited and print it.
  visited[s] = true; 

  // Recur for all the vertices adjacent to this vertex.
  const auto& adjList = mVertices[s].mAdjList;
  for (auto i = adjList.begin(); i != adjList.end(); ++i) {
    if (!visited[i->getVertex()]) {
      if (findPathDFS(visited, i->getVertex(), d, path)) {
        return true;
      }
      path.pop_back();
    }
  }

  return false;
}

bool Graph::getPath(int s, int d, vector<int>& path) const {
  path.clear();
  // Make sure its with in the range.
  if (s > mVertexCount || d > mVertexCount) {
    return false;
  }
  // Mark all the vertices as not visited
  vector<bool> visited(mVertexCount, false);

  bool found = findPathDFS(visited, s, d, path);
#ifdef _DEBUG
  printPath(s, d, path);
#endif
  return found;
}

bool Graph::isReachable(int s, int d) const {
  vector<int> path;
  return getPath(s, d, path);
}

int Graph::countUniqueVerticies(int s, int d) const {
  // Make sure its with in the range.
  if (s > mVertexCount || d > mVertexCount) {
    return false;
  }
  if (mUniqueCountDP[s][d] != 0) {
    return mUniqueCountDP[s][d];
  }
  size_t count = 0;
  vector<int> path;
  if (!getPath(s, d, path)) {
    return count;
  }

  unordered_map<int, bool> colors;
  for (const auto& vertex : path) {
    auto color = mVertices[vertex].getColor();
    if (colors.find(color) == colors.end()) {
      colors[color] = true;
      count++;
    }
    mUniqueCountDP[s][vertex] = count;
  }
  return count;
}
