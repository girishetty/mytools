/*
 https://leetcode.com/problems/insert-interval/

 You are given an array of non-overlapping intervals intervals where intervals[i] = [starti, endi] represent the start and the end of the ith interval and intervals is sorted in ascending order by starti. You are also given an interval newInterval = [start, end] that represents the start and end of another interval.

 Insert newInterval into intervals such that intervals is still sorted in ascending order by starti and intervals still does not have any overlapping intervals (merge overlapping intervals if necessary).

 Return intervals after the insertion.
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

std::vector<std::vector<int>> insert(std::vector<std::vector<int>> intervals, std::vector<int>& newInterval) {
  // Cover the case when intervals is empty
  if (intervals.empty()) {
    intervals.push_back(newInterval);
    return intervals;
  }

  // Cover the case when the new interval to be added is empty
  if (newInterval.empty()) {
    return intervals;
  }

  // find the slot where this new inerval can be inserted
  // While doing so, check whether it overlaps with in an existing slot or not.
  bool overlappingInside = false;
  bool overlappingOutside = false;
  auto found = std::lower_bound(intervals.begin(), intervals.end(), newInterval,
      [&overlappingInside, &overlappingOutside]
      (const std::vector<int>& interval, const std::vector<int>& toAdd) {
        int startDiff = interval[0] - toAdd[0];
        int overlappingDiff = interval[1] - toAdd[0];
        if (startDiff <= 0 && overlappingDiff >= 0) {
          int endDiff = interval[1] - toAdd[1];
          if (endDiff < 0) {
            overlappingOutside = true;
          } else {
            overlappingInside = true;
          }
          return false;
        }
        return startDiff <= 0;
      });

  // See if we have found an externally overlapping slot
  int index = 0;
  if (found != intervals.end()) {
    index = found - intervals.begin();
    if (overlappingInside) {
      // Nothing to do
      return intervals;
    } else if (!overlappingOutside) {
      if (newInterval[1] < intervals[index][0]) {
        // A new entry to be added before "found" index.
        intervals.insert(found, newInterval);
        return intervals;
      } else {
        // The new entry is overlaping with "found" entry
        overlappingOutside = true;
      }
    }
  } else {
    // A new entry to be added to the end.
    intervals.push_back(newInterval);
    return intervals;
  }

  // Its effecient to create a new list than working on the current list
  // First add all the intetervals that needs no change
  std::vector<std::vector<int>> updatedIntervals;
  int i = 0;
  for (; i < index; i++) {
    updatedIntervals.push_back(intervals[i]);
  }

  // Now work on merging overlapping time slots
  std::vector<int> mergedSlot = {0, 0};
  mergedSlot[0] = std::min(intervals[index][0], newInterval[0]);
  int newIndex = index;
  int diff = intervals[index][1] - newInterval[1];
  if (diff < 0) {
    // Now find the end of the time slot
    index++;
    auto endTime = newInterval[1];
    for (; index < intervals.size(); index++) {
      auto endDiff = intervals[index][1] - endTime;
      if (endDiff == 0) {
        // overlaps with end time of this interval
        endTime = intervals[index][1];
        break;
      } else if (endDiff < 0) {
        // Goes beyong this interval, check next one.
        continue;
      }
      
      auto diff = intervals[index][0] - endTime;
      if (diff <= 0) {
        // overlaps with begin time of this interval
        endTime = intervals[index][1];
        break;
      } else {
        // we came ahead, should go back the last interval
        // so that it can be added to the updatedIntervals
        // And the end time would be new interval's end time.
        index--;
        break;
      }
    }
    mergedSlot[1] = endTime;
  } else {
    mergedSlot[1] = intervals[index][1];
  }
  updatedIntervals.push_back(mergedSlot);

  // Now copy rest of the elements
  index++;
  for (; index < intervals.size(); index++) {
    updatedIntervals.push_back(intervals[index]);
  }

  return updatedIntervals;
}

std::vector<std::vector<int>> insertFaster(const std::vector<std::vector<int>>& intervals, std::vector<int>& newinterval) {
  std::vector<std::vector<int>> res;
  
  for (int i = 0; i < intervals.size(); i++){
    if (intervals[i][1] < newinterval[0]) {
      // Push until we reach new interval
      res.push_back(intervals[i]);
    } else if (intervals[i][0] > newinterval[1]) {
      // Now we can push the new interval, and update new interval as the current interval
      res.push_back(newinterval);
      newinterval = intervals[i];
    } else if (intervals[i][1] >= newinterval[0] || intervals[i][0] <= newinterval[1]) {
      // Overlapping intervals: {start = min of two starts, end = max of two ends}
      newinterval[0] = std::min(intervals[i][0], newinterval[0]);
      newinterval[1] = std::max(intervals[i][1], newinterval[1]);
    }
  }
  res.push_back(newinterval);
  return res;
}

void testIntervalInserts(std::vector<std::vector<int>> intervals, std::vector<int>& newInterval) {
  std::cout << "===================================================" << std::endl;
  std::cout << "Interval: [" << newInterval[0] << "," << newInterval[1] << "]" << std::endl;
  display("Before", intervals);
  auto new_intervals = insert(intervals, newInterval);
  display("After", new_intervals);
  new_intervals = insertFaster(intervals, newInterval);
  display("After", new_intervals);
}

int main() {

  std::vector<std::vector<int>> intervals = {{1,2},{3,5},{6,7},{8,10},{12,16}};
  std::vector<int> newInterval1 = {4,8};   // {{1,2},{3,10},{12,16}};
  std::vector<int> newInterval2 = {12,17}; // {{1,2},{3,5},{6,7},{8,10},{12,17}};
  std::vector<int> newInterval3 = {1,6};   // {{1,7},{8,10},{12,16}};
  std::vector<int> newInterval4 = {1,1};   // {{1,2},{3,5},{6,7},{8,10},{12,16}};
  std::vector<int> newInterval5 = {0,6};   // {{0,7},{8,10},{12,16}};
  std::vector<int> newInterval6 = {17,18}; // {{1,2},{3,5},{6,7},{8,10},{12,16}, {17, 18}};
  std::vector<int> newInterval7 = {11,18}; // {{1,2},{3,5},{6,7},{8,10},{11,18}};
  std::vector<int> newInterval8 = {4,5};   // {{1,2},{3,5},{6,7},{8,10},{12,16}};
  std::vector<int> newInterval9 = {4,11};   // {{1,2},{3,5},{6,7},{8,10},{12,16}};

  testIntervalInserts(intervals, newInterval1);
  testIntervalInserts(intervals, newInterval2);
  testIntervalInserts(intervals, newInterval3);
  testIntervalInserts(intervals, newInterval4);
  testIntervalInserts(intervals, newInterval5);
  testIntervalInserts(intervals, newInterval6);
  testIntervalInserts(intervals, newInterval7);
  testIntervalInserts(intervals, newInterval8);
  testIntervalInserts(intervals, newInterval9);

  std::vector<std::vector<int>> intervals1 = {{1,3},{6, 9}};
  std::vector<int> newInterval11 = {2,5};   // {{1,5},{6,9}};
  testIntervalInserts(intervals1, newInterval11);
  return 0;
}
