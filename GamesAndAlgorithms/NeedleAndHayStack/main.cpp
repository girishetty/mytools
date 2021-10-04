#include <iostream>
#include <algorithm>
#include <vector>
 
//#define USE_BINARY_SEARCH

int main() {
  std::vector<int> haystack {1, 3, 4, 5, 9};
  std::vector<int> needles {1, 2, 3};
 
  auto start = haystack.begin();
  for (auto needle : needles) {
#ifdef USE_BINARY_SEARCH
    std::cout << "Searching for " << needle << '\n';
    if (std::binary_search(haystack.begin(), haystack.end(), needle)) {
      std::cout << "Found: " << needle << '\n';
    } else {
      std::cout << "no dice!\n";
    }
#endif
    start = std::lower_bound(start, haystack.end(), needle);
    if (!(start == haystack.end()) && !(needle < *start)) {
      std::cout << "Found: " << needle << '\n';
    } else {
      std::cout << "no dice!\n";
    }
  }
}
