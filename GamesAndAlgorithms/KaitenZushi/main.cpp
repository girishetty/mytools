/*
 There are N dishes in a row on a kaiten belt, with the ith dish being of type D[i].
 Some dishes may be of the same type as one another.

 You would like to eat as many dishes as possible, but would also like to keep it interesting.
 The N dishes will arrive in front of you, one after another in order, and for each one you may
 choose to either eat or not eat it. However, if a dish has the same type as any of the previous
 K dishes you've eaten, you'll refuse to eat it. You eat very fast, so you can consume a dish before the next one gets to you.

 Determine the maximum number of dishes which you can eat.

 Constraints:
    1 ≤ N ≤ 500,000
    1 ≤ K ≤ N
    1 ≤ D[i] ≤ 1,000,000

 Sample Test Case #1:
    N = 6
    D = [1, 2, 3, 3, 2, 1]
    K = 1
    Expected Return Value = 5

 Sample Test Case #2
    N = 6
    D = [1, 2, 3, 3, 2, 1]
    K = 2
    Expected Return Value = 4

 Sample Test Case #3
    N = 7
    D = [1, 2, 1, 2, 1, 2, 1]
    K = 2
    Expected Return Value = 2

 Sample Explanation:
    In the first case, the dishes have types of [1, 2, 3, 3, 2, 1], and you may not eat the same type of dish twice in a row.
    You can therefore only eat one of the two type-3 dishes, but can otherwise eat each remaining dish, for a total of 5.

    In the second case, you may not eat a dish if it has the same type as either of the previous 2 dishes you've eaten.
    The largest number of dishes you can eat is 4, for example by eating dishes 1, 3, 5, and 6.

    In the third case, once you eat one type-1 dish and one type-2 dish, you may not eat any of the remaining dishes.
    Therefore, you can eat at most 2 dishes (for example, the first 2).
 */

#include <bits/stdc++.h>
using namespace std;

void displayEaten(const std::deque<int>& queue, int dish) {
  std::cout << "Dish: " << dish << " Last: ";

  for (auto item : queue) {
    std::cout << item << " ";
  }

  std::cout << std::endl;
}

void displayDishes(const vector<int>& D) {
  std::cout << "Dishes: ";
  for (auto item : D) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

int getMaximumEatenDishCount(int N, const vector<int>& D, int K) {
  int eatenCount = 0;
  std::deque<int> queue;

  //displayDishes();

  for (auto dish : D) {
    //displayEaten(queue, dish);
    if (std::find(queue.begin(), queue.end(), dish) == queue.end()) {
      eatenCount++;
      if (queue.size() == K) {
        queue.pop_front();
      }
      queue.push_back(dish);
    }
  }

  return eatenCount;
}

int main() {
  std::cout << getMaximumEatenDishCount(6, {1, 2, 3, 3, 2, 1}, 1) << std::endl;
  std::cout << getMaximumEatenDishCount(6, {1, 2, 3, 3, 2, 1}, 2) << std::endl;
  std::cout << getMaximumEatenDishCount(7, {1, 2, 1, 2, 1, 2, 1}, 2) << std::endl;

  return 0;
}

