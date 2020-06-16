// https://www.hackerrank.com/challenges/count-triplets-1/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dictionaries-hashmaps

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <unordered_map>

using namespace std;

// Complete the countTriplets function below.
long countTriplets(const vector<long>& arr, long r) {
  long count = 0;

#ifdef SOLUTION_1
  size_t arraySize = arr.size();
  unordered_map<long, vector<size_t>> table;

  for (size_t index = 0; index < arraySize; index++) {
    table[arr[index]].push_back(index);
  }

  // Handle the special case where in there is only one item with multiple occurances.
  if (table.size() == 1 && r == 1) {
    //                              n!
    // Formula here is C(n,r) =  -------
    //                           r!(n-r)!
    if (arraySize > 2) {
      // Calculate nCr.
      double nCr = (arraySize / 6.0) * (arraySize - 1) * (arraySize - 2);
      count = static_cast<long>(nCr);
    }
    return count;
  }
  for (size_t index = 0; index < arraySize - 2; index++) {
    auto found1 = table.find(arr[index]);
    long nextItem = arr[index] * r;
    long nextNextItem = nextItem * r;
    auto found2 = table.find(nextItem);
    if (found2 != table.end()) {
      auto found3 = table.find(nextNextItem);
      if (found3 != table.end()) {
        count += 1 + found1->second.size() - 1 +
                     found2->second.size() - 1 +
                     found3->second.size() - 1;
        cout << count;
      }
    }
  }
#else
  // Idea here is that maintain 2 hash tables, which
  // maintains the pair of <value, occurances>
  // One that is for the 2nd occurance (vale * r, occurance)
  // One that is for the 3rd occurance (vale * r *, occurance)
  // For every item, do this following:
  //  - Check if its in 2nd hash table.
  //    If so, we have found the triplet - increment the count based on this occurance.
  //  - Check if its in the 1st hash table
  //    If so, push (value * r) to the 2nd hash table (that we are goint to check it later)
  //    (which also increases the occurance count in there)
  //  - Add (value * r) to the 2nd hash table (that we are goint to check it later)
  //    And also increment the occurance count.
  unordered_map<long, size_t> secondOccurance;
  unordered_map<long, size_t> thirdOccurance;

  for (auto value : arr) {
    auto found3 = thirdOccurance.find(value);
    if (found3 != thirdOccurance.end()) {
      // We have found the triplets.
      count += found3->second;
    }
    auto found2 = secondOccurance.find(value);
    if (found2 != secondOccurance.end()) {
      // Since we have found this in 2nd part of the triplet, let us update the
      // 3rd part of trippet that we are expecting this.
      thirdOccurance[value * r] += found2->second;
    }
    // Lets add this to 2nd part of trippet, as we are going to check it later.
    secondOccurance[value * r]++;
  }
#endif
  return count;
}

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

int main(int argc, char** argv) {
  if (argc > 1) {
    string nr_temp;
    getline(cin, nr_temp);

    vector<string> nr = split(rtrim(nr_temp));

    int n = stoi(nr[0]);
    long r = stol(nr[1]);

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));
    vector<long> arr(n);

    for (int i = 0; i < n; i++) {
      long arr_item = stol(arr_temp[i]);
      arr[i] = arr_item;
    }

    long ans = countTriplets(arr, r);

    cout << ans << "\n";
  } else {
    vector<long> arr1 = { 1, 3, 9, 9, 27, 81 };
    long r = 3;
    auto res = countTriplets(arr1, r);
    cout << endl << "No of triplets with geometric progression of " << r << " is: " << res << endl;
    vector<long> arr2 = { 1, 5, 5, 25, 125 };
    r = 5;
    res = countTriplets(arr2, r);
    cout << endl << "No of triplets with geometric progression of " << r << " is: " << res << endl;
    vector<long> arr3(100, 1);
    r = 1;
    res = countTriplets(arr3, r);
    cout << endl << "No of triplets with geometric progression of " << r << " is: " << res << endl;
  }

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

