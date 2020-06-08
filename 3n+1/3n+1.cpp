#include <iostream>
#include <vector>
#include <utility>

using namespace std;

int Three_n_plus_one(int n) {
  int count = 1;
  while (n != 1) {
    if (n & 0x01) {
      n = 3 * n + 1;
    } else {
      n = n >> 1;
    }
    count++;
  }
  return count;
}

int main() {
  vector<pair <int, int> > myList;
  myList.push_back(make_pair(1, 20));
  myList.push_back(make_pair(100, 200));
  myList.push_back(make_pair(201, 210));
  myList.push_back(make_pair(900, 1000));

  vector<pair <int, int> >::const_iterator it;
  int max_cycle_len = 0;
  int cycle_len = 0;
  for (it = myList.begin(); it < myList.end(); it++) {
    max_cycle_len = 0;
    for (int begin = it->first; begin <= it->second; begin++) {
      cycle_len = Three_n_plus_one(begin);
      if (cycle_len > max_cycle_len) {
        max_cycle_len = cycle_len;
      }
    }
    cout<<it->first<<" "<<it->second<<" "<<max_cycle_len<<endl;
  }

  return 0;
}
