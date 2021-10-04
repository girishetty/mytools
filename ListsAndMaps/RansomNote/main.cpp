// https://www.hackerrank.com/challenges/ctci-ransom-note/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dictionaries-hashmaps

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <unordered_map>

using namespace std;

// Complete the twoStrings function below.
string twoStrings(const string& s1, const string& s2) {
  const string* whichOne = &s1;
  const string* otherOne = &s2;
  size_t hashTableSize = s1.size();
  if (hashTableSize  > s2.size()) {
    hashTableSize = s2.size();
    whichOne = &s2;
    otherOne = &s1;
  }
  // Considering String s1/s2 contains only lowercase letters,
  // we can construct a table counting no of occurances of each letters.
  vector<size_t> countChars(26, 0);
  for (auto ch : *whichOne) {
    countChars[ch - 'a']++;
  }

  string common = "NO";
  for (auto ch : *otherOne) {
    if (countChars[ch - 'a'] > 0) {
      common = "YES";
      break;
    }
  }

  return common;
}

// Complete the checkMagazine function below.
void checkMagazine(const vector<string>& magazine, const vector<string>& note) {

  // Add it to a hash table or <word, count>
  unordered_map<string, size_t> magazineSet;
  for (auto& item : magazine) {
    auto found = magazineSet.find(item);
    if (found != magazineSet.end()) {
      found->second++;
    } else {
      magazineSet.insert(std::make_pair(item, 1));
    }
  }

  // Check if we have enough words to construct the note.
  string can = "Yes";
  for (auto& item : note) {
    auto found = magazineSet.find(item);
    if (found != magazineSet.end() && found->second > 0) {
      found->second--;
    } else {
      can = "No";
      break;
    }
  }

  cout << can << endl;
}

vector<string> split_string(string);

int main(int argc, char** argv) {
  if (argc > 1) {
    string mn_temp;
    getline(cin, mn_temp);

    vector<string> mn = split_string(mn_temp);

    int m = stoi(mn[0]);

    int n = stoi(mn[1]);

    string magazine_temp_temp;
    getline(cin, magazine_temp_temp);

    vector<string> magazine_temp = split_string(magazine_temp_temp);
    vector<string> magazine(m);

    for (int i = 0; i < m; i++) {
      string magazine_item = magazine_temp[i];
      magazine[i] = magazine_item;
    }

    string note_temp_temp;
    getline(cin, note_temp_temp);

    vector<string> note_temp = split_string(note_temp_temp);
    vector<string> note(n);

    for (int i = 0; i < n; i++) {
      string note_item = note_temp[i];
      note[i] = note_item;
    }
 
    checkMagazine(magazine, note);
  } else {
    vector<string> magazine = { "ive", "got", "a", "lovely", "bunch", "of", "coconuts" };
    vector<string> note1 = { "ive", "got", "some", "coconuts" };
    checkMagazine(magazine, note1);
    vector<string> note2 = { "ive", "got", "a", "coconuts" };
    checkMagazine(magazine, note2);
  }


  string s1 = "hello";
  string s2 = "world";
  cout << "Does " << s1 << " and " << s2 << " share commonn substring? " << twoStrings(s1, s2) << endl;
  s1 = "hi";
  cout << "Does " << s1 << " and " << s2 << " share commonn substring? " << twoStrings(s1, s2) << endl;
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

