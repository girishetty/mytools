/*
 You are spectating a programming contest with N competitors, each trying to independently solve
 the same set of programming problems. Each problem has a point value, which is either 1 or 2.

 On the scoreboard, you observe that the i ith competitor has attained a score of S[i],
 which is a positive integer equal to the sum of the point values of all the problems they have solved.

 The scoreboard does not display the number of problems in the contest, nor their point values.
 Using the information available, you would like to determine the minimum possible number of problems in the contest.

 Constraints:
     1 ≤ N ≤ 500,000
     1 ≤ Si ≤ 1,000,000,000

 Sample Test Case #1
    N = 6
    S = [1, 2, 3, 4, 5, 6]
    Expected Return Value = 4

 Sample Test Case #2
    N = 4
    S = [4, 3, 3, 4]
    Expected Return Value = 3
 Sample Test Case #3
    N = 4
    S = [2, 4, 6, 8]
    Expected Return Value = 4

 Sample Explanation:
    In the first case, it's possible that there are as few as 4 problems in the contest,
    for example with point values [1, 1, 2, 2]. The 6 competitors could have solved the following subsets of problems:
      - 1. Problem 1 (1 point)
      - 2. Problem 3 (2 points)
      - 3. Problems 2 and 3 (1 + 2 = 3 points)
      - 4. Problems 1, 2, and 4 (1 + 1 + 2 = 4 points)
      - 5. Problems 2, 3 3, and 4 (1 + 2 + 2 = 5 points)
      - 6.  All 4 problems (1 + 1 + 2 + 2 = 6 points)

    It is impossible for all 6 competitors to have achieved their scores if there are fewer than 4 problems.

    In the second case, one optimal set of point values is [1, 1, 2]

    In the third case, one optimal set of point values is [2, 2, 2, 2]
 */

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#ifdef DEBUG
void displayVector(const char* name, const std::vector<int>& array) {
  std::cout << name << ": [";
  for (const auto& value : array) {
    std::cout << value << " ";
  }
  std::cout << "]" << std::endl;
}

void displayVector(const char* name, const std::vector<std::vector<int> >& scorePatterns) {

  int currentScore = 0;
  for (const auto& scorePattern : scorePatterns) {
    std::cout << name << "Pattern For Score: [" << currentScore << "]: ";
    if (scorePattern.empty()) {
      std::cout << "[]" << std::endl;
    } else {
      std::cout << "[ ";
      int score = 0;
      for (const auto& value : scorePattern) {
        score += value;
        if (score == currentScore) {
          score = 0;
          std::cout << value << " ";
        } else {
          std::cout << value << "-";
        }
      }
      std::cout << "]" << std::endl;
    }
    currentScore++;
  }
  std::cout << std::endl;
}
#endif

void appendPatterns(const std::vector<int>& lowerScorePattern, int prevScore,
                    std::vector<int>& higherScorePattern, int addValue) {

  int score = 0;
  for (const auto& value : lowerScorePattern) {
    higherScorePattern.push_back(value);
    score += value;
    if (score == prevScore) {
      higherScorePattern.push_back(addValue);
      score = 0;
    }
  }
}

/**
 * Given a set of scores in which someone (or a team) can score, for any given
 * expected final score, find out all possible ways for scoring that final score.
 * This returns all the patterns of possible ways to score.
 */
std::vector<std::vector<int>> findPossibilities(int totalScore, const std::vector<int>& values) {
  std::vector<int> scoreCounts(totalScore + 1, 0);
  std::vector<std::vector<int> > scorePattern(totalScore + 1);

  // Possibility of not scoring anything is 1.
  scoreCounts[0] = 1;

  for (int i = 1; i <= totalScore; i++) {
    for (int value : values) {
      // Check if the target score is less than one of the values from the given set of scores.
      if (value <= i) {
        scoreCounts[i] += scoreCounts[i-value];
        if (i == value) {
          scorePattern[i].push_back(value);
        } else {
          if (!scorePattern[i - value].empty()) {
            appendPatterns(scorePattern[i - value], i - value, scorePattern[i], value);
          }
        }
      } else {
        break;
      }
    }
  }

  //displayVector("Pattern", scorePattern);
  return scorePattern;
}

/**
 * Given a set of scores in which someone (or a team) can score, for any given
 * expected final score, find out all possible ways for scoring that final score.
 * This returns count of all the possible ways to score.
 */
std::vector<int> findPossibileWays(int totalScore, const std::vector<int>& values) {
  std::vector<int> scoreCounts(totalScore + 1, 0);

  // Possibility of not scoring anything is 1.
  scoreCounts[0] = 1;

  for (int i = 1; i <= totalScore; i++) {
    for (int value : values) {
      // Check if the target score is less than one of the values from the given set of scores.
      if (value <= i) {
        scoreCounts[i] += scoreCounts[i-value];
      } else {
        break;
      }
    }
  }

  return scoreCounts;
}

int getMinProblemCount(std::vector<int>& scores) {
  std::vector<int> points {1, 2}; // Ways of scoring points.

  // Sort all the scores to find the highest score first.
  std::sort(scores.begin(), scores.end());
  int highestScore = scores[scores.size() - 1];

  // Find all the possible patterns to score the highest score
  auto scorePatterns = findPossibilities(highestScore, points);

  // This is where we have all the patterns of scoring highestScore (in a vector)
  int index = scorePatterns.size() - 1;

  // Now start from the last possible pattern (which is the least one)
  // And see if we can use this pattern to score all other scores in scores[]
  for (int lastIndex = scorePatterns[index].size() - 1; lastIndex >= 0;) {

    // Since all the possible patterns are in one vector, we need to seperate each of them
    std::vector<int> onePattern;
    int score = 0;
    do {
      score += scorePatterns[index][lastIndex];
      onePattern.push_back(scorePatterns[index][lastIndex--]);
      if (score == highestScore) {
        // That was end of one pattern.
        break;
      }
    } while (lastIndex >= 0);

    // Sort the pattern - makes it easy
    std::sort(onePattern.begin(), onePattern.end());

    // Get the list of Possible Ways of scoring 0 to highestScore using this pattern of points
    auto scoreCounts = findPossibileWays(highestScore, onePattern);

#ifdef DEBUG
    displayVector("onePattern", onePattern);
    displayVector("scoreCounts", scoreCounts);
#endif

    // Check if all the given scores can be achieved with this pattern of points
    bool good = true;
    for (auto score : scores) {
      if (scoreCounts[score] <= 1) {
        good = false;
        break;
      }
    }

    if (good) {
      // That is our minimal problem count
      return onePattern.size();
    }
  }

  return 0;
}

void testMinProblemCount(std::vector<int> scores) {
  auto count = getMinProblemCount(scores);
  std::cout << "Minimum Problem Count is: " << count << std::endl;
}

int main() {
  testMinProblemCount({1, 2, 3, 4, 5, 6});
  testMinProblemCount({4, 3, 3, 4});
  testMinProblemCount({2, 4, 6, 8});

  return 0;
}
