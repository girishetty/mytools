/**
 * https://www.hackerrank.com/challenges/max-array-sum/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dynamic-programming
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

// Complete the maxSubsetSum function below.
int maxSubsetSum(const vector<int>& arr) {
  size_t size = arr.size();
  // Case where there aren't any element ==> 0
  vector<int> sum (size == 0? 1 : size, 0);
  if (size > 0) {
    // Case where there is only one element ==> { (0) }
    sum[0] = arr[0];
    if (size > 1) {
      // Case where there are only two element ==> { (0), (1) }
      sum[1] = max(arr[0], arr[1]);
      for (size_t index = 2; index < size; index++) {
        // At this point, what we are looking at is elementes at indeces: [0, 1, 2]
        // And our options are set of { (0, 2), (1), (2) }
        // Pick the set that adds up to the maximum
#ifdef SLOWER_STD_MAX
        //Using max with initializer list takes more time than the one below!
        sum[index] = max({sum[index - 1], sum[index - 2] + arr[index], arr[index]});
#else
        sum[index] = max(sum[index - 1], sum[index - 2] + arr[index]);
        sum[index] = max(sum[index], arr[index]);
#endif
      }
    }
  }

  return sum[sum.size() - 1];
}

vector<string> split_string(string);

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

    int res = maxSubsetSum(arr);

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


