#include <string>
#include <vector>
#include <iostream>

using namespace std;

// https://www.hackerrank.com/challenges/changing-bits/problem

int getNumber(const string& str, size_t& pos) {
  const char* digits = "0123456789";
  int val = -1;

  auto found = str.find_first_of(digits, pos);
  if (found != string::npos) {
    auto foundEnd = str.find_first_not_of(digits, found);
    val = stoi(str.substr(found, foundEnd != string::npos ? foundEnd - found : foundEnd));
    pos = foundEnd;
  }
  return val;
}

bool getIndexAndBit(const string& query, size_t& index, char& bit) {
  bool success = false;
  size_t pos = 4;
  int number = getNumber(query, pos);
  if (number >= 0) {
    success = true;
    index = number;
    number = getNumber(query, pos);
    if (number == 0) {
      bit = '0';
    } else if (number == 1) {
      bit = '1';
    } else {
      success = false;
    }
  }

  return success;
}

bool getIndex(const string& query, size_t& index) {
  bool success = false;
  size_t pos = 4;
  int number = getNumber(query, pos);
  if (number >= 0) {
    success = true;
    index = number;
  }

  return success;
}

void sum(const string& a, const string& b, string& c) {
  char carry = '0';
  size_t noOfBits = a.size();
  c.assign(noOfBits + 1, '0');

  for (int index = noOfBits - 1; index >= 0; index--) {
    if (a[index] == '0' && b[index] == '0') {
      c[index + 1] = carry;
      carry = '0';
    } else if ((a[index] == '0' && b[index] == '1') || (a[index] == '1' && b[index] == '0')) {
      if (carry == '0') {
        c[index + 1] = '1';
      }
    } else if (a[index] == '1' && b[index] == '1') {
      c[index + 1] = carry;
      carry = '1';
    }
  }

  c[0] = carry;
}

/*
 * The function accepts following parameters:
 *  1. STRING a
 *  2. STRING b
 *  3. STRING_ARRAY queries
 */

void changeBits(string& a, string& b, const vector<string>& queries) {
  string output;
  string c;
  string* toOperate = nullptr;
  size_t binarySize = a.size();
  size_t index = 0;
  char   bit = 0;

  for (auto query : queries) {
    toOperate = nullptr;
    if (query.find("set_a") == 0) {
      if (getIndexAndBit(query, index, bit)) {
        toOperate = &a;
      }
    } else if (query.find("set_b") == 0) {
      if (getIndexAndBit(query, index, bit)) {
        toOperate = &b;
      }
    } else if (query.find("get_c") == 0) {
      if (getIndex(query, index)) {
        sum(a, b, c);
        bit = '0';
        if (index < c.size()) {
          bit = c[c.size()-index];
        }
        output = bit + output;
      }
    }

    if (toOperate != nullptr && index < binarySize) {
      (*toOperate)[binarySize - 1 - index] = bit;
    }
  }

  cout << output << endl;
}

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

int main() {
  string first_multiple_input_temp;
  getline(cin, first_multiple_input_temp);

  vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

  int ab_size = stoi(first_multiple_input[0]);

  int queries_size = stoi(first_multiple_input[1]);

  string a;
  getline(cin, a);

  string b;
  getline(cin, b);

  vector<string> queries(queries_size);

  for (int i = 0; i < queries_size; i++) {
    string queries_item;
    getline(cin, queries_item);

    queries[i] = queries_item;
  }

  changeBits(a, b, queries);

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

