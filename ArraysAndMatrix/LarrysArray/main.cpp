#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

/*
 * https://www.hackerrank.com/challenges/larrys-array/problem
 */

using namespace std;

inline void rotateByOne(vector<int>& A, size_t index) {
  auto temp = A[index];
  A[index] = A[index + 1];
  A[index + 1] = A[index + 2];
  A[index + 2] = temp;
}

inline bool isSorted(const vector<int>& A, size_t& firstUnsortedIndex) {
  bool sorted = false;
  size_t index = 0;
  for (index = 0; !sorted && index < A.size() - 1; index++) {
    if (A[index] > A[index + 1]) {
      break;
    }
  }
  if (index == A.size() - 1) {
    sorted = true;
  } else {
    firstUnsortedIndex = index;
  }

  return sorted;
}

// Complete the larrysArray function below.
string larrysArray(vector<int> A) {
  size_t beginIndex = 0;
  size_t index = 0;
  size_t maxRotationCount = 3;
  size_t arraySize = A.size();
  bool   sorted = false;
  bool   rotated = false;

  // Check if the array is already sorted.
  // If not get the range of idex [begin, begin + 1, begin + 2]
  while (beginIndex < arraySize) {
    size_t firstUnsortedIndex = 0;
    sorted = isSorted(A, firstUnsortedIndex);
    if (sorted) {
      break;
    }
    size_t index = max(firstUnsortedIndex, beginIndex);
    for ( ;index < arraySize - 1; index++) {
      if (A[index] > A[index + 1]) {
        auto diff = arraySize - index;
        if (diff <= maxRotationCount) {
          // rotate the last 3 elements
          beginIndex = arraySize - maxRotationCount;
          break;
        } else {
          // rotate the first 3 elements
          beginIndex = index;
          break;
        }
      }
    }

    if (rotated && (index == arraySize - 1)) {
      beginIndex = 0;
      rotated = false;
      continue;
    }

    auto smallest = std::min({A[beginIndex], A[beginIndex + 1], A[beginIndex + 2]});
    if (smallest != A[beginIndex]) {
      rotated = true;
      // rotate the 3 elements by one now.
      rotateByOne(A, beginIndex);
      smallest = std::min({A[beginIndex], A[beginIndex + 1], A[beginIndex + 2]});
      if (smallest != A[beginIndex]) {
        // rotate one more time.
        rotateByOne(A, beginIndex);
      }
    }
    if (rotated && (arraySize - beginIndex == maxRotationCount)) {
      // start all over again.
      beginIndex = 0;
      rotated = false;
    } else {
      beginIndex += maxRotationCount;
    }
  }

  return sorted ? "YES" : "NO";
}

vector<string> split_string(string);

int main() {
  int t;
  cin >> t;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  vector<string> results;

  for (int t_itr = 0; t_itr < t; t_itr++) {
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string A_temp_temp;
    getline(cin, A_temp_temp);

    vector<string> A_temp = split_string(A_temp_temp);

    vector<int> A(n);

    for (int i = 0; i < n; i++) {
      int A_item = stoi(A_temp[i]);

      A[i] = A_item;
    }

    string result = larrysArray(A);
    cout << endl << result;
    results.push_back(result);
  }

  for (auto result : results) {
    cout << endl << result;
  }

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
