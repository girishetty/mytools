/**
 * https://www.hackerrank.com/challenges/minimum-swaps-2/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=arrays
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

inline void DisplayArray(const vector<int>& arr) {
  for (auto item : arr) {
    cout << setw(4) << item << "    ";
  }

  cout << endl;
}

#define USE_SORTING

#ifdef USE_SORTING
int minimumSwaps(vector<int>& arr) {
  vector<int> sortedArray = arr;
  std::sort(sortedArray.begin(), sortedArray.end());
  int    swapCount = 0;

  for (size_t index = 0; index < arr.size(); index++) {
    if (arr[index] != sortedArray[index]) {
      for (auto j = index + 1; j < arr.size(); j++) {
        if(arr[j] == sortedArray[index]) {
          // now swap these two.
          auto temp = arr[index];
          arr[index] = arr[j];
          arr[j] = temp;
          swapCount++;
          break;
        }
      }
    }
  }

  return swapCount;
}

#else
int minimumSwaps(vector<int>& arr) {
  size_t beginIndex = 0;
  size_t arrayEnd = arr.size() - 1;
  size_t endIndex = arrayEnd;
  int    swapCount = 0;

  do {
    // Check if its already sorted.
    for (; beginIndex < arrayEnd && arr[beginIndex] <= arr[beginIndex + 1]; beginIndex++);
    if (beginIndex == arrayEnd) {
      return swapCount;
    }

    // approach from the end towards begining.
    for (; endIndex > beginIndex && arr[endIndex] >= arr[endIndex - 1]; endIndex--);

    if (beginIndex != endIndex) {
      // now swap these two.
      auto temp = arr[beginIndex];
      arr[beginIndex] = arr[endIndex];
      arr[endIndex] = temp;
      swapCount++;
    }

    // Check if we need to reset beginIndex.
    if (beginIndex > 0 && (arr[beginIndex] < arr[beginIndex - 1])) {
      beginIndex = 0;
    }

    // Check if we need to reset beginIndex.
    if (endIndex - beginIndex <= 1) {
      endIndex = arrayEnd;
    }

  } while (true);

  return swapCount;
}
#endif

vector<string> split_string(string input_string);

int main() {
  int n;
  cin >> n;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  string arr_temp_temp;
  getline(cin, arr_temp_temp);

  vector<string> arr_temp = split_string(arr_temp_temp);

  vector<int> arr(n);

  for (int i = 0; i < n; i++) {
    int arr_item = stoi(arr_temp[i]);

    arr[i] = arr_item;
  }

  int res = minimumSwaps(arr);

  cout << res << "\n";

  return 0;
}

vector<string> split_string(string input_string) {
  string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
    return x == y and x == ' ';
  });

  input_string.erase(new_end, input_string.end());

  while (input_string[input_string.length() - 1] == ' ') {
    input_string.pop_back();
  }

  vector<string> splits;
  char delimiter = ' ';

  size_t i = 0;
  size_t pos = input_string.find(delimiter);

  while (pos != string::npos) {
    splits.push_back(input_string.substr(i, pos - i));

    i = pos + 1;
    pos = input_string.find(delimiter, i);
  }

  splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

  return splits;
}
