#pragma once
#include <vector>
class PathFinder {
public:
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Point {
        int x, y;
        Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    };
    static std::vector<Point> FindPath(int map[20][25]);
    static void FindPathStartEnd(int map[20][25], Point& start, Point& end);
};