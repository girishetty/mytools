
/*
A Linear chess is a game with a row with n number of steps in that.
The rules of the game is:
1. The Left player(s): can only move right from their current position or not move at all.
2. The Right players(s): can only move left from their current position or not move at all.
3. At any point Left and Right player(s) can jump over another Right or Left player(s)

For example:
Given solve(start, end), the function should return these for following start/end positions:

solve(“L__R”, “__LR”) ==> true
solve(“_LR_“, “L_R_“) ==> false
solve(“L__R“, “_RL_“) ==> false
solve(“_RL_“, “R__L“) ==> true
solve(“_RL_“, “__LR“) ==> false
solve(“_RL_“, "LR__") ==> false;
*/


#include <string>
#include <vector>
#include <iostream>

struct LinearChessGameState {
  std::vector<size_t> mLeftPlayers;
  std::vector<size_t> mRightPlayers;
};

void setState(const std::string& positions, LinearChessGameState& state) {
  state.mLeftPlayers.clear();
  state.mRightPlayers.clear();

  size_t index = 0;
  for (auto& player : positions) {
    if (player == 'L') {
      state.mLeftPlayers.push_back(index);
    } else if (player == 'R') {
      state.mRightPlayers.push_back(index);
    }
    index++;
  }
}

bool solve(const std::string& start, const std::string& finish) {
  // Make sure the size matches.
  if (start.size() != finish.size()) {
    return false;
  }

  // Construct end and begin states
  LinearChessGameState beginState;
  LinearChessGameState endState;
  setState(start, beginState);
  setState(finish, endState);

  auto leftPlayerCount = beginState.mLeftPlayers.size();
  auto rightPlayerCount = beginState.mRightPlayers.size();

  // Make sure the size matches.
  if ((leftPlayerCount != endState.mLeftPlayers.size()) ||
      (rightPlayerCount != endState.mRightPlayers.size())) {
    return false;
  }

  // Make sure the moves by Left Players are valid.
  size_t index = 0;
  for (; index < leftPlayerCount; index++) {
    if (beginState.mLeftPlayers[index] > endState.mLeftPlayers[index]) {
      return false;
    }
  }

  // Make sure the moves by Right Players are valid.
  for (index = 0; index < rightPlayerCount; index++) {
    if (beginState.mRightPlayers[index] < endState.mRightPlayers[index]) {
      return false;
    }
  }

  // Make sure there was no jumping over Left/Right players.
  if (rightPlayerCount > 0 && leftPlayerCount > 0) {
    if ((beginState.mLeftPlayers[leftPlayerCount - 1] < beginState.mRightPlayers[0]) &&
        (endState.mLeftPlayers[leftPlayerCount - 1] > endState.mRightPlayers[0])) {
      return false;
    }
  }
  
  return true;
}

int main() {

  std::cout << "\"L__R\", \"__LR\" ==> " << solve("L__R", "__LR") << std::endl;
  std::cout << "\"_LR_\", \"L_R_\" ==> " << solve("_LR_", "L_R_") << std::endl;
  std::cout << "\"L__R\", \"_RL_\" ==> " << solve("L__R", "_RL_") << std::endl;
  std::cout << "\"_RL_\", \"R__L\" ==> " << solve("_RL_", "R__L") << std::endl;
  std::cout << "\"_RL_\", \"__LR\" ==> " << solve("_RL_", "__LR") << std::endl;
  std::cout << "\"_RL_\", \"LR__\" ==> " << solve("_RL_", "LR__") << std::endl;
  return 0;
}
