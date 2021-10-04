// https://www.hackerrank.com/challenges/sherlock-and-anagrams/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dictionaries-hashmaps

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

// Give all substrings of size len from the source.
vector<string> getAllSubstring(const string& source, size_t len) {
  vector<string> substrings;

  for (size_t startIndex = 0; startIndex <= source.size() - len; startIndex++) {
    substrings.push_back(source.substr(startIndex, len));
  }

  return substrings;
}

//#define USE_SORTING

// Complete the sherlockAndAnagrams function below.
int sherlockAndAnagrams(const string& s) {
  int count = 0;
  for (size_t len = 1; len <= s.size(); len++) {
    auto substrings = getAllSubstring(s, len);

#ifdef USE_SORTING
    // sort the substrings and then compare to see if we find another identical substring.
    if (len > 1) {
      for (auto& item : substrings) {
        std::sort(item.begin(), item.end());
      }
    }
    for (size_t lhs = 0; lhs < substrings.size() - 1; lhs++) {
      for (size_t rhs = lhs + 1; rhs < substrings.size(); rhs++) {
        if (substrings[lhs] == substrings[rhs]) {
          count++;
        }
      }
    }
#else
    // Considering String s contains only lowercase letters,
    // we can construct a table counting no of occurances of each letters.
    vector<vector<size_t>> countCharsList(substrings.size(), vector<size_t>(26, 0));
    int index = 0;
    for (const auto& item : substrings) {
      for (auto ch : item) {
        countCharsList[index][ch - 'a']++;
      }
      index++;
    }
    for (size_t lhs = 0; lhs < substrings.size() - 1; lhs++) {
      for (size_t rhs = lhs + 1; rhs < substrings.size(); rhs++) {
        if (countCharsList[lhs] == countCharsList[rhs]) {
          count++;
        }
      }
    }
#endif
  }

  return count;
}

int main(int argc, char** argv) {
  if (argc > 1) {
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
      string s;
      getline(cin, s);

      int result = sherlockAndAnagrams(s);

      cout << result << "\n";
    }
  } else {
    string s = "ifailuhkqq";
    cout << "String: " << s << " No of anagrams: " << sherlockAndAnagrams(s) << endl;
    s = "kkkk";
    cout << "String: " << s << " No of anagrams: " << sherlockAndAnagrams(s) << endl;
    s = "abcd";
    cout << "String: " << s << " No of anagrams: " << sherlockAndAnagrams(s) << endl;
  }

  return 0;
}

