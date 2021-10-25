// Program to print BFS traversal from a given source vertex.
// BFS(int s) traverses vertices reachable from s.

#include <list>
#include <iomanip>
#include <iostream>
#include "Graph.h"

using std::cout;
using std::list;
using std::setw;
using std::vector;

/*
 * Shortest distance between 2 nodes in a graph (distance by number of nodes in between).
 * If no such path, it returns -1
 *
 */
int Graph::shortestDistance(int s, int d) const {
  // Mark all the vertices as not visited
  vector<bool> visited(mVertex, false);

  struct Pair {
    int vertex;
    int distance;
    Pair(int v, int d) : vertex(v), distance(d) {}
    Pair(const Pair& p) : vertex(p.vertex), distance(p.distance) {}
  };
  // Create a queue for BFS
  list<Pair> queue;

  // Mark the current node as visited and enqueue it.
  visited[s] = true;
  queue.push_back(Pair(s, 0));

  // it will be used to get all adjacent vertices of a vertex.
  vector<AdjListNode>::const_iterator i;

  while (!queue.empty()) {
    // Dequeue a vertex from queue.
    Pair p = queue.front();
    queue.pop_front();

    if (p.vertex == d) {
      return p.distance;
    }
    // Get all adjacent vertices of the dequeued vertex s.
    // If a adjacent has not been visited, then mark it visited and enqueue it.
    for (i = mAdjList[p.vertex].begin(); i != mAdjList[p.vertex].end(); ++i) {
      // If this adjacent node has not been visited yet, add it to the queue
      auto vertex = i->getVertex();
      if (!visited[vertex]) { 
        visited[vertex] = true;
        queue.push_back(Pair(vertex, p.distance+1));
      }
    } 
  }

  // If BFS is complete without visiting d 
  return -1; 
}

/*
 * Given 2 nodes of a Graph, checks if the 2nd one is reachable from the first one.
 * This one is almost similar to shortestDistance algorithm.
 * If no such path, it returns false.
 *
 */
bool Graph::isReachable(int s, int d) const {
  // Base case
  if (s == d) {
    return true;
  }

  // Mark all the vertices as not visited
  vector<bool> visited(mVertex, false);

  // Create a queue for BFS
  list<int> queue;

  // Mark the current node as visited and enqueue it.
  visited[s] = true;
  queue.push_back(s);

  // it will be used to get all adjacent vertices of a vertex.
  vector<AdjListNode>::const_iterator i;

  while (!queue.empty()) {
    // Dequeue a vertex from queue.
    s = queue.front();
    queue.pop_front();

    // Get all adjacent vertices of the dequeued vertex s.
    // If a adjacent has not been visited, then mark it visited and enqueue it.
    for (i = mAdjList[s].begin(); i != mAdjList[s].end(); ++i) {
      // If this adjacent node is the destination node, then  return true.
      auto vertex = i->getVertex();
      if (vertex == d) {
        return true;
      }

      // Else, continue to do BFS 
      if (!visited[vertex]) { 
        visited[vertex] = true;
        queue.push_back(vertex);
      }
    } 
  }

  // If BFS is complete without visiting d 
  return false; 
}

/*
 * Breadth first traversal of a Graph.
 * Here, we visit all the neighboring nodes of a node, before taking further away nodes
 *
 */
void Graph::BFS(int beginVertex) {
  // Mark all the vertices as not visited.
  vector<bool> visited(mVertex, false);

  // Create a queue for BFS
  list<int> queue;

  // Mark the current node as visited and enqueue it.
  visited[beginVertex] = true;
  queue.push_back(beginVertex);

  // 'i' will be used to get all adjacent vertices of a vertex.
  vector<AdjListNode>::iterator i;

  cout << "BFS: ";
  while (!queue.empty()) {
    // Dequeue a vertex from queue and print it
    beginVertex = queue.front();
    cout << setw(4) << beginVertex << "    ";
    queue.pop_front();

    // Get all adjacent vertices of the dequeued vertex s.
    // If a adjacent has not been visited, then mark it visited and enqueue it.
    for (i = mAdjList[beginVertex].begin(); i != mAdjList[beginVertex].end(); ++i) {
      if (!visited[i->getVertex()]) {
        visited[i->getVertex()] = true;
        queue.push_back(i->getVertex());
      }
    }
  }
  cout << std::endl;
}

void Graph::DFSUtil(int v, vector<bool>& visited, bool print) const {
  // Mark the current node as visited and print it.
  visited[v] = true; 
  if (print) {
    cout << setw(4) << v << "    "; 
  }

  // Recur for all the vertices adjacent to this vertex.
  for (auto i = mAdjList[v].begin(); i != mAdjList[v].end(); ++i) {
    if (!visited[i->getVertex()]) {
      DFSUtil(i->getVertex(), visited, print); 
    }
  }
}

// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
void Graph::DFS(int v) {
  // Mark all the vertices as not visited
  vector<bool> visited(mVertex, false);

  // Call the recursive helper function to print DFS traversal.
  cout << "DFS: ";
  DFSUtil(v, visited);
  cout << std::endl;
}

void Graph::TopologicalOrderingUtil(int v, vector<bool>& visited, vector<int>& order) const {
  // Mark the current node as visited and print it.
  visited[v] = true; 

  // Recur for all the vertices adjacent to this vertex.
  for (auto i = mAdjList[v].begin(); i != mAdjList[v].end(); ++i) {
    if (!visited[i->getVertex()]) {
      TopologicalOrderingUtil(i->getVertex(), visited, order); 
    }
  }
  order.push_back(v);
}

void Graph::TopologicalOrdering() {
  // Mark all the vertices as not visited
  vector<bool> visited(mVertex, false);
  vector<int>  order;

  // Call the recursive helper function to print Topological Ordering.
  for (size_t vertex = 0; vertex < mVertex; vertex++) {
    if (!visited[vertex]) {
      TopologicalOrderingUtil(0, visited, order);
    }
  }

  cout << "TopologicalOrdering: ";
  for (auto it = order.rbegin(); it != order.rend(); it++) {
    cout << setw(4) << *it << "    ";
  }
  cout << std::endl;
}

// Returns a mother vertex if exists. Otherwise returns -1.
int Graph::FindMother() {
  // Mark all the vertices as not visited
  vector<bool> visited(mVertex, false);

  // To store last finished vertex (or mother vertex)
  int v = -1;

  // Do a DFS traversal and find the last finished vertex.
  for (size_t i = 0; i < mVertex; i++) {
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
  for (size_t i = 0; i < mVertex; i++) {
    if (visited[i] == false) {
      return -1;
    }
  }
  
  return v; 
}
