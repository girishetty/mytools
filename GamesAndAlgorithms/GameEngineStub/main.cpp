/**
 * This code snippet uses raw pointers in its APIs.
 *
 * Retrofit this code to use shared_ptr smart pointer in all of
 * its APIs.
 *
 */

#include <memory>
#include <iostream>

class Game;

class GameEngine {
public:
  void recalculateState (std::shared_ptr<Game> game);
};

class Game : public std::enable_shared_from_this<Game> {
  int pos = 0;

public:
  Game () {}

  void move(std::shared_ptr<GameEngine> eng) {
    pos++;
    eng->recalculateState(shared_from_this());
  }

  int position() const {
    return pos;
  }
};

void GameEngine::recalculateState (std::shared_ptr<Game> game) {
  auto pos = game->position();
  /* do something */
}

int main () {
  auto eng = std::make_shared<GameEngine>();
  auto game = std::make_shared<Game>();

  game->move(eng);

  return 0;
}
