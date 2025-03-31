#include "PathFinder.h"
#include "Map.h"
#include <algorithm>
#include <cmath>
std::vector<PathFinder::Point> PathFinder::FindPath(int map[20][25]) {
    std::vector<Point> path;
    Point start, end;
    FindPathStartEnd(map, start, end);

    std::cout << "PathFinder Debug:" << std::endl;
    std::cout << "Start Point: (" << start.x << ", " << start.y << ")" << std::endl;
    std::cout << "End Point: (" << end.x << ", " << end.y << ")" << std::endl;

    if (start.x == -1 || end.x == -1) {
        std::cerr << "Error: Start or end point not found!" << std::endl;
        return path;
    }

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

            // Debug print for path finding
            std::cout << "Checking point: (" << newX << ", " << newY << ")" << std::endl;
            std::cout << "Is enemy path: " << mapObj.IsEnemyPath(newY, newX) << std::endl;

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

    // Print out full path
    std::cout << "Generated Path:" << std::endl;
    for (const auto& point : path) {
        std::cout << "(" << point.x << ", " << point.y << ") ";
    }
    std::cout << std::endl;
    return path;
}
void PathFinder::FindPathStartEnd(int map[20][25], Point& start, Point& end){
    start = Point(-1, -1);
    end = Point(-1, -1);
    //Find start point (1st dirt tile)
    for (int y = 0; y < 20; y++) {
        if (map[y][0] == 2) {
            start = Point(0, y);    
            break;
        }
    }
    //Find end point (Last dirt tile)
    for (int y = 0; y < 20; y++) {
        if (map[y][24] == 2) {
            end = Point(24, y);
            break;
        }
    }
}