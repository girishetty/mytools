/*
 * Problem:
 * Implement quicksort()
 * Solution:
 * Select a pivot element and re-arrange the array so that all the elements to the left of pivot are lesser than or equal to pivot
 * and all the elements to the right of pivot are greater than or equal to pivot
 * sort these two partitions again, until whole array is sorted
 */

void QuickSort(int array[], int beginIndex, int endIndex) {
	int rightPart = beginIndex;
	int leftPart = endIndex;
	int temp;	
	/* make the middle element as pivot */ 
	int pivot = array[(beginIndex + endIndex) >> 1];

	/* make two partition based on pivot */
	while (rightPart <= leftPart) {
		while (array[rightPart] < pivot) {
			rightPart++;
		}
		while (array[leftPart] > pivot) {
			leftPart--;
		}
		/* time to swap the elements */
		if (rightPart <= leftPart) {
			temp = array[rightPart];
			array[rightPart] = array[leftPart];
			array[leftPart] = temp;
			rightPart++;
			leftPart--;
		}
	};

	/* Recursively call sort on both the partitions */
	if (left < leftPart) {
		QuickSort(array, left, leftPart);
	}
	if (rightPart < right) {
		QuickSort(array, rightPart, right);
	}
}
