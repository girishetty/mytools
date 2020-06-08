/**
 * given a set of numbers, find all possible sub-set from this set that adds up to N
 */

#include <iostream>

using namespace std;

void printCombination(int candidateSet[], int combinationIndex[], int noOfElements) {
  for (int i = 1; i <= noOfElements; i++) {
    cout << candidateSet[combinationIndex[i]] << ((i == noOfElements) ? "" : "+");
  }
  cout <<endl;
}
 
void findCombination(int targetNumber, int currentSum, int candidateSet[], int size, 
           int combinationIndex[], int noOfElements) {
  if (currentSum > targetNumber) {
    //Failed combination
    return;
  }
  if (currentSum == targetNumber) {
    //Successful combination
    printCombination(candidateSet, combinationIndex, noOfElements);
    return;
  }

  for (int i = combinationIndex[noOfElements]; i < size; i++) {
    combinationIndex[noOfElements+1] = i;
    findCombination(targetNumber, currentSum + candidateSet[i], candidateSet, size, 
      combinationIndex, noOfElements+1);
  }
}

void solveCombination(int targetNumber, int candidateSet[], int size) {

  //allocate a possible combination with 2 more than targetNumber
  int* combinationIndex = new int[targetNumber + 2];
  combinationIndex[0] = 0;
  findCombination(targetNumber, 0, candidateSet, size, combinationIndex, 0);
  delete []combinationIndex;
}

void Combination(const char* set, char* combination, int start, int depth, int length) {
  if(start < length) {
    //We got a combination, so print it
    combination[depth] = '\0';
    cout<<combination<<endl;
  }
  for(int index=start; index<length; index++) {
    combination[depth] = set[index];
    Combination(set, combination, index+1, depth+1, length);
  }
}

void CreateCombinations(const char* set) {
  int length = strlen(set) + 1;
  char* combination = new char[length];
  Combination(set, combination, 0, 0, length);
  delete [] combination;
}

int main() {
  int candidateSet[] = {10, 1, 2, 7, 6, 1, 5};
  int targetNumber = 8;
  int sizeOfSet = sizeof(candidateSet)/sizeof(int);
  solveCombination(targetNumber, candidateSet, sizeOfSet);

  //
  char set[] = "ABC";
  CreateCombinations(set);

  return 0;
}
