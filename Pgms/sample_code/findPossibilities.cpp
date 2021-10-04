#include <vectro>

/**
 * Given a set of scores in which someone (or a team) can score, for any given
 * expected final score, find out all possible ways for scoring that final score.
 */
size_t findPossibilities(size_t totalScore, const std::vector<size_t>& values) {
  std::vector<size_t> allScores(totalScore + 1, 0);

  // Possibility of not scoring anything is 1.
  allScores[0] = 1;

  // Assuming the values are in sorted order
  for (auto score = values[0]; score <= totalScore; score++) {
    for (auto value : values) {
      if (value <= score) {
        allScores[score] += allScores[score - value];
      }
    }
  }

  return allScores[totalScore];
}
