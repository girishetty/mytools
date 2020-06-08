#include <iostream>
using namespace std;

void Merge(int array[], int tempArray[], int first, int second, int end) {
  int firstLen = second;
  int index = first;
  while (first < firstLen && second < end) {
    if (tempArray[first] <= tempArray[second]) {
      array[index++] = tempArray[first++];
    } else {
      array[index++] = tempArray[second++];
    }
  }
  //Copy remaining entries of first sorted list
  while (first < firstLen) {
    array[index++] = tempArray[first++];
  }
  //Copy remaining entries of second sorted list
  while (second < end) {
    array[index++] = tempArray[second++];
  }
}

void MergeSort(int array[], int begin, int end, int tempArray[]) {
  if (begin < end - 1) {
    int mid = (begin + end) >> 1;
    int* tempArray = new int[length];
    for (int i=0; i<length; i++) {
      tempArray[i] = my_array[i];
    }

    MergeSort(array, begin, mid);
    MergeSort(array, mid, end, tempArray);
    Merge(array, tempArray, begin, mid, end);
  }
}

void Display(int array[], int length) {
  cout<<"Array Contents :";
  for (int i = 0; i<length; i++) {
    cout<<" "<<array[i];
  }
  cout<<endl;
}

int my_array[] = { 4, 1, 9, 3}; //, 8, 2, 7, 6 };

int main() {
  int length = 4;

  Display(my_array, length);
  MergeSort(my_array, 0, length);
  Display(my_array, length);

  return 0;
}
