#include <vector>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
  
/* Function to print an array */
void printArray(const string& msg, const vector<int>& arr) {
  cout << msg << ": ";
  for (size_t i = 0; i < arr.size(); i++) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

inline void swap(int& a, int& b) {
  int t = a;
  a = b;
  b = t;
}

typedef size_t (*partition_function)(vector<int>& arr, size_t low, size_t high);

int findKthLargest(vector<int>& arr, size_t low, size_t high, size_t position, partition_function partition) {
  if (low < high) {
    // pi is partitioning index, arr[p] is now at right place
    size_t pi = partition(arr, low, high);

    if (position == pi) {
      return arr[pi];
    }
    // Separately sort elements before partition and after partition
    if (pi > low && position < pi) {
      return findKthLargest(arr, low, pi - 1, position, partition);
    }
    if (pi < high && position > pi) {
      return findKthLargest(arr, pi + 1, high, position, partition);
    }
  }
  if (low == position) {
    return arr[low];
  }
  return -1;
}

int findKthLargest(vector<int>& arr, size_t K, partition_function partition) {
  return findKthLargest(arr, 0, arr.size() - 1, arr.size() - K, partition);
}

/**
 * This function takes middle element as pivot, places the pivot element
 * at its correct position in sorted array, and places all smaller (smaller than pivot)
 * to left of pivot and all greater elements to right of pivot.
 */
size_t partition_middle_pivot(vector<int>& arr, size_t low, size_t high) {
  int pi = low + ((high - low) / 2);
  int pivot = arr[pi];
  int i = low;
  int j = high;

  for (; i < j;) {
    // Move i until we find an item bigger than pivot
    for (; arr[i] <= pivot; i++);

    // Move j until we find an item not bigger than pivot
    for (; arr[j] > pivot; j--);

    // Check if we can swap i and j
    if (i < j) {
      swap(arr[i], arr[j]);
      i++;
      j--;
    }
  }

  // Now swap pivot with the arr[j]. Thats where the parition happens now.
  if (pi < j) {
    swap(arr[pi], arr[j]);
  }

  return j;
}

/**
 * This function takes last element as pivot, places the pivot element
 * at its correct position in sorted array, and places all smaller (smaller than pivot)
 * to left of pivot and all greater elements to right of pivot.
 */
size_t partition_high_pivot(vector<int>& arr, size_t low, size_t high) {
  int pivot = arr[high]; // pivot
  // i will hold the index of element that is >= pivot
  size_t i = low;

  for (size_t j = low; j < high; j++) {
    // If current element is smaller than the pivot
    if (arr[j] < pivot) {
      if (i != j) {
        swap(arr[i], arr[j]);
      }
      // Since this element is smaller than pivot, lets hope that next one is >=
      i++;
    }
  }
  if (i < high) {
    // Since i points to an index where we have the first element that is >= pivot
    // we can swap them to create a parition <smaller elements> pivot <equal or greater elements>
    swap(arr[i], arr[high]);
  }

  return i;
}

/**
 * The main function that implements QuickSort
 * arr[] --> Array to be sorted,
 * low --> Starting index,
 * high --> Ending index
 */
void quickSort(vector<int>& arr, size_t low, size_t high, partition_function partition) {
  if (low < high) {
    // pi is partitioning index, arr[p] is now at right place
    size_t pi = partition(arr, low, high);

    // Separately sort elements before partition and after partition
    if (pi > low) {
      quickSort(arr, low, pi - 1, partition);
    }
    if (pi < high) {
      quickSort(arr, pi + 1, high, partition);
    }
  }
}

void testQuickSort(const std::vector<int>& arr, size_t K) {
  std::vector<int> toSort1 = arr;
  printArray("Orignal Array", arr);
  quickSort(toSort1, 0, arr.size() - 1, partition_middle_pivot);
  printArray("Sorted Array with Middle Pivot", toSort1);
  std::vector<int> toSort2 = arr;
  quickSort(toSort2, 0, arr.size() - 1, partition_high_pivot);
  printArray("Sorted Array with High Pivot  ", toSort2);
  if (toSort2 != toSort1) {
    std::cout << "Error!! Mismatch in sorting order from 2 different approach!" << std::endl;
  } else {
    std::cout << "Both Pivot worked successfully" << std::endl;
  }
  toSort1 = arr;
  int kth_largest = findKthLargest(toSort1, K, partition_middle_pivot);
  toSort2 = arr;
  if (kth_largest == findKthLargest(toSort2, K, partition_high_pivot)) {
    std::cout << "K[" << K <<"]th Largest Number is: " << kth_largest << std::endl;
  } else {
    std::cout << "Error!! Mismatch in K[" << K <<"]th Largest Number from 2 different approach!" << std::endl;
  }
  printArray("Partially Sorted Array", toSort2);
  std::cout << "===================================================================================" << std::endl;
}

int main() {
  vector<int> arr{10, 7, 8, 9, 1, 5, 3, 0, 12, 0, 4, 2, 1};
  testQuickSort(arr, 3);
  vector<int> arr_1{10, 7, 0, 8, 0, 9, 1, 5};
  testQuickSort(arr_1, 3);
  vector<int> arr_2 {3,2,1,5,6,4};
  testQuickSort(arr_2, 3);
  vector<int> arr_3 {2,1};
  testQuickSort(arr_3, 2);
  vector<int> arr_4 {1,2,1,3,5,6,4};
  testQuickSort(arr_4, 1);
  return 0;
}
