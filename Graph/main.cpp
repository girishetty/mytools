// Program that runs Graph functionalities.
#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;

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

  cout << "Following is Breadth First Traversal (starting from vertex 2)" << endl;
  g.BFS(2);

  cout << "Following is Depth First Traversal (starting from vertex 2)" << endl;
  g.DFS(2);

  g.TopologicalOrdering();

  cout << "Mother Vertex is: " << g.FindMother() << endl;

  cout << "There is a Path between (0, 3): " << g.isReachable(0, 3) << endl;
  cout << "There is a Path between (3, 0): " << g.isReachable(3, 0) << endl;
  return 0;
}
