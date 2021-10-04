/*
 * https://leetcode.com/problems/k-closest-points-to-origin/
 *
 * Given an array of points where points[i] = [xi, yi] represents a point on the X-Y plane and an integer k, return the k closest points to the origin (0, 0).
 *
 * The distance between two points on the X-Y plane is the Euclidean distance (i.e., √(x1 - x2)2 + (y1 - y2)2).
 * 
 * You may return the answer in any order. The answer is guaranteed to be unique (except for the order that it is in).
 *
 * Example 1:
 *    Input: points = [[1,3],[-2,2]], k = 1
 *    Output: [[-2,2]]
 *    Explanation:
 *    The distance between (1, 3) and the origin is sqrt(10).
 *    The distance between (-2, 2) and the origin is sqrt(8).
 *    Since sqrt(8) < sqrt(10), (-2, 2) is closer to the origin.
 *    We only want the closest k = 1 points from the origin, so the answer is just [[-2,2]].
 *
 * Example 2:
 *    Input: points = [[3,3],[5,-1],[-2,4]], k = 2
 *    Output: [[3,3],[-2,4]]
 *    Explanation: The answer [[-2,4],[3,3]] would also be accepted.
 */

#include <map>
#include <cmath>
#include <vector>
#include <iostream>

inline double getDistance(const std::vector<int>& point) {
  return std::sqrt(point[0] * point[0] + point[1] * point[1]);
}

/**
 * This function takes middle element as pivot, places the pivot element
 * at its correct position in sorted array, and places all smaller (smaller than pivot)
 * to left of pivot and all greater elements to right of pivot.
 */
int partition(std::vector<std::vector<int>>& points, int low, int high) {
  int i = low;
  int j = high;
  auto& pivot = points[(low + high) >> 1];
  auto pivotDist = getDistance(pivot);

  for (; i < j;) {
    // Move i until we find an item bigger than pivot
    for (; i < j && getDistance(points[i]) <= pivotDist; i++);

    // Move j until we find an item not bigger than pivot
    for (; getDistance(points[j]) > pivotDist; j--);

    // Check if we can swap i and j
    if (i < j) {
      points[i].swap(points[j]);
      i++;
      j--;
    }
  }

  // Now swap pivot with the points[j]. Thats where the parition happens now.
  pivot.swap(points[j]);

  return j;
}

std::vector<std::vector<int>> quickSelect(std::vector<std::vector<int>>& points, int k) {
  int left = 0;
  int right = points.size() - 1;
  int pivotIndex = right + 1;

  while (pivotIndex != k) {
    pivotIndex = partition(points, left, right);
    if (pivotIndex < k) {
      // pivot is behind k, but we need more elements, so move left to pivot
      left = pivotIndex;
    } else {
      // pivot is ahead of k, but we need less elements, so move right to pivot
      right = pivotIndex - 1;
    }
  }

  return std::vector<std::vector<int>>(points.begin(), points.begin() + k);
}

std::vector<std::vector<int>> kClosestUsingQuickSort(std::vector<std::vector<int>>& points, int k) {
  return quickSelect(points, k);
}

std::vector<std::vector<int>> kClosestUsingMap(const std::vector<std::vector<int>>& points, int k) {
  std::map<double, std::vector<int>> sortedMap;

  for (int i = 0; i < points.size(); i++) {
    auto distance = getDistance(points[i]);
    auto found = sortedMap.find(distance);
    if (found == sortedMap.end()) {
      sortedMap[distance] = std::vector<int>{i};
    } else {
      found->second.push_back(i);
    }
  }

  std::vector<std::vector<int>> sorted;
  int i = 0;
  for (auto it = sortedMap.begin(); it != sortedMap.end(); it++) {
    for (auto item : it->second) {
      sorted.push_back(points[item]);
      i++;
      if (i == k) {
        break;
      }
    }
    if (i == k) {
      break;
    }
  }
  return sorted;
}

void display(const char* what, const std::vector<std::vector<int>>& points) {
  std::cout << what << " are: { ";
  for (const auto& point: points) {
    std::cout << "(" << point[0] << ", " << point[1] << "), ";
  }
  std::cout << " }" << std::endl;
}

void testKClosest(std::vector<std::vector<int>> points, int k) {
  auto match1 = kClosestUsingMap(points, k);
  char what[100];
  snprintf(what, 100, "%d Closest Points using kClosestUsingMap", k);
  display(what, match1);
  auto match2 = kClosestUsingQuickSort(points, k);
  snprintf(what, 100, "%d Closest Points using kClosestUsingQuickSort", k);
  display(what, match2);
}

int main() {
  testKClosest({{1,3},{-2,2}}, 1);
  testKClosest({{3,3},{5,-1},{-2,4}}, 2);

  return 0;
}
