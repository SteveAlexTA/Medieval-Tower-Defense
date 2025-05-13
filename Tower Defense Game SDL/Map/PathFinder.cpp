#include "PathFinder.h"
#include "Map.h"
#include <algorithm>
#include <cmath>

std::vector<PathFinder::Point> PathFinder::FindPath(int map[20][25]) {
    std::vector<Point> path;
    Point start, end;
    FindPathStartEnd(map, start, end);
    Point curr = start;
    path.push_back(curr);

    const int dx[4] = { 0,0,-1,1 };
    const int dy[4] = { -1,1,0,0 };

    Map mapObj;

    while (!(curr.x == end.x && curr.y == end.y))
    {
        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int newX = curr.x + dx[i];
            int newY = curr.y + dy[i];
            if (mapObj.IsEnemyPath(newY, newX)) {
                if (path.size() > 1 && newX == path[path.size() - 2].x && newY == path[path.size() - 2].y) {
                    continue;
                }
                curr = Point(newX, newY);
                path.push_back(curr);
                moved = true;
                break;
            }
        }
        if (!moved) break;
    }
    return path;
}

void PathFinder::FindPathStartEnd(int map[20][25], Point& start, Point& end) {
    start = Point(-1, -1);
    end = Point(-1, -1);
    //Find start point (Spawning area)
    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 20; y++) {
            if (map[y][x] == 2) {
                // Find the leftmost path tile
                if (x == 0 || map[y][x - 1] != 2) {
                    start = Point(x, y);
                    break;
                }
            }
        }
        if (start.x != -1) break;
    }
    // Find end point (rightmost path tile)
    for (int x = 24; x >= 0; x--) {
        for (int y = 0; y < 20; y++) {
            if (map[y][x] == 2) {
                // Find the rightmost path tile
                if (x == 24 || map[y][x + 1] != 2) {
                    end = Point(x, y);
                    break;
                }
            }
        }
        if (end.x != -1) break;
    }
}