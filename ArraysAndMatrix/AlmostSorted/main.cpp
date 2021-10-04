/**
 * https://www.hackerrank.com/challenges/almost-sorted/problem
 */

/**
Sample input#1:
6
1 5 4 3 2 6

Output#1:
yes
reverse 2 5


Sample input#1:
4  
1 4 2 5

Output#1:
yes  
swap 1 2
*/

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>


using namespace std;

void almostSorted(vector<int>& arr) {
  size_t beginIndex = 0;
  size_t arrayEnd = arr.size() - 1;
  size_t endIndex = arrayEnd;
  size_t left = 0;
  size_t right = 0;

  // Check if its already sorted.
  for (; beginIndex < arrayEnd && arr[beginIndex] <= arr[beginIndex + 1]; beginIndex++);
  if (beginIndex == arrayEnd) {
    cout << "yes" << endl;
    return;
  }

  // approach from the end towards begining.
  for (; endIndex > beginIndex && arr[endIndex] >= arr[endIndex - 1]; endIndex--);
  if (left == 0 && right == 0) {
    left = beginIndex + 1;
    right = endIndex + 1;
  }
  if ((beginIndex == 0 || arr[endIndex] >= arr[beginIndex - 1]) &&
      (endIndex == arrayEnd || arr[beginIndex] <= arr[endIndex + 1])) {
    for (; (beginIndex < endIndex - 1) && arr[beginIndex] >= arr[beginIndex + 1]; beginIndex++);
    if (beginIndex == endIndex - 1) {
      cout << "yes" << endl;
      if (left != right - 1) {
        cout << "reverse " << left << " " << right << endl;
      } else {
        cout << "swap " << left << " " << right << endl;
      }
      return;
    } else {
      beginIndex--;
      auto temp = arr[beginIndex];
      arr[beginIndex] = arr[endIndex];
      arr[endIndex] = temp;
      for (; beginIndex < arrayEnd && arr[beginIndex] <= arr[beginIndex + 1]; beginIndex++);
      if (beginIndex == arrayEnd) {
        cout << "yes" << endl;
        cout << "swap " << left << " " << right << endl;
        return;
      }
    }
  }
  cout << "no" << endl;
}

vector<string> split_string(string);

int main()
{
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

  almostSorted(arr);

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

