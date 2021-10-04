/*
 * https://www.geeksforgeeks.org/largest-rectangle-under-histogram/
 *
 * Find the largest rectangular area possible in a given histogram where the largest rectangle can be made of a number of contiguous bars.
 * For simplicity, assume that all bars have same width and the width is 1 unit. 
 *
 * For example, consider the following histogram with 9 bars of heights {6, 2, 5, 4, 4, 5, 2, 1, 6}.
 * The largest possible rectangle possible is 16 (of size 4 x 4)
 *
 * |
 * |
 * |
 * |6                       6
 * |__    5        5        __
 * |  |   __  4 4  __      |  | 
 * |  |  |  |__ _ |  |     |  |
 * |  |2 |  |  |  |  | 2   |  |
 * |  |__|  |  |  |  |__ 1 |  |
 * |  |  |  |  |  |  |  |__|  |
 * |__|__|__|__|__|__|__|__|__|___
 *
 *  0  1  2  3  4  5  6  7  8
 */

#include <stack>
#include <vector>
#include <iostream>

void display(const char* what, const std::vector<size_t>& array) {
  std::cout << what << " is: ";
  for (auto item : array) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

inline void updateLargestArea(const std::vector<size_t>& histogram, size_t currentIndex,
                         std::stack<size_t>& stack, size_t& largestArea) {
  // Calculate the area of the largest histogram with top of the stack as the height of the bar.
  auto lastIndex = stack.top();
  stack.pop();

  // Check how many such bars (of size equal or more) are there.
  auto noOfBars = stack.empty() ? currentIndex : currentIndex - (stack.top() + 1);

  // Now we can calculate the area.
  auto lastArea = histogram[lastIndex] * noOfBars;

  // See which one is the largest so far.
  largestArea = std::max(largestArea, lastArea);
}

size_t largestRectangleInHistogram(const std::vector<size_t>& histogram) {
  size_t largestArea = 0;
  size_t size = histogram.size();
  std::stack<size_t> visited;

  for (size_t i = 0; i < size;) {
    // as long as the current histogram is bigger than the last one, push the position/index to stack.
    if (visited.empty() || histogram[visited.top()] <= histogram[i]) {
      visited.push(i);
      i++;
      continue;
    }

    // Now we have found a smaller histogram, which means time to check the current rectangle size.
    updateLargestArea(histogram, i, visited, largestArea);
  }

  // Make sure we check all the remaining entries in the stack
  while (!visited.empty()) {
    updateLargestArea(histogram, size, visited, largestArea);
  }

  return largestArea;
}

void testLargestRectangleInHistogram(std::vector<size_t> histogram) {
  display("Histogram", histogram);
  size_t area = largestRectangleInHistogram(histogram);
  std::cout << "Area of the Largest Histogram is: " << area << std::endl;
}

int main() {
  testLargestRectangleInHistogram({6, 2, 5, 4, 4, 5, 2, 1, 6});

  return 0;
}
