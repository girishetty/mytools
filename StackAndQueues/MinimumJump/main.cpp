/*
 https://leetcode.com/problems/minimum-jumps-to-reach-home/

 A certain bug's home is on the x-axis at position x. Help them get there from position 0.

 The bug jumps according to the following rules:

  - It can jump exactly a positions forward (to the right).
  - It can jump exactly b positions backward (to the left).
  - It cannot jump backward twice in a row.
  - It cannot jump to any forbidden positions.

 The bug may jump forward beyond its home, but it cannot jump to positions numbered with negative integers.

 Given an array of integers forbidden, where forbidden[i] means that the bug cannot jump to the position forbidden[i], and integers a, b, and x, return the minimum number of jumps needed for the bug to reach its home. If there is no possible sequence of jumps that lands the bug on position x, return -1.
*/

#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>

void display(const std::string& what, const std::vector<int>& list) {
  std::cout << what << " { ";
  for (const auto& item : list) {
    std::cout << item  << " ";
  }
  std::cout << "} " << std::endl;
}

int minimumJumps(const std::vector<int>& forbidden, int a, int b, int x) {
  std::unordered_set<int> forbiddenPositions;
  for (auto item : forbidden) {
    forbiddenPositions.insert(item);
  }

  int count = 0;
  // Queue of Steps represented as {position, direction}.
  // When direction is "true", that means forward direction.
  std::queue<std::pair<int, bool>> stepsQueue;
  // Starting at position 0 and we can make only forward jump.
  stepsQueue.push(std::make_pair(0, true));

  while (!stepsQueue.empty()) {

    int size = stepsQueue.size();
    while (size--) {
      auto currentStep = stepsQueue.front();
      stepsQueue.pop();
      int position = currentStep.first;
      if (position == x) {
        // We have reached the home now!
        return count;
      }
     
      // Check whether its a forbidden position
      if (forbiddenPositions.count(position) == 1){
        continue;
      } 

      // If we have already been here without reaching home, then no point in being here again
      // Its as good as a forbidden position.
      forbiddenPositions.insert(position);

      if (currentStep.second == true){
        // Since the last jump was forward, we can take a backward step now.
        int step = position - b;
        if (step >= 0) {
          stepsQueue.push(std::make_pair(step, false));
        }
      }
      // We can always try a forward jump now.
      int step = position + a;
      if (step <= 2000 + a + b) {
        stepsQueue.push(std::make_pair(step, true));
      }
    }
    count++;
  }
  return -1;
}

void testMinimumJumps(const std::vector<int>& forbidden, int a, int b, int x) {
  std::cout << "===================================================" << std::endl;
  std::cout << "Home is @: " << x << " Forward Steps: " << a << " Forward Steps: " << b << std::endl;
  display("Forbidden Positions", forbidden);
  auto count = minimumJumps(forbidden, a, b, x);
  if (count != -1) {
    std::cout << "Takes " << count << " steps to reach home" << std::endl;
  } else {
    std::cout << "There is no way we can reach home!" << std::endl;
  }
}

int main() {

  std::vector<int> example1 = {14, 4, 18, 1, 15};
  testMinimumJumps(example1, 3, 15, 9);

  std::vector<int> example2 = {8, 3, 16, 6, 12, 20};
  testMinimumJumps(example2, 15, 13, 11);

  std::vector<int> example3 = {1, 6, 2, 14, 5, 17, 4};
  testMinimumJumps(example3, 16, 9, 7);

  return 0;
}
