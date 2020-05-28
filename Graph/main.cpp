// Program to print BFS traversal from a given source vertex.
// BFS(int s) traverses vertices reachable from s.
#include <list>
#include <stack>
#include <vector>
#include <iostream>

using std::cout;
using std::list;
using std::stack;
using std::vector;

// Graph is represented using adjacency list. Everynode of adjacency list contains vertex number of
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
  Graph(size_t vertex) : mVertex(vertex) {
    mAdjList = new list<AdjListNode>[mVertex];
  }

  ~Graph() {
    delete [] mAdjList;
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

  int FindMother();

private:
  void DFSUtil(int v, vector<bool>& visited, bool print = true);

private:
  // Number of vertices.
  size_t mVertex;
  
  // Pointer to an array containing adjacency lists.
  list<AdjListNode>* mAdjList = nullptr;
};

void Graph::BFS(int beginVertex) { 
  // Mark all the vertices as not visited
  bool* visited = new bool[mVertex];
  for (size_t i = 0; i < mVertex; i++) {
    visited[i] = false;
  }

  // Create a queue for BFS
  list<int> queue; 
  
  // Mark the current node as visited and enqueue it.
  visited[beginVertex] = true; 
  queue.push_back(beginVertex); 
  
  // 'i' will be used to get all adjacent vertices of a vertex.
  list<AdjListNode>::iterator i;
  
  while (!queue.empty()) {
    // Dequeue a vertex from queue and print it
    beginVertex = queue.front();
    cout << beginVertex << " ";
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

void Graph::DFSUtil(int v, vector<bool>& visited, bool print) {
  // Mark the current node as visited and print it.
  visited[v] = true; 
  if (print) {
    cout << v << " "; 
  }

  // Recur for all the vertices adjacent to this vertex.
  list<AdjListNode>::iterator i;
  for (i = mAdjList[v].begin(); i != mAdjList[v].end(); ++i) {
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
  DFSUtil(v, visited);
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

// Driver program to test methods of graph class.
int main() {
  // Create a graph.
  Graph g(4);
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 2);
  g.addEdge(2, 0);
  g.addEdge(2, 3);
  g.addEdge(3, 3);

  cout << "Following is Breadth First Traversal (starting from vertex 2)" << std::endl;
  g.BFS(2);

  cout << "Following is Depth First Traversal (starting from vertex 2)" << std::endl;
  g.DFS(2);

  cout << "Mother Vertex is: " << g.FindMother() << std::endl;
  return 0;
}
