// Program to print BFS traversal from a given source vertex.
// BFS(int s) traverses vertices reachable from s.

#pragma once

#include <vector>

// Graph is represented using adjacency list. Every node of adjacency list contains vertex number of
// the vertex to which edge connects. It also contains weight of the edge.
class AdjListNode {
public:
  AdjListNode(int v, int w = 1) : mVertex(v), mWeight(w) {}

  int getVertex() const { return mVertex; }  
  int getWeight() const { return mWeight; }  

private:
  int mVertex;
  int mWeight;
};

// This class represents a directed graph using adjacency list representation.
class Graph { 
public: 
  Graph(size_t vertex) : mVertex(vertex),
    mAdjList(mVertex) {
  }

  ~Graph() {
  }

  // function to add an edge (u-->v) to graph of weight w.
  void addEdge(int u, int v, int w = 1) {
    // Adding v with weight w to u's list.
    AdjListNode node(v, w);
    mAdjList[u].push_back(node);
  }

  // prints BFS traversal from a given source s.
  void BFS(int beginVertex);

  // prints BFS traversal from a given vertex v.
  void DFS(int v);

  // Topological sorting for Directed Acyclic Graph (DAG) is a linear ordering of
  // vertices such that for every directed edge uv, vertex u comes before v in the
  // ordering. Topological Sorting for a graph is not possible if the graph is not a DAG
  void TopologicalOrdering();

  // A mother vertex in a graph G = (V,E) is a vertex v such that all other vertices
  // in G can be reached by a path from v.
  int FindMother();

  // A BFS based function to check whether d is reachable from s.
  bool isReachable(int s, int d) const;

private:
  void DFSUtil(int v, std::vector<bool>& visited, bool print = true) const;
  void TopologicalOrderingUtil(int v, std::vector<bool>& visited, std::vector<int>& order) const;

private:
  // Number of vertices.
  size_t mVertex;
  
  // vector (of vetrix count) containing adjacency lists.
  std::vector<std::vector<AdjListNode>> mAdjList;
};
