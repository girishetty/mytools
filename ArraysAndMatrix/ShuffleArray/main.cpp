/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/98/design/670/
 *
 * Given an integer array nums, design an algorithm to randomly shuffle the array.
 * All permutations of the array should be equally likely as a result of the shuffling.
 *
 * Implement the Solution class:
 *   - Solution(int[] nums) Initializes the object with the integer array nums.
 *   - int[] reset() Resets the array to its original configuration and returns it.
 *   - int[] shuffle() Returns a random shuffling of the array.
 */

#include <memory>
#include <vector>
#include <climits>
#include <iostream>

void display(const char* what, const std::vector<int>& array) {
  std::cout << what << " is: ";
  for (auto item : array) {
    std::cout << item << "  ";
  }

  std::cout << std::endl;
}

class Solution {
  const std::vector<int> nums_;
  std::vector<int> state_;

public:
  Solution(const std::vector<int>& nums) : nums_(nums), state_(nums) {
    srand(time(0));
  }

  std::vector<int> shuffle() {
    int j = 0;
    for (int index = state_.size() - 1; index > 1; index--) {
      j = rand() % (index + 1);
      int temp = state_[index];
      state_[index] = state_[j];
      state_[j] = temp;
    }
    return state_;
  }

  std::vector<int> reset() {
    srand(time(0));
    state_ = nums_;
    return state_;
  }
};

void testShuffleSolution(const std::vector<int>& nums) {
  display("Input Array", nums);
  std::unique_ptr<Solution> obj = std::make_unique<Solution>(nums);
  int half = nums.size(); // >> 1;
  for (int i = 0; i < half; i++) {
    auto shuffled = obj->shuffle();
    display("Shuffled Array", shuffled);
  }

  auto reset = obj->reset();
  display("Reset Array", reset);
  for (int i = 0; i < half; i++) {
    auto shuffled = obj->shuffle();
    display("Shuffled Array", shuffled);
  }
}

int main() {
  //testShuffleSolution({1, 2, 3, 4, 5, 6});
  testShuffleSolution({-6, 10, 184});

  return 0;
}
