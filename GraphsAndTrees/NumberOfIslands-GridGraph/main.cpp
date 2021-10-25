/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/108/trees-and-graphs/792/
 *
 * Given an m x n 2D binary grid grid which represents a map of '1's (land) and '0's (water),
 * return the number of islands.
 *
 * An island is surrounded by water and is formed by connecting adjacent lands horizontally
 * or vertically. You may assume all four edges of the grid are all surrounded by water.
 *
 * Example#1:
 *     Input: grid = [
 *       [1,1,1,1,0],
 *       [1,1,0,1,0],
 *       [1,1,0,0,0],
 *       [0,0,0,0,0]
 *     ]
 *     Output: 1
 *
 * Example#2:
 *     Input: grid = [
 *       [1,1,0,0,0],
 *       [1,1,0,0,0],
 *       [0,0,1,0,0],
 *       [0,0,0,1,1]
 *      ]
 *      Output: 3
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>

#define USE_SET_AS_VISITED

/*
 * Given an unvisited island, this algorithm will visit all
 * the neighbouring lands that belongs to this island and
 * returns the size of the island.
 *
 */
int exploreIsland(const std::vector<std::vector<int>>& grid,
                  int row, int col,
#ifdef USE_SET_AS_VISITED
                  std::unordered_set<std::string>& visited) {
#else
                  std::vector<std::vector<bool>>& visited) {
#endif
  bool rowValidated = row >= 0 && row < grid.size();
  bool colValidated = col >= 0 && col < grid[0].size();

  // row OR col index is invalid
  if (!rowValidated || !colValidated) {
    return 0;
  }

  // Check if its water or land
  if (grid[row][col] == 0) {
    return 0;
  }

#ifdef USE_SET_AS_VISITED
  // We have already visited this node
  std::string rowCol = std::to_string(row) + '+' + std::to_string(col);
  if (visited.find(rowCol) != visited.end()) {
    return 0;
  }

  // Lets mark this as visited
  visited.insert(rowCol);
#else
  // We have already visited this node
  if (visited[row][col]) {
    return 0;
  }

  // Lets mark this as visited
  visited[row][col] = true;
#endif

  // We know that this island is of size 1 (at least now)
  int size = 1;
  // Now visit all the neighbors of this node
  size += exploreIsland(grid, row - 1, col, visited);
  size += exploreIsland(grid, row + 1, col, visited);
  size += exploreIsland(grid, row, col - 1, visited);
  size += exploreIsland(grid, row, col + 1, visited);
  return size;
}

int findBiggestIsland(const std::vector<std::vector<int>>& grid) {
  int biggest = 0;
  int rows = grid.size();
  int cols = grid[0].size();
  // A visited book keeper to track all the visited nodes in the Grid
#ifdef USE_SET_AS_VISITED
  // We can use a unordered_set instead ==> std::unordered_set<string> visited
  // in which case, the elements of visited unordered_sets are "$row+$col"
  // (That is stringified row and col seperated by + OR ,)
  // This can be efficient, if we have very few lands (1s) in the Grid.
  std::unordered_set<std::string> visited;
#else
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
#endif

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      auto size = exploreIsland(grid, row, col, visited);
      if (size > biggest) {
        biggest = size;
      }
    }
  }

  
  return biggest;
}

int numberOfIslands(const std::vector<std::vector<int>>& grid) {
  int count = 0;
  int rows = grid.size();
  int cols = grid[0].size();
  // A visited book keeper to track all the visited nodes in the Grid
#ifdef USE_SET_AS_VISITED
  // We can use a unordered_set instead ==> std::unordered_set<string> visited
  // in which case, the elements of visited unordered_sets are "$row+$col"
  // (That is stringified row and col seperated by + OR ,)
  // This can be efficient, if we have very few lands (1s) in the Grid.
  std::unordered_set<std::string> visited;
#else
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
#endif

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      if (exploreIsland(grid, row, col, visited) > 0) {
        count++;
      }
    }
  }

  
  return count;
}

void display(const char* what, const std::vector<std::vector<int>>& matrix) {
  std::cout << what << " is: " << std::endl;
  for (const auto& row : matrix) {
    for (const auto& item : row) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
}

void testNumberOfIslands(const std::vector<std::vector<int>>& grid) {
  display("Grid", grid);
  auto count = numberOfIslands(grid);
  auto size = findBiggestIsland(grid);
  std::cout << "Grid has " << count << " Islands in it. And the biggest Island is of size: " << size << std::endl;
}

int main() {
  testNumberOfIslands({
        {1,1,1,1,0},
        {1,1,0,1,0},
        {1,1,0,0,0},
        {0,0,0,0,0},
      });

  testNumberOfIslands({
        {1,1,0,0,0},
        {1,1,0,0,0},
        {0,0,1,0,0},
        {0,0,0,1,1},
      });

  return 0;
}
