#include <vector>

// reverse the bits of a 32 bit integer. 
uint32_t reverse(uint32_t num) {
  uint32_t result = 0;
  uint32_t mask = 0x01;
  size_t count = 0;
  
  while (num != 0) {
    uint32_t bit = mask & num; 
    result = (result << 1) | bit;
    num = num >> 1;
    count++;
  }
  
  if (count != 32 && result != 0) {
    result = result << (32 - count);
  }
  return result;
}

  
// Streaming Median Filter:
// This is a real-time aka "streaming" audio system that takes in one number/sample per "frame" and
// outputs one number per frame, representing the median of the N past inputs.
//
// For example, N = 3
// frame 1 in: 1 -> 0
// frame 2 in: 2 -> 1
// frame 3 in: 2 -> 2
// frame 4 in: 1 -> 2
// frame 5 in: 1 -> 1  
  
  
int getMedian(std::vector<int>& list, int frameNum, size_t N) {
  
  if (list.size() == N) {
    list.pop_back();
  }
  list.push_back(frameNum);
  
  std::vector<int> sortedList = list;
  std::sort(sortedList.begin(), sortedList.end());
  return sortedList[sortedList.size()/2];
}


int main() {
  size_t N = 3;
  std::vector<int> list(N, 0);
  int median = getMedian(list, 1, N);  // {0, 0, 0} ==> {0,0,1} ==>0
  median = getMedian(list, 2, N);  // {0, 0, 1} ==> {0,1,2} ==>1
  median = getMedian(list, 2, N);  // {0, 1, 2} ==> {1,2,2} ==>2
  median = getMedian(list, 1, N);  // {1, 2, 2} ==> {2,2, 1} == {1, 2, 2} ==>2
  median = getMedian(list, 1, N);  // {2, 2, 1} ==> {,1, 2,1,1} ==>1
}
