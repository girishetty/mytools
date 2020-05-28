#include <iostream>
#include <vector>

void LexicographicPermutation(std::vector<int>& permutation, int count = 0) {
    count++;
    for (auto val : permutation) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    // Step#1:
    // Find the largest x such that P[x]<P[x+1].
    // (If there is no such x, P is the last permutation.)
    size_t largestI = -1;
    for (size_t index = 0; index < permutation.size() - 1; index++) {
        if (permutation[index] < permutation[index + 1]) {
            largestI = index;
        }
    }
    if (largestI == -1) {
        std::cout << "Total of " << count << " Permutations" << std::endl;
        return;
    }

    // Step#2:
    // Find the largest y such that P[x]<P[y].
    // Since we know that P[x+1] > P[x], we can start from there.
    size_t largestJ = -1;
    for (size_t index = largestI + 1; index < permutation.size(); index++) {
        if (permutation[largestI] < permutation[index]) {
            largestJ = index;
        }
    }

    // Step#3:
    // Swap P[x] and P[y]
    auto temp = permutation[largestI];
    permutation[largestI] = permutation[largestJ];
    permutation[largestJ] = temp;

    // Step#4:
    // Reverse P[x+1 .. n]
    auto reverseStart = permutation.begin() + largestI + 1;
    std::reverse(reverseStart, std::end(permutation));
    LexicographicPermutation(permutation, count);
}

void LexicographicOrdering(std::vector<int> array) {
    for (auto val : array) {
        std::cout << val << " ";
    }
    std::cout << std::endl << "======================" << std::endl;
    // Sort the array and then print all the permutations
    std::sort(array.begin(), array.end());
    LexicographicPermutation(array);
}

void getVector(std::vector<int>& array, size_t n) {
    array.clear();
    for (size_t i = 0; i < n; i++) {
        array.push_back(i);
    }
}

int main(int argc, char* argv[]) {
    std::vector<int> array {4, 5, 2, 3, 1, 0};
    if (argc == 2) {
        getVector(array, std::stoi(argv[1]));
    }
    LexicographicOrdering(array);
    return 0;
}
