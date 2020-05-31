#include "BinarySearch.h"

/*
 * Loop based Binary search algorithm
 * Returns the index of the element of interest or -1 if its not found
 */

int BinarySearch(int array[], int length, int num) {
  int left = 0;
  int right = length;
  int mid = 0;
  int index = -1;
  
  while(left <= right) {
    /* get the index of middle element */ 
    mid = (left + right) >> 1;
    if(array[mid] < num) {
      left = mid + 1;
    } else if(array[mid] > num) {
      right = mid - 1;
    } else {
      /* we have found the element */
      index = mid;
      break;
    }
  }

  return index;
}

/*
 * Recursive based Binary search algorithm
 * Returns the index of the element of interest or -1 if its not found
 */
int RecursiveBinarySearch(int array[], int left, int right, int num) {
  int mid = 0;
  int index = -1;
  
  if(left <= right) {
    /* get the index of middle element */ 
    mid = (left + right) >> 1;
    if(array[mid] < num) {
      index = RecursiveBinarySearch(array, mid + 1, right, num);
    } else if(array[mid] > num) {
      index = RecursiveBinarySearch(array, left, mid - 1, num);
    } else {
      /* we have found the element */
      index = mid;
    }
  }
  return index;
}
