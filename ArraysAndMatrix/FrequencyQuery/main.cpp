// https://www.hackerrank.com/challenges/lookup-queries/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dictionaries-hashmaps

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

// Complete the freqQuery function below.
vector<int> freqQuery(const vector<vector<int>>& queries) {
  vector<int> result;
  unordered_map<int, unsigned long long> values;

  for (auto query : queries) {
    int lookup = query[1];
    switch (query[0]) {
    case 1: {
      auto found = values.find(lookup);
      if (found != values.end()) {
        // increment the current count.
        found->second++;
      } else {
        // First occarance.
        values.insert(std::make_pair(lookup, 1));
      }
      break;
    }
    case 2: {
      auto found = values.find(lookup);
      if (found != values.end() && found->second > 0) {
        // If found and the count is > 0, decrement it.
        found->second--;
      }
      break;
    }
    case 3: {
      // hash table lookup by value (not key).
      auto found = std::find_if(values.begin(), values.end(), [lookup](auto&& item) {
        return (item.second == lookup);
      });
      // If there is an entry with given frequency update the result.
      if (found != values.end()) {
        result.push_back(1);
      } else {
        result.push_back(0);
      }
      break;
      }
    default:
      break;
    }
  }

  return result;
}

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

int main() {
  string q_temp;
  getline(cin, q_temp);

  int q = stoi(ltrim(rtrim(q_temp)));

  vector<vector<int>> queries(q);

  for (int i = 0; i < q; i++) {
    queries[i].resize(2);

    string queries_row_temp_temp;
    getline(cin, queries_row_temp_temp);

    vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

    for (int j = 0; j < 2; j++) {
      int queries_row_item = stoi(queries_row_temp[j]);

      queries[i][j] = queries_row_item;
    }
  }

  vector<int> ans = freqQuery(queries);

  for (int i = 0; i < ans.size(); i++) {
    cout << ans[i];

    if (i != ans.size() - 1) {
      cout << "\n";
    }
  }

  cout << "\n";

  return 0;
}

string ltrim(const string &str) {
  string s(str);

  s.erase(
    s.begin(),
    find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
  );

  return s;
}

string rtrim(const string &str) {
  string s(str);

  s.erase(
    find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
    s.end()
  );

  return s;
}

vector<string> split(const string &str) {
  vector<string> tokens;

  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = str.find(" ", start)) != string::npos) {
    tokens.push_back(str.substr(start, end - start));

    start = end + 1;
  }

  tokens.push_back(str.substr(start));

  return tokens;
}

