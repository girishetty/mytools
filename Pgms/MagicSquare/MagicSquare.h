#ifndef MAGICSQUARE_H
#define MAGICSQUARE_H

#include <vector>

class MagicSquare {
enum {
  KMaxSize = 20,
};
public:
  MagicSquare(size_t aSize = 1);

  void Display() const;

  void Create();

  size_t fixMagicSquare(std::vector<std::vector<int>>& square);

private:
  int    iSize;
  int    iSquare[KMaxSize][KMaxSize];
};

#endif //MAGICSQUARE_H
