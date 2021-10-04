#include <vector>
#include <string>
#include <iostream>

inline void removeLastChar(char c, std::string& pattern) {
  if (!pattern.empty() && pattern[pattern.size()-1] == c) {
    pattern = pattern.substr(0, pattern.size()-1);
  }
}

void displayVector(const char* name, const std::vector<std::vector<size_t> >& scorePatterns) {

  size_t currentScore = 0;
  for (const auto& scorePattern : scorePatterns) {
    std::cout << name << "Pattern For Score: [" << currentScore << "]: ";
    if (scorePattern.empty()) {
      std::cout << "[]" << std::endl;
    } else {
      std::cout << "[ ";
      size_t score = 0;
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

void appendPatterns(const std::vector<size_t>& lowerScorePattern, size_t prevScore,
                    std::vector<size_t>& higherScorePattern, size_t addValue) {

  size_t score = 0;
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
 */
size_t findPossibilities(size_t totalScore, const std::vector<size_t>& values) {
  std::vector<size_t> scoreCounts(totalScore + 1, 0);
  std::vector<std::vector<size_t> > scorePattern(totalScore + 1);

  // Possibility of not scoring anything is 1.
  scoreCounts[0] = 1;

  for (size_t i = 1; i <= totalScore; i++) {
    for (size_t value : values) {
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

  displayVector("Pattern", scorePattern);
  return scoreCounts[totalScore];
}

/**
 * Given a set of scores in which someone (or a team) can score, for any given
 * expected final score, find out all possible ways for scoring that final score.
 * This method uses recursive implementation.
 */
size_t findPossibilitiesRecursive(int totalScore, const std::vector<size_t>& values) {
  if (totalScore == 0) {
    return 1;
  } else if (totalScore < 0) {
    return 0;
  } else {
    size_t possibilities = 0;
    for (auto value : values) {
      possibilities += findPossibilitiesRecursive(totalScore - value, values);
    }
    return possibilities;
  }
}

/**
 * Given a set of scores in which someone (or a team) can score, for any given
 * expected final score, find out all unique possible ways for scoring that final score.
 */
size_t findUniquePossibilities(size_t totalScore, const std::vector<size_t>& values) {
  std::vector<size_t> scoreCounts(totalScore + 1, 0);
  scoreCounts[0] = 1;

  for (auto value : values) {
    for (size_t j = value; j <= totalScore; j++) {
      scoreCounts[j] += scoreCounts[j-value];
    }
  }

  return scoreCounts[totalScore];
}

int main() {
  std::vector<size_t> points {2, 3, 6, 7}; // Ways of scoring points.
  size_t finalScore = 6;
  std::cout << "Enter a Desirable Score: ";
  std::cin >> finalScore;

  size_t way1 = findPossibilities(finalScore, points);
  size_t way2 = findUniquePossibilities(finalScore, points);
  size_t way3 = findPossibilitiesRecursive(finalScore, points);
  std::cout << "Score " << finalScore << " Can be scored in " << way1 << " : " << way3
            << " different ways and " << way2 << " unique ways" << std::endl;

  return 0;
}
