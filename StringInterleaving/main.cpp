/**
 * https://www.youtube.com/watch?v=ih2OZ9-M3OM
 * Interleaving: give 2 string (A, B), the third string (C) is entirely made up of A and B preserving the order
 *
 * A ==> a a b
 * B ==> a x y
 * C ==> a a x a b y ==> TRUE
 * C ==> a b a a x y ==> FALSE
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

void Display2DVector(const vector<vector<bool>>& matrix) {
  for (auto rows : matrix) {
    for (auto item : rows) {
      cout << setw(4) << item << "    ";
    }
    cout << endl;
  }

  cout << "=================================" << endl;
}

string IsInterleaved(const string& A, const string& B, const string& C) {
  if (C.size() < A.size() + B.size()) {
    return "NO";
  }
  size_t sizeA = A.size() + 1;
  size_t sizeB = B.size() + 1;
  // set up the dp with all false.
  vector<vector<bool>> dp(sizeA, vector<bool>(sizeB, false));

  // case#1: when both are empty.
  dp[0][0] = true;

  // case#2: when B is empty.
  for (size_t row = 1; row < sizeA; row++) {
    auto aIndex = row - 1;
    if (dp[aIndex][0] && (A[aIndex] == C[aIndex])) {
      dp[row][0] = true;
    }
  }

  // case#3: when A is empty.
  for (size_t col = 1; col < sizeB; col++) {
    auto bIndex = col - 1;
    if (dp[0][bIndex] && (B[bIndex] == C[bIndex])) {
      dp[0][col] = true;
    }
  }

  // case#4: When neither is empty
  for (size_t row = 1; row < sizeA; row++) {
    auto aIndex = row - 1;
    for (size_t col = 1; col < sizeB; col++) {
      auto bIndex = col - 1;
      auto cIndex = aIndex + bIndex + 1;
      if ((dp[aIndex][col] && A[aIndex] == C[cIndex]) ||
          (dp[row][bIndex] && B[bIndex] == C[cIndex])) {
        dp[row][col] = true;
      }
    }
  }

  return dp[sizeA - 1][sizeB - 1] ? "YES" : "NO";
}

int main() {
  string A("aab");
  string B("axy");
  string C("aaxaby");
  //cin >> A;
  //cin >> B;
  //cin >> C;

  cout << "A: " << A << "  B: " << B << "  C: " << C << "  Interleaved? " << IsInterleaved(A, B, C) << endl;
  C = "abaaxy";
  cout << "A: " << A << "  B: " << B << "  C: " << C << "  Interleaved? " << IsInterleaved(A, B, C) << endl;

  return 0;
}
