#include <vector>
#include <iostream>

void displayVectors(const std::vector<std::vector<int>>& list) {
  for (auto item : list) {
    for (size_t index = 0; index < item.size(); index++) {
      std::cout << item[index] << "  ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main() {
  std::vector<std::vector<int>> list = { {13, 14, 15, 16},
                                         {1, 4, 3, 2},
                                         {7, 5, 7, 6},
                                         {9, 10, 11, 12}};

  displayVectors(list);
  std::sort(list.begin(), list.end());
  displayVectors(list);
  return 0;
}

