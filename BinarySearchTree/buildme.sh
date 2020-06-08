rm -f a.out

SOURCE="BST.cpp BinarySearch.cpp HeapSort.cpp BinaryMaxHeap.cpp BSTNode.cpp Main.cpp BinaryMinHeap.cpp BinaryHeap.cpp Display.cpp"

cc $SOURCE -std=c++14 -lstdc++ -Wall -o a.out
