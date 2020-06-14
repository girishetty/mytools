/**
 * https://www.youtube.com/watch?v=nYFd7VHKyWQ
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

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
  unordered_map<char, size_t> countMap;
  std::for_each(A.begin(), A.end(), [&countMap](char ch) {countMap[ch]++; });

  string input;
  vector<size_t> counter;
  std::for_each(countMap.begin(), countMap.end(), [&input, &counter](const pair<char, size_t>& item) {
    input.push_back(item.first);
    counter.push_back(item.second);
  });

  string result(A.size(), '\0');
  makePermutation(input, counter, result, 0);
}

int main() {
  string A("aab");
  cin >> A;

  permutation(A);

  return 0;
}
