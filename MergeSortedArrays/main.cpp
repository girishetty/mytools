#include <iostream>
#include <vector>

void displayVector(const char* name, const std::vector<size_t>& list) {
  std::cout << name << ": ";
  for (const auto& item : list) {
    std::cout << item << ' ';
  }
  std::cout << std::endl;
}

template <class T>
class MergedSortedArrays {
public:
  MergedSortedArrays(const std::vector<std::vector<T> >& arrays) :
    mArrays(arrays),
    mIndexs(arrays.size(), 0),
    mListSize(arrays[0].size()) {}

  T next();
  bool hasNext() const { return mConsumedItems < (mListSize * mIndexs.size());}

private:
  const std::vector<std::vector<T> >& mArrays;
  std::vector<size_t> mIndexs;
  const size_t mListSize;
  size_t mConsumedItems = 0;
};

template <class T>
T MergedSortedArrays<T>::next() {

  // Pick the next smallest element from all the sorted lists available.
  T smallest = std::numeric_limits<T>::max();
  int pickedIndex = -1;
  for (size_t index = 0; index < mIndexs.size(); index++) {
    size_t listIndex = mIndexs[index];
    if (listIndex >= mListSize) {
      // This list has been consumed already.
      continue;
    }
    if (smallest >= mArrays[index][listIndex]) {
      smallest = mArrays[index][listIndex];
      pickedIndex = index;
    }
  }
  if (pickedIndex != -1) {
    // The index should point to the next element now.
    mIndexs[pickedIndex] += 1;
    // Lets keep track of consumed elements.
    mConsumedItems += 1;
  } else {
    // We can throw an exception here if need be.
  }

  return smallest;
}

int main() {

  std::vector<int> v1 {1, 5, 7};
  std::vector<int> v2 {2, 3, 10};
  std::vector<int> v3 {4, 6, 9};

  std::vector<std::vector<int> > list {v1, v2, v3};

  MergedSortedArrays<int> sortedList (list);

  while (sortedList.hasNext()) {
    std::cout << sortedList.next() << " ";
  }
  return 0;
}
