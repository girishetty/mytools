#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

uint64_t NumberOfBallsInContainer(const vector<int>& container) {
  uint64_t sum = 0;
  for (auto& items : container) {
    sum += items;
  }

  return sum;
}

uint64_t NumberOfBallsOfType(size_t type, const vector<vector<int>>& containers) {
  uint64_t sum = 0;
  for (auto& container : containers) {
    sum += container[type];
  }

  return sum;
}

// Complete the organizingContainers function below.
string organizingContainers(const vector<vector<int>>& containers) {

  vector<uint64_t> containerCapacity;
  vector<uint64_t> ballCapacity;
  for (size_t index = 0; index < containers.size(); index++) {
    containerCapacity.push_back(NumberOfBallsInContainer(containers[index]));
    ballCapacity.push_back(NumberOfBallsOfType(index, containers));
  }

  // sort the capacity lists
  std::sort(containerCapacity.begin(), containerCapacity.end());
  std::sort(ballCapacity.begin(), ballCapacity.end());
  if (containerCapacity != ballCapacity) {
    return "Impossible";
  }


  return "Possible";
}

int main() {
  int q;
  cin >> q;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  for (int q_itr = 0; q_itr < q; q_itr++) {
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> container(n);
    for (int i = 0; i < n; i++) {
      container[i].resize(n);

      for (int j = 0; j < n; j++) {
        cin >> container[i][j];
      }

      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string result = organizingContainers(container);
    cout << endl << "Result: " << result << endl;

  }

  return 0;
}

