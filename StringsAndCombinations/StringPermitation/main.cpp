/**
 * https://www.youtube.com/watch?v=nYFd7VHKyWQ
 *
 */

#define FASTER

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#ifdef FASTER
#include <unordered_map>
#else
#include <map>
#endif

using namespace std;

void makePermutation(const string& input, vector<size_t>& counter, string& result, size_t level) {
  if (level == result.size()) {
    cout << result << endl;
    return;
  }

  for (size_t index = 0; index < input.size(); index++) {
    if (counter[index] == 0) {
      continue;
    }
    result[level] = input[index];
    counter[index]--;
    makePermutation(input, counter, result, level + 1);
    counter[index]++;
  }
}

void permutation(const string& A) {
  // Using map to find all unique characters and their occurances (count)
#ifdef FASTER
  unordered_map<char, size_t> countMap;
#else
  map<char, size_t> countMap;
#endif
  std::for_each(A.begin(), A.end(), [&countMap](char ch) { countMap[ch]++; });

  string input;
  vector<size_t> counter;
  std::for_each(countMap.begin(), countMap.end(), [&input, &counter](const pair<char, size_t>& item) {
    input.push_back(item.first);
    counter.push_back(item.second);
  });

  string result(A.size(), '\0');
  // Now lets create permutations of this input
  makePermutation(input, counter, result, 0);
}

int main() {
  string A("aab");
  cout << "Enter a string for which you are looking for possible Permutations: ";
  cin >> A;

  cout << "Permutations of " << A << " are: " << endl;
  permutation(A);

  return 0;
}
