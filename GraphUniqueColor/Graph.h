// Program to print BFS traversal from a given source vertex.
// BFS(int s) traverses vertices reachable from s.

#pragma once

#include <vector>

class Edge;
class Graph;

// Each Node/Vertex in the graph has an id (vertex number)
// and a color associated with it along with
// an Adjucency List (list of all edges from that vertex)
class Vertex {
friend class Graph;
public:
  Vertex(int id, int color) : mID(id), mColor(color) {};
  int getID() const { return mID; };
  int getColor() const { return mColor; };

  void addEdge(const Edge& edge) {
    mAdjList.push_back(edge);
  }

private:
  int mID;
  int mColor;
  // containing adjacency list or edges.
  std::vector<Edge> mAdjList;
};

// Graph is represented using Adjacency List of edges.
// Every Node of of Adjacency List contains Vertex ID of the Vertex to which Edge connects.
// It also contains weight of the Edge.
class Edge {
public:
  Edge(int vertexID, int w = 1) : mVertexID(vertexID), mWeight(w) {}

  int getVertex() const { return mVertexID; }  
  int getWeight() const { return mWeight; }  

private:
  int mVertexID;
  int mWeight;
};

// This class represents a directed graph using adjacency list representation.
class Graph {
public: 
  Graph(const std::vector<int>& colors);

  ~Graph() {}

  size_t getSize() const {
    return mVertexCount;
  }

  // function to add an edge (u-->v) to graph of weight w.
  void addEdge(int u, int v, int w = 1);

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

  // Get the list of all vertecies from s to d.
  // Returns false if there is no path between s and d.
  bool getPath(int s, int d, std::vector<int>& path) const;

  // Count the number of unique verticies between path s to d.
  int countUniqueVerticies(int s, int d) const;

private:
  void DFSUtil(int v, std::vector<bool>& visited, bool print = true) const;
  void TopologicalOrderingUtil(int v, std::vector<bool>& visited, std::vector<int>& order) const;
  bool findPathDFS(std::vector<bool>& visited, int s, int d, std::vector<int>& path) const;

private:
  // Number of vertices.
  const size_t mVertexCount;
  
  // List of verticies.
  std::vector<Vertex> mVertices;
  mutable std::vector<std::vector<int>> mUniqueCountDP;
};
