/*
 https://leetcode.com/problems/merge-intervals/

 Given an array of intervals where intervals[i] = [starti, endi], merge all overlapping intervals, and return an array of the non-overlapping intervals that cover all the intervals in the input.

 Example 1:
  Input: intervals = [[1,3],[2,6],[8,10],[15,18]]
  Output: [[1,6],[8,10],[15,18]]
  Explanation: Since intervals [1,3] and [2,6] overlaps, merge them into [1,6].

 Example 2:
  Input: intervals = [[1,4],[4,5]]
  Output: [[1,5]]
  Explanation: Intervals [1,4] and [4,5] are considered overlapping.
*/

#include <vector>
#include <iostream>
#include <algorithm>

void display(const std::string& what, const std::vector<std::vector<int>>& intervals) {
  std::cout << what << " { ";
  for (const auto& interval : intervals) {
    std::cout << "[" << interval[0] << "," << interval[1] << "] ";
  }
  std::cout << "} " << std::endl;
}

std::vector<std::vector<int>> mergeIntervalsFaster(const std::vector<std::vector<int>>& intervals) {
  std::sort(intervals.begin(), intervals.end());
  std::vector<std::vector<int>> merged;
    
  for (const auto& interval : intervals) {
    if (merged.empty()) {
      merged.push_back(interval);
    } else {
      if (merged.back()[1] < interval[0]) {
        merged.push_back(interval);
      } else {
        merged.back()[1] = std::max(interval[1], merged.back()[1]);
      }
    }
  }

  return merged;
}

std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>>& intervals) {
  std::vector<std::vector<int>> res;
  std::sort(intervals.begin(), intervals.end());

  int i = 0;
  for (; i < intervals.size() - 1; i++){
    int j = i + 1;
    if (intervals[i][1] < intervals[j][0]) {
      // push non-overlapping interval.
      res.push_back(intervals[i]);
    } else {
      intervals[j][0] = intervals[i][0];
      intervals[j][1] = std::max(intervals[i][1], intervals[j][1]);
      i = j - 1;
    }
  }

  if (i == intervals.size() - 1) {
    res.push_back(intervals[i]);
  }

  return res;
}

void testMergeIntervals(std::vector<std::vector<int>>& intervals) {
  std::cout << "===================================================" << std::endl;
  display("Before", intervals);
  auto new_intervals = mergeIntervalsFaster(intervals);
  display("After", new_intervals);
  display("Before", intervals);
  new_intervals = mergeIntervals(intervals);
  display("After", new_intervals);
}

int main() {
  std::vector<std::vector<int>> intervals1 = {{1,4},{4,5}};
  testMergeIntervals(intervals1);

  std::vector<std::vector<int>> intervals2 = {{1,3},{2,6},{8,10},{15,18}};
  testMergeIntervals(intervals2);

  std::vector<std::vector<int>> intervals3 = {{1,3},{2,6},{8,10},{15,18}};
  testMergeIntervals(intervals3);

  std::vector<std::vector<int>> intervals4 = {{1,4}, {0,4}};
  testMergeIntervals(intervals4);

  std::vector<std::vector<int>> intervals5 = {{1,4}, {0,0}};
  testMergeIntervals(intervals5);

  std::vector<std::vector<int>> intervals6 = {{2,3},{4,5},{6,7},{8,9},{1,10}};
  testMergeIntervals(intervals6);

  std::vector<std::vector<int>> intervals7 = {{2,3},{5,5},{2,2},{3,4},{3,4}};
  testMergeIntervals(intervals7);

  return 0;
}
