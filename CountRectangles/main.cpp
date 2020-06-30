/**
 * https://www.mYoutube.com/watch?v=EuPSibuIKIg
 * Given a set of points (x, y), find no of rectangles that can be formed
 * with those points.
 */

#include <map>
#include <vector>
#include <iostream>

using std::map;
using std::endl;
using std::cout;
using std::pair;
using std::vector;

struct Point {
  Point(int x, int y) : mX(x), mY(y) {}
  Point(const Point& p) : mX(p.mX), mY(p.mY) {}

  int mX;
  int mY;
};

/**
 * Logic is to find pair of 4 points (x1, y1), (x1, y2), (x2, y1), (x2, y2)
 * such that x1 < x2 and y1 < y2
 * 
 */
size_t countNoOfRectangles(const vector<Point>& points) {
  map<pair<int, int>, size_t> vericleLineCount;
  size_t rectangleCount = 0;

  for (const auto& point : points) {
    // For each point (x1, y1), check is there a verticle line starting from
    // this point. i.e find a point (x2, y2) such that x1 = x2 and y2 > y1.
    for (const auto& point_above : points) {
      if (point.mX == point_above.mX && point.mY < point_above.mY) {
        // Save the y-coordinates of this verticle line in a map.
        pair<int, int> yPair{point.mY, point_above.mY};
        rectangleCount += vericleLineCount[yPair];
        // Increment the occurances of such/similar verticle lines.
        vericleLineCount[yPair]++;
      }
    }
  }
  return rectangleCount;
}

int main() {
  
  vector<Point> points = { {1, 1}, {1, 3}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
                           {3, 1}, {3, 3}, {3, 5}, {4, 3}, {5, 4}, {6, 1}, {6, 5} };
  auto count = countNoOfRectangles(points);

  cout << "No of Rectangles: " << count << endl;
}
