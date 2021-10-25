/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/109/backtracking/797/
 *
 * Given an m x n grid of characters board and a string word, return true if word exists in the grid.
 *
 * The word can be constructed from letters of sequentially adjacent cells, where adjacent cells are
 * horizontally or vertically neighboring. The same letter cell may not be used more than once.
 *
 *  A B C E
 *  S F C S
 *  A D E E
 *
 * Example#1:
 *    Input: board = [["A','B','C','E"],["S','F','C','S"],["A','D','E','E"]], word = "ABCCED"
 *    Output: true
 *
 * Example#2:
 *    Input: board = [["A','B','C','E"],["S','F','C','S"],["A','D','E','E"]], word = "SEE"
 *    Output: true
 *
 */

#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

bool exploreWord(const std::vector<std::vector<char>>& board,
                 int row, int col,
                 const std::string& word, int index,
                 std::unordered_set<std::string>& visited) {
  //std::cout << row << ":" << col << "==> " << index << std::endl;
  // Check if we have already explored the full work
  if (index == word.size()) {
    return true;
  }

  int rows = board.size();
  int cols = board[0].size();
  bool rowValidated = row >= 0 && row < rows;
  bool colValidated = col >= 0 && col < cols;

  // row OR col index is invalid
  if (!rowValidated || !colValidated) {
    return false;
  }

  // Check if this is what we are looking for.
  if (board[row][col] != word[index]) {
    return false;
  }

  // Check if we have already visited this node
  std::string rowCol = std::to_string(row) + '+' + std::to_string(col);
  if (visited.find(rowCol) != visited.end()) {
    return false;
  }

  // Lets mark this as visited
  visited.insert(rowCol);

  // Since this matches what we are looking for, we can explore the remaning lettes in the word
  // One position at a time (top, bottom, left, right)
  if (!exploreWord(board, row - 1, col, word, index + 1, visited)) {
    if (!exploreWord(board, row + 1, col, word, index + 1, visited)) {
      if (!exploreWord(board, row, col - 1, word, index + 1, visited)) {
        if (!exploreWord(board, row, col + 1, word, index + 1, visited)) {
          // Since we failed to explore from here, lets remove this from visited.
          visited.erase(rowCol);
          return false;
        }
      }
    }
  }
  return true;
}

bool isPossible(const std::vector<std::vector<char>>& board, const std::string& word) {
  std::unordered_map<char, int> boardMap;
  for (auto& rows : board) {
    for (auto& item : rows) {
      boardMap[item]++;
    }
  }

  for (auto& item : word) {
    auto found = boardMap.find(item);
    if (found == boardMap.end()) {
      return false;
    }
    if (found->second == 1) {
      boardMap.erase(found);
    } else {
      boardMap[item]--;
    }
  }

  return true;
}

bool isWordExist(const std::vector<std::vector<char>>& board, const std::string& word) {
  auto possible = isPossible(board, word);

  if (!possible) {
    return false;
  }

  int rows = board.size();
  int cols = board[0].size();
  // A visited book keeper to track all the visited nodes in the Grid
  std::unordered_set<std::string> visited;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      if (exploreWord(board, row, col, word, 0, visited) > 0) {
        return true;
      }
    }
  }

  return false;
}

void display(const char* what, const std::vector<std::vector<char>>& matrix) {
  std::cout << what << " is: " << std::endl;
  for (const auto& row : matrix) {
    for (const auto& item : row) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
}

void testIsWordExist(const std::vector<std::vector<char>>& board, const std::string& word) {
  display("Board", board);
  auto found = isWordExist(board, word);
  if (found) {
    std::cout << word << " is Found in the given Board" << std::endl;
  } else {
    std::cout << word << " is NOT Found in the given Board" << std::endl;
  }
}

int main() {
  testIsWordExist({
        {'A', 'B', 'C', 'E'},
        {'S', 'F', 'C', 'S'},
        {'A', 'D', 'E', 'E'},
      }, "ABCCED");

  testIsWordExist({
        {'A', 'B', 'C', 'E'},
        {'S', 'F', 'C', 'S'},
        {'A', 'D', 'E', 'E'},
      }, "ABCB");

  testIsWordExist({
        {'C', 'A', 'A'},
        {'A', 'A', 'A'},
        {'B', 'C', 'D'},
      }, "AAB");

  testIsWordExist({
        {'A', 'B', 'C', 'E'},
        {'S', 'F', 'E', 'S'},
        {'A', 'D', 'E', 'E'},
      }, "ABCESEEEFS");

  testIsWordExist({
        {'A','A','A','A','A','A'},
        {'A','A','A','A','A','A'},
        {'A','A','A','A','A','A'},
        {'A','A','A','A','A','A'},
        {'A','A','A','A','A','A'},
        {'A','A','A','A','A','A'},
      }, "AAAAAAAAAAAAAAB");

  return 0;
}
