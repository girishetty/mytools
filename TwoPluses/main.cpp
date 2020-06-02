/**
 * https://www.hackerrank.com/challenges/two-pluses/problem
 */

/**
Sample input#1:
5 6
GGGGGG
GBBBGB
GGGGGG
GGBBGB
GGGGGG

Output#1:
5

Sample input#1:
6 6
BGBBGB
GGGGGG
BGBBGB
GGGGGG
BGBBGB
BGBBGB

Output#1:
25
*/

#include <list>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace std;

class Plus {
public:
  Plus(int x = 0, int y = 0, int s = 0) : 
    centerRowIndex(x),
    centerColIndex(y),
    size(s) {
    area = 4 * size + 1;
  }

  bool isDisconnected(const Plus& other) const {
    if (other.centerRowIndex == centerRowIndex && other.centerColIndex == centerColIndex) {
      return false;
    }
    if (area == 1 || other.area == 1) {
      return true;
    }

    int colLeftEdge = centerColIndex - size;
    int colRightEdge = centerColIndex + size;
    int rowTopEdge = centerRowIndex - size;
    int rowBottomEdge = centerRowIndex + size;

    // See if the column of others touches column of ours.
    if (other.centerColIndex == centerColIndex &&
        other.centerRowIndex + other.size >= rowTopEdge) {
      return false;
    }

    // See if the row of others touches row of ours.
    if (other.centerRowIndex == centerRowIndex &&
        other.centerColIndex + other.size >= colLeftEdge) {
      return false;
    }

    // See if the column of others touches the row of ours.
    if (colLeftEdge <= other.centerColIndex && colRightEdge >= other.centerColIndex) {
      if (other.centerRowIndex < centerRowIndex) {
        if (other.centerRowIndex + other.size >= centerRowIndex) {
          return false;
        }
      } else if (other.centerRowIndex - other.size <= centerRowIndex) {
        return false;
      }
    }

    // See if the row of others touches the column of ours.
    if (rowTopEdge <= other.centerRowIndex && rowBottomEdge >= other.centerRowIndex) {
      if (other.centerColIndex < centerColIndex) {
        if (other.centerColIndex + other.size >= centerColIndex) {
          return false;
        }
      } else if (other.centerColIndex - other.size <= centerColIndex) {
        return false;
      }
    }

    return true;
  }

  int getArea() const {
    return area;
  }

  int getSize() const {
    return size;
  }

  friend ostream& operator<<(ostream& os, const Plus& plus);

private:
  int centerRowIndex = 0;
  int centerColIndex = 0;
  int size = 0;
  int area = 1;
};

ostream& operator<<(ostream& os, const Plus& plus) {
  os << "[(" << plus.centerRowIndex << ", " << plus.centerColIndex
     << ") : (" << plus.size << ", " << plus.area << ")]" << endl;
   return os;
}

bool operator>=(const Plus& lhs, const Plus& rhs) {
  return (lhs.getSize() >= rhs.getSize());
}

bool operator<(const Plus& lhs, const Plus& rhs) {
  return (lhs.getSize() < rhs.getSize());
}

//#define DEBUG_ALL
#define DEBUG_LESS

int twoPluses(const vector<string>& grid) {
  vector<Plus> maxPlusList;
  size_t rows = grid.size();
  size_t cols = grid[0].size();
  maxPlusList.push_back(Plus());

  for (size_t i = 1 ; i < rows - 1; i++) {
    for (size_t j = 1 ; j < cols - 1; j++) {
      if (grid[i][j] == 'G') {
        // We have got a plus. Lets find the size of it.
        int width = 0;
        int height = 0;
        int topIndex = i - 1;
        int bottomIndex = i + 1;
        for (; topIndex >= 0 && bottomIndex < rows; topIndex--, bottomIndex++) {
          if (grid[topIndex][j] != 'G' || grid[bottomIndex][j] != 'G') {
            break;
          }
          height++;
        }
        int leftIndex = j - 1;
        int rightIndex = j + 1;
        for (; leftIndex >= 0 && rightIndex < cols; leftIndex--, rightIndex++) {
          if (grid[i][leftIndex] != 'G' || grid[i][rightIndex] != 'G') {
            break;
          }
          width++;
        }

        auto size = std::min(width, height);
        if (size == 0) {
          continue;
        }
#ifdef DEBUG_ALL
        cout << "[" << i << ":" << j << "] (" << width << ", " << height << ": " << size << ") "  << endl;
#endif

        for (int possibilities = size; possibilities >= 1; possibilities--) {
          Plus newSize(i, j, possibilities);
          maxPlusList.insert(std::upper_bound(maxPlusList.begin(), maxPlusList.end(), newSize), newSize);
        }
      }
    }
  }

  int maxProd = 1;
  for (int index = maxPlusList.size() - 1; index > 0; index--) {
    const Plus& max = maxPlusList[index];
    int prod = max.getArea();
#ifdef DEBUG_LESS
    cout << "Index: " << index << "-->  " << maxPlusList[index];
#endif
    for (int inner = index - 1; inner >= 0; inner--) {
      if (max.isDisconnected(maxPlusList[inner])) {
        prod *= maxPlusList[inner].getArea();
        if (maxProd < prod) {
          maxProd = prod;
        }
        break;
      }
    }
  }
  return maxProd;
}

vector<string> split_string(string);

int main() {
  string nm_temp;
  getline(cin, nm_temp);

  vector<string> nm = split_string(nm_temp);

  int n = stoi(nm[0]);

  int m = stoi(nm[1]);

  vector<string> grid(n);

  for (int i = 0; i < n; i++) {
    string grid_item;
    getline(cin, grid_item);

    grid[i] = grid_item;
  }

  int result = twoPluses(grid);
  cout << "Result: " << result << endl;

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
