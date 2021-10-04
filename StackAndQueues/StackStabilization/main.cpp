/*
 There's a stack of N inflatable discs, with the ith disc from the top having an initial radius of R[i] inches.

 The stack is considered unstable if it includes at least one disc whose radius is larger than or equal to that
 of the disc directly under it. In other words, for the stack to be stable, each disc must have a strictly
 smaller radius than that of the disc directly under it.

 As long as the stack is unstable, you can repeatedly choose any disc of your choice and deflate it down
 to have a radius of your choice which is strictly smaller than the disc’s prior radius.
 The new radius must be a positive integer number of inches.

 Determine the minimum number of discs which need to be deflated in order to make the stack stable,
 if this is possible at all. If it is impossible to stabilize the stack, return -1 instead.

 Constraints:
     1 ≤ N ≤ 50
     1 ≤ Ri ≤ 1,000,000,000

 Sample Test Case #1
     N = 5
     R = [2, 5, 3, 6, 5]
     Expected Return Value = 3

 Sample Test Case #2
     N = 3
     R = [100, 100, 100]
     Expected Return Value = 2

 Sample Test Case #3
     N = 4
     R = [6, 5, 4, 3]
     Expected Return Value = -1

 Sample Explanation
     In the first case, the discs (from top to bottom) have radii of [2", 5", 3", 6", 5"].
     One optimal way to stabilize the stack is by deflating disc 1 from 2" to 1",
     deflating disc 2 from 5" to 2", and deflating disc 4 from 6" to 4"
     This yields final radii of [1", 2", 3", 4", 5"].

     In the second case, one optimal way to stabilize the stack is by deflating disc 1
     from 100" to 1" and disc 2 from 100" to 10".

     In the third case, it is impossible to make the stack stable after any number of deflations.
 */

#include <bits/stdc++.h>
using namespace std;

void display(const vector<int>& D) {
  std::cout << "Stack: ";
  for (auto item : D) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

int getMinimumDeflatedDiscsUsingStack(const vector<int>& discs) {
  std::stack<int> sorted;
  std::stack<int> intermediate;

  sorted.push(discs[0]);
  for (size_t index = 1; index < discs.size(); index++) {
    // As long as discs are in order, push it to sorted stack.
    if (sorted.top() < discs[index]) {
      sorted.push(discs[index]);
      continue;
    }

    auto disc = discs[index];
    //std::cout << "Disc Radius: " << disc << " No of Discs before this: " << sorted.size() << std::endl;
    // failure condition - there can't be >= number of elements than this element itself
    if (disc <= sorted.size()) {
      return -1;
    }

    // Now pop the discs from sorted stack, deflate it and move into another stack
    // until we find a disc that is smaller than the current disc
    intermediate.push(disc--);
    do {
      sorted.pop();
      intermediate.push(disc--);
    } while (!sorted.empty() && sorted.top() >= disc);

    // Now we can move all the discs from intermediate to sorted
    do {
      sorted.push(intermediate.top());
      intermediate.pop();
    } while (!intermediate.empty());
  }

  size_t deflated = 0;
  // Now we can compare discs with sorted discs to see how many discs have been deflated
  for (auto it = discs.rbegin(); it != discs.rend(); it++) {
    auto disc = sorted.top();
    if (disc != *it) {
      deflated++;
    }
    sorted.pop();
  }

  return deflated;
}

int getMinimumDeflatedDiscs(vector<int>& discs) {
  display(discs);
  size_t minCount = 0;
  size_t count = discs.size();
  size_t mark = 0;
  for (size_t index = 0; index < count; ) {
    // move "index" until we find an element that is not in sorted order
    for (; discs[index + 1] > discs[index] && index < count - 1; index++);
    if (index < count - 1) {
      index++;
    }
    std::cout << "Mark: " << mark << " Index: " << index << std::endl;
    // failure condition - there can't be >= elements than this element itself
    if (discs[index] <= index) {
      return -1;
    }

    // Now go back one position at a time and make it smaller than the current element.
    for (size_t back = index - 1; back >= mark; back--) {
      if (discs[back] < discs[back + 1]) {
        break;
      }
      // Deflate the disk at position back to a radius smaller than the next one
      discs[back] = discs[back + 1] - 1;
      minCount++;
    }
    mark = index;
    index++;
  }

  display(discs);
  return minCount;
}

void testMinimumDeflatedDiscs(vector<int> discs) {
  std::cout << "Disc Count#1: " << getMinimumDeflatedDiscsUsingStack(discs) << std::endl;
  std::cout << "Disc Count#2: " << getMinimumDeflatedDiscs(discs) << std::endl;
}

int main() {
  //testMinimumDeflatedDiscs({2, 5, 3, 6, 5});
  //testMinimumDeflatedDiscs({2, 5, 3, 5, 4});
  //testMinimumDeflatedDiscs({100, 100, 100});
  //testMinimumDeflatedDiscs({6, 5, 4, 3});
  testMinimumDeflatedDiscs({1, 2, 6, 5});

  return 0;
}

