/**
 * https://www.hackerrank.com/challenges/acm-icpc-team/problem
 */

/**
  Sample input:
4 5
10101
11100
11010
00101
*/

#include <vector>
#include <string>
#include <iostream>

using namespace std;

int getcombinedCourseCount(const string& topics1, const string& topics2) {
  int combinedCourse = 0;
  for (size_t index = 0; index < topics1.size(); index++) {
    if (topics1[index] == '1' || topics2[index] == '1') {
      combinedCourse++;
    }
  }

  return combinedCourse;
}

// Complete the acmTeam function below.
vector<int> acmTeam(const vector<string>& topics) {
  size_t noOfAttendees = topics.size();
  int    maxCourse = -1;
  vector<int> knowItAll(2, 0);

  for (size_t member1 = 0; member1 < noOfAttendees; member1++) {
    for (size_t member2 = member1 + 1; member2 < noOfAttendees; member2++) {
      auto combinedCourse = getcombinedCourseCount(topics[member1], topics[member2]);
      if (combinedCourse > maxCourse) {
        maxCourse = combinedCourse;
        knowItAll[0] = combinedCourse;
        knowItAll[1] = 1;
      } else if (combinedCourse == maxCourse) {
        knowItAll[1] += 1;
      }
      cout << member1 << ": " << member2 << ": " << combinedCourse << ": " << maxCourse
           << ": " << knowItAll[0] << ": " << knowItAll[1] << endl;
    }
  }
  return knowItAll;
}

vector<string> split_string(string);

int main() {
  string nm_temp;
  getline(cin, nm_temp);

  vector<string> nm = split_string(nm_temp);

  int n = stoi(nm[0]);

  int m = stoi(nm[1]);

  vector<string> topic(n);

  for (int i = 0; i < n; i++) {
    string topic_item;
    getline(cin, topic_item);

    topic[i] = topic_item;
  }

  vector<int> result = acmTeam(topic);

  for (int i = 0; i < result.size(); i++) {
    cout << result[i];

    if (i != result.size() - 1) {
      cout << "\n";
    }
  }

  cout << "\n";

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

