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
  int pivot = arr[(low + high) >> 1];
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
  swap(pivot, arr[j]);  

  return j;
}

/**
 * This function takes last element as pivot, places the pivot element
 * at its correct position in sorted array, and places all smaller (smaller than pivot)
 * to left of pivot and all greater elements to right of pivot.
 */
size_t partition_high_pivot(vector<int>& arr, size_t low, size_t high) {
  int pivot = arr[high]; // pivot
  size_t i = low; // Index of smaller element

  for (size_t j = low; j < high; j++) {
    // If current element is smaller than the pivot
    if (arr[j] < pivot) {
      if (i != j) {
        swap(arr[i], arr[j]);
      }
      i++; // increment index of smaller element
    }
  }
  if (i < high) {
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

void testFindKthLargestNumber(std::vector<int> arr, size_t K, bool middle_pivot) {
  printArray("Orignal Array:", arr);
  int kth_largest = -1;
  if (middle_pivot) {
    kth_largest = findKthLargest(arr, K, partition_middle_pivot);
  } else {
    kth_largest = findKthLargest(arr, K, partition_high_pivot);
  }
  std::cout << "K[" << K <<"]th Largest Number is: " << kth_largest << std::endl;
}

void testQuickSort(std::vector<int> arr, bool middle_pivot) {
  printArray("Orignal Array:", arr);
  if (middle_pivot) {
    quickSort(arr, 0, arr.size() - 1, partition_middle_pivot);
  } else {
    quickSort(arr, 0, arr.size() - 1, partition_high_pivot);
  }
  printArray("Sorted  Array:", arr);
}

int main() {
  vector<int> arr{10, 7, 8, 9, 1, 5, 3, 0, 12, 0, 4, 2, 1};
  testQuickSort(arr, false);
  testQuickSort(arr, true);
  testFindKthLargestNumber(arr, 3, false);
  testFindKthLargestNumber(arr, 3, true);
  vector<int> arr_1{10, 7, 0, 8, 0, 9, 1, 5};
  testQuickSort(arr_1, false);
  testQuickSort(arr_1, true);
  testFindKthLargestNumber(arr_1, 3, false);
  testFindKthLargestNumber(arr_1, 3, true);
  return 0;
}
