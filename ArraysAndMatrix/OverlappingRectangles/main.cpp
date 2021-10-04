
/*
 * Given two rectangles, find the area of the overlapping region/rectangle.
 * It should return 0 if the rectangles don't overlap
 *
 * Note that a rectangle can be represented by two coordinates, top left and bottom right.
 * So mainly we are given following four coordinates. 
 *  - l1: Top Left coordinate of first rectangle. 
 *  - r1: Bottom Right coordinate of first rectangle. 
 *  - l2: Top Left coordinate of second rectangle. 
 *  - r2: Bottom Right coordinate of second rectangle.
 */

#include <iostream>

struct Point {
  int x;
  int y;

  Point(int a, int b) : x(a), y(b) {}
};

/*
   (x, y) topLeft
       ____________________
      |                   |
      |                   |
      |                   |
      |                   |
      |___________________|
                        (x,y) bottomRight
*/
struct Rectangle {
  Point topLeft;
  Point bottomRight;

  Rectangle(const Point& l, const Point& r) : topLeft(l), bottomRight(r) {}
};

/*
 * Two rectangles are not overlapping if:
 * - one is sitting on top of top edde of others
 * - one is sitting left to left edde of others
 * - And also if one of them is a line - there is nothing overlapping
 */
bool isOverlapping(const Rectangle& r1, const Rectangle& r2) {
  // If either of them is a line
  if (r1.topLeft.x == r1.bottomRight.x ||
      r1.topLeft.y == r1.bottomRight.y ||
      r2.topLeft.x == r2.bottomRight.x ||
      r2.topLeft.y == r2.bottomRight.y) {
    return false;
  }

  //one is sitting on top of top edde of others
  if (r1.bottomRight.y >= r2.topLeft.y || r2.bottomRight.y >= r1.topLeft.y) {
    return false;
  }

  //one is sitting left to left edde of others
  if (r1.topLeft.x >= r2.bottomRight.x || r2.topLeft.x >= r1.bottomRight.x) {
    return false;
  }

  // Else its overlapping.
  return true;
}

/*
 * Get the area of intersection with overlapping
 */
int getOverlappingArea(const Rectangle& r1, const Rectangle& r2) {
  if (!isOverlapping(r1, r2)) {
    return 0;
  }

  int width_of_intersection = std::min(r1.bottomRight.x, r2.bottomRight.x) -
                              std::max(r1.topLeft.x, r2.topLeft.x);
  int height_of_intersection = std::min(r1.topLeft.y, r2.topLeft.y) -
                               std::max(r1.bottomRight.y, r2.bottomRight.y);

  return width_of_intersection * height_of_intersection;
}

int getArea(const Rectangle& r) {
  return std::abs(r.bottomRight.x - r.topLeft.x) * std::abs(r.topLeft.y - r.bottomRight.y);
}

/*
 * Get the area of total of overlapping area.
 */
int getTotalOverlappingArea(const Rectangle& r1, const Rectangle& r2) {
  int area_r1 = getArea(r1);
  int area_r2 = getArea(r2);
  int union_area = getOverlappingArea(r1, r2);
  return area_r1 + area_r2 - union_area;
}

int main() {
    Rectangle r1({1, 5}, {5, 1});
    Rectangle r2({2, 5}, {5, 3});

    std::cout << "Overlapping Union Area: " << getOverlappingArea(r1, r2) << std::endl;
    std::cout << "Total Overlapping Area: " << getTotalOverlappingArea(r1, r2) << std::endl;
}
