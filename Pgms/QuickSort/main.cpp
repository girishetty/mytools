#include <vector>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
  
inline void swap(int& a, int& b) {
  int t = a;
  a = b;
  b = t;
}
  
/**
 * This function takes last element as pivot, places the pivot element
 * at its correct position in sorted array, and places all smaller (smaller than pivot)
 * to left of pivot and all greater elements to right of pivot.
 */
size_t partition (vector<int>& arr, size_t low, size_t high) {
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
  swap(arr[i], arr[high]);  
  return i;  
}

/**
 * The main function that implements QuickSort
 * arr[] --> Array to be sorted,
 * low --> Starting index,
 * high --> Ending index
 */
void quickSort(vector<int>& arr, size_t low, size_t high) {
  if (low < high) {
    // pi is partitioning index, arr[p] is now at right place
    size_t pi = partition(arr, low, high);

    // Separately sort elements before partition and after partition
    if (pi > low) {
      quickSort(arr, low, pi - 1);
    }
    quickSort(arr, pi + 1, high);
  }
}

/* Function to print an array */
void printArray(const string& msg, const vector<int>& arr) {
  cout << msg << ": ";
  for (size_t i = 0; i < arr.size(); i++) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

int main() {
  vector<int> arr{10, 7, 8, 9, 1, 5, 3, 0, 12, 0, 4, 2, 1};
  printArray("Orignal Array:", arr);
  quickSort(arr, 0, arr.size() - 1);
  printArray("Sorted  Array:", arr);
  return 0;
}
