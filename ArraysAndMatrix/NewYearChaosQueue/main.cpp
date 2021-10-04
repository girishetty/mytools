/**
 * https://www.hackerrank.com/challenges/new-year-chaos/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=arrays
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

void minimumBribes(const vector<int>& q) {
  int bribeCount = 0;

  for (int index = q.size() - 1; index >= 0; index--) {
    int position = index + 1;
    int shifts = q[index] - position;
#ifdef DEBUG
    cout << "Index: " << index << " Position: " << q[index] << " Actual Position: "
         << position << " Shifts: " << shifts << endl;
#endif
    if (shifts > 2) {
      cout << "Too chaotic" << endl;
      return;
    }

    for (int overtakes = max(0, q[index] - 2); overtakes < index; overtakes++) {
      if (q[overtakes] > q[index]) {
        bribeCount++;
      }
    }
  }

  cout << bribeCount << endl;
}

vector<string> split_string(string);

int main() {
  int t;
  cin >> t;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  for (int t_itr = 0; t_itr < t; t_itr++) {
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string q_temp_temp;
    getline(cin, q_temp_temp);

    vector<string> q_temp = split_string(q_temp_temp);

    vector<int> q(n);

    for (int i = 0; i < n; i++) {
      int q_item = stoi(q_temp[i]);

      q[i] = q_item;
    }

    minimumBribes(q);
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

