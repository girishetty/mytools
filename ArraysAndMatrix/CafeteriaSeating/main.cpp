/*
  A cafeteria table consists of a row of N.  N seats, numbered from 1 to N
  N from left to right. Social distancing guidelines require that every diner be seated such that:
   * K seats their left and K
   * K seats to their right (or all the remaining seats to that side if there are fewer than K) remain empty.

  There are currently M diners seated at the table, the ith of whom is in seat S[i]

  No two diners are sitting in the same seat, and the social distancing guidelines are satisfied.

  Determine the maximum number of additional diners who can potentially sit at the table without
  social distancing guidelines being violated for any new or existing diners, assuming that the
  existing diners cannot move and that the additional diners will cooperate to maximize how many of them can sit down.

  Please take care to write a solution which runs within the time limit.

  Constraints:
   * 1 <= N <= 10^15
   * 1 ≤ K ≤ N
   * 1 ≤ M ≤ 500,000
   * 1 ≤ Si ≤ N

  Sample Test Case #1
    N = 10
    K = 1
    M = 2
    S = [2, 6]
    Expected Return Value = 3

Sample Test Case #2
    N = 15
    K = 2
    M = 3
    S = [11, 6, 14]
    Expected Return Value = 1
 */

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

long long getMaxAdditionalDinersCount(long long N, long long K, int M, vector<long long> S) {
  // Write your code here
  long long moreSeating = 0;
  long long gap = K + 1;
  long long begin_range = 1;
  long long free_seats = 0;
  
  // Sort the seating
  std::sort(S.begin(), S.end());
  S.push_back(N+gap);
 
  for (auto next_seating : S) {
    free_seats = next_seating - gap - begin_range;
    //std::cout << "moreSeating: " << moreSeating << ":begin_range: " << begin_range << ":next_seating: " << next_seating << ":free_seats: " << free_seats << std::endl;
    if (free_seats >= 0) {
      moreSeating += std::floor(free_seats / gap) + 1;
    }
    begin_range = next_seating + gap;
  }

  return moreSeating;
}

int main() {
  std::cout << "No. of seats to accomodate: " << getMaxAdditionalDinersCount(10, 1, 2, {2, 6}) << std::endl;
  std::cout << "No. of seats to accomodate: " << getMaxAdditionalDinersCount(15, 2, 3, {11, 6, 14}) << std::endl;

  return 0;
}
