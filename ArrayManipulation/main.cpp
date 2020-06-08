/**
 * https://www.hackerrank.com/challenges/crush/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=arrays
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>


using namespace std;

inline void DisplayVector(const vector<long>& array) {
  for (auto item : array) {
    cout << setw(4) << item << "    ";
  }
  cout << endl;
}

// Complete the arrayManipulation function below.
long arrayManipulation(int n, const vector<vector<int>>& queries) {
  vector<long> array(n, 0);

  for (auto query : queries) {
    int beginIndex = query[0];
    int endIndex = query[1];
    int k = query[2];
    array[beginIndex - 1] += k;
    if (endIndex < n) {
      array[endIndex] -= k;
    }
  }

  DisplayVector(array);
  long allSum = array[0];
  long max = array[0];
  for (size_t index = 1; index < n; index++) {
    allSum += array[index];
    if (allSum > max) {
      max = allSum;
    }
  }
  return max;
}

vector<string> split_string(string);

int main() {
  string nm_temp;
  getline(cin, nm_temp);

  vector<string> nm = split_string(nm_temp);

  int n = stoi(nm[0]);

  int m = stoi(nm[1]);

  vector<vector<int>> queries(m);
  for (int i = 0; i < m; i++) {
    queries[i].resize(3);

    for (int j = 0; j < 3; j++) {
      cin >> queries[i][j];
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  long result = arrayManipulation(n, queries);

  cout << result << "\n";
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

