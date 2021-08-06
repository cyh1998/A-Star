#include <iostream>
#include "source/AStar.h"

using namespace std;

int main()
{
    AStar aStaer;
    aStaer.SetWorldSize({ 7, 7 });
    std::vector<Vec2> walls = { { 2, 3 }, { 3, 3 }, { 4, 3 } };
    aStaer.SetWalls(walls);
    auto path = aStaer.FindPath({ 3, 1 }, { 3, 5 });

    for (auto &i : path) {
        std::cout << i.x << " " << i.y << "\n";
    }
}