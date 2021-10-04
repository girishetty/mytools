/*
 A photography set consists of N cells in a row, numbered from 1 to N in order, and can be represented by a string C of length N.
 Each cell i is one of the following types (indicated by C[i], the ith character of C):
   - If C[i] = “P”, it is allowed to contain a photographer
   - If C[i] = “A”, it is allowed to contain an actor
   - If C[i] = “B”, it is allowed to contain a backdrop
   - If C[i] = “.”, it must be left empty

 A photograph consists of a photographer, an actor, and a backdrop, such that each of them is placed in a valid cell,
 and such that the actor is between the photographer and the backdrop.
 Such a photograph is considered artistic if the distance between the photographer and the actor is
 between X and Y cells (inclusive), and the distance between the actor and the backdrop is also between X and Y cells (inclusive).
 The distance between cells i and j is ∣i−j∣ (the absolute value of the difference between their indices).

 Determine the number of different artistic photographs which could potentially be taken at the set.
 Two photographs are considered different if they involve a different photographer cell, actor cell, and/or backdrop cell.

 Constraints:
    1 ≤ N ≤ 200
    1 ≤ X ≤ Y ≤ N

 Sample Test Case #1:
    N = 5
    C = APABA
    X = 1
    Y = 2
    Expected Return Value = 1
 Sample Test Case #2
    N = 5
    C = APABA
    X = 2
    Y = 3
    Expected Return Value = 0
 Sample Test Case #3
    N = 8
    C = .PBAAP.B
    X = 1
    Y = 3
    Expected Return Value = 3

 Sample Explanation:
    In the first case, the absolute distances between photographer/actor and actor/backdrop must be between 1 and 2.
    The only possible photograph that can be taken is with the 3 middle cells, and it happens to be artistic.

    In the second case, the only possible photograph is again taken with the 3 middle cells.
    However, as the distance requirement is between 2 and 3, it is not possible to take an artistic photograph.

    In the third case, there are 4 possible photographs, illustrated as follows:
        .P.A...B
        .P..A..B
        ..BA.P..
        ..B.AP..
    All are artistic except the first, where the artist and backdrop exceed the maximum distance of 3
 */

#include <bits/stdc++.h>
using namespace std;

void displayPattern(const string& C, size_t firstIndex, size_t secondIndex, size_t thirdIndex) {
  for (size_t index = 0; index < C.size(); index++) {
    if (index == firstIndex || index == secondIndex || index == thirdIndex) {
      std::cout << C[index];
    } else {
      std::cout << ".";
    }
  }
  std::cout << std::endl;
}

#define USE_MAP

#ifdef USE_MAP
size_t getArtisticPhotographCount(const string& C, int X, int Y) {
  size_t count = 0;
  size_t N = C.size();
  
  std::cout << "String: " << C << std::endl;
  std::map<size_t, size_t> pMap;
  std::map<size_t, size_t> aMap;
  std::map<size_t, size_t> bMap;

  for (size_t index = 0; index < N; index++) {
    switch (C[index]) {
    case 'P':
      pMap[index] = index;
      break;
    case 'A':
      aMap[index] = index;
      break;
    case 'B':
      bMap[index] = index;
      break;
    }
  }

  // Start looking forward
  for (auto pIndex = pMap.begin(); pIndex != pMap.end(); ++pIndex) {
    auto startA = pIndex->second + X;
    auto endA = pIndex->second + Y;
    auto aIndex = aMap.lower_bound(startA);

    while (aIndex != aMap.end() && aIndex->second >= startA && aIndex->second <= endA) {
      auto startB = aIndex->second + X;
      auto endB = aIndex->second + Y;
      auto bIndex = bMap.lower_bound(startB);

      while (bIndex != bMap.end() && bIndex->second >= startB && bIndex->second <= endB) {
        count++;
        startB++;
        bIndex++;
      }

      startA++;
      aIndex++;
    }
  }

#ifdef ENABLE_BIDIRECTION
  // Start looking backward
  for (auto pIndex = pMap.rbegin(); pIndex != pMap.rend(); ++pIndex) {
    //std::cout << "pIndex: " << pIndex->first << ":" << pIndex->second << std::endl;
    auto startA = pIndex->second - X;
    auto endA = pIndex->second - Y;
    auto aIndex = aMap.lower_bound(endA);

    while (aIndex != aMap.end() && aIndex->second <= startA && aIndex->second >= endA) {
      auto startB = aIndex->second - X;
      auto endB = aIndex->second - Y;
      auto bIndex = bMap.lower_bound(endB);

      while (bIndex != bMap.end() && bIndex->second <= startB && bIndex->second >= endB) {
        count++;
        endB++;
        bIndex++;
      }

      endA++;
      aIndex++;
    }
  }
#endif

  return count;
}

int getArtisticPhotographCount(int N, string C, int X, int Y) {
  size_t count = getArtisticPhotographCount(C, X, Y);
#ifndef ENABLE_BIDIRECTION
  std::reverse(C.begin(), C.end());
  count += getArtisticPhotographCount(C, X, Y);
#endif
  return count;
}
#else
size_t getArtisticPhotographCount(const string& C, int X, int Y) {
  size_t count = 0;
  
  std::cout << "String: " << C << std::endl;
  for (size_t firstIndex = 0; firstIndex < C.size() - 2 * X; firstIndex++) {
    std::cout << "firstIndex: " << firstIndex << std::endl;
    if (C[firstIndex] == 'P') {
      for (size_t secondIndex = firstIndex + X; (secondIndex - firstIndex <= Y) && secondIndex < C.size() - X; secondIndex++) {
        std::cout << "    secondIndex: " << secondIndex << std::endl;
        if (C[secondIndex] == 'A') {
          for (size_t thirdIndex = secondIndex + X; (thirdIndex - secondIndex <= Y) && thirdIndex < C.size(); thirdIndex++) {
            std::cout << "        thirdIndex: " << thirdIndex << std::endl;
            if (C[thirdIndex] == 'B') {
              displayPattern(C, firstIndex, secondIndex, thirdIndex);
              count++;
            }
          }
        }
      }
    }
  }
  return count;
}

int getArtisticPhotographCount(int N, string C, int X, int Y) {
  size_t count = getArtisticPhotographCount(C, X, Y);
  std::reverse(C.begin(), C.end());
  count += getArtisticPhotographCount(C, X, Y);
  return count;
}
#endif

int main() {
  std::cout << getArtisticPhotographCount(8, ".PBAAP.B", 1, 3) << std::endl;
  std::cout << getArtisticPhotographCount(5, "APABA", 1, 2) << std::endl;
  std::cout << getArtisticPhotographCount(5, "APABA", 2, 3) << std::endl;

  return 0;
}

