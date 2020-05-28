/*
 * Problem:
 * 9.  Given the function: "bool numExists( int array[], int array_len, int num )" where "array" is a sorted array of integers.  
 * Determine if "num" exists in the array.  This is basically a binary search question.
 * Do a recursive version and an iterative version.  Writing binary search is easy but getting it the most efficient is hard.  
 * Can you do it with the minimum number of operations possible?
 * Solution:
 * Implement recursive and non-recursive algorithm for BinarySearch and use that within numExists
 */

bool numExists(int array[], int array_len, int num) {
	bool isExists = true;
	if(-1 == BinarySearch(array, length, num)) {
		isExists = false;
	}
	/*
	To use recursive algo, use this code
	if(-1 == RecursiveBinarySearch(array, 0, length, num)) {
		isExists = false;
	}
	*/
	return isExists;
}

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
		}
		else if(array[mid] > num) {
			right = mid - 1;
		}
		else {
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
		}
		else if(array[mid] > num) {
			index = RecursiveBinarySearch(array, left, mid - 1, num);
		}
		else {
			/* we have found the element */
			index = mid;
		}
	}
	return index;
}
