/**
 * given a set of numbers, find all possible sub-set from this set that adds up to N
 */

#include <vector>
#include <string>
#include <iostream>

using namespace std;

void printCombination(const vector<int>& candidateSet, const vector<int>& combinationIndex, int noOfElements) {
  for (int i = 1; i <= noOfElements; i++) {
    cout << candidateSet[combinationIndex[i]] << ((i == noOfElements) ? "" : "+");
  }
  cout <<endl;
}
 
void findCombination(int targetNumber, int currentSum, const vector<int>& candidateSet,
           vector<int>& combinationIndex, int noOfElements) {
  if (currentSum > targetNumber) {
    //Failed combination
    return;
  }
  if (currentSum == targetNumber) {
    //Successful combination
    printCombination(candidateSet, combinationIndex, noOfElements);
    return;
  }

  for (size_t i = combinationIndex[noOfElements]; i < candidateSet.size(); i++) {
    combinationIndex[noOfElements+1] = i;
    findCombination(targetNumber, currentSum + candidateSet[i], candidateSet,
      combinationIndex, noOfElements+1);
  }
}

void solveCombination(int targetNumber, const vector<int>& candidateSet) {

  //allocate a possible combination with 2 more than targetNumber
  vector<int> combinationIndex(targetNumber + 2, 0);
  findCombination(targetNumber, 0, candidateSet, combinationIndex, 0);
}

void Combination(const string& set, string& combination, size_t start, size_t depth) {
  if (start < combination.size()) {
    //We got a combination, so print it
    combination[depth] = '\0';
    cout << combination << endl;
  }
  for (size_t index = start; index < combination.size(); index++) {
    combination[depth] = set[index];
    Combination(set, combination, index + 1, depth + 1);
  }
}

/*
 * Create all possible combination for a given set (of any size)
 */
void CreateCombinations(const string& set) {
  string combination(set.size() + 1, '\0');
  Combination(set, combination, 0, 0);
}

void displayCombination(const string& combination) {
  cout << "[ ";
  for (auto item : combination) {
    cout << item << " ";
  }
  cout << "]" << endl;
}
 
void CreateCombinationsOfSize(const string& set, size_t combSize, size_t offset, string& combination) {
  if (combSize == 0) {
    // we have found a combination of desired size
    displayCombination(combination);
    return;
  }

  for (size_t i = offset; i <= set.size() - combSize; i++) {
    // Add this element
    combination.push_back(set[i]);
    // Create combination with rest of the element of size one less
    CreateCombinationsOfSize(set, combSize - 1, i + 1, combination);
    // Remove this element and try again with the next element
    combination.pop_back();
  }
}

/*
 * Create all possible combination for a given set (of any size)
 */
void CreateCombinationsOfSize(const string& set, size_t size) {
  string combination;
  CreateCombinationsOfSize(set, size, 0, combination);
}

int main() {
  vector<int> candidateSet{10, 1, 2, 7, 6, 1, 5};
  int targetNumber = 8;
  solveCombination(targetNumber, candidateSet);

  string set("ABC");
  CreateCombinations(set);

  string input("ABCDEF");
  CreateCombinationsOfSize(input, 3);

  return 0;
}
