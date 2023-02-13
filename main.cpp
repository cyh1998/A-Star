#include <iostream>
#include "source/AStar.h"

using namespace std;

int main()
{
    AStar aStar;
    aStar.SetWorldSize({ 9, 9 }); //设置世界大小
    aStar.SetDiagonal(false); //设置取消对角线方向
    std::vector<Vec2> walls = { { 2, 3 }, { 3, 3 }, { 3, 4 }, { 3, 5 },
                                { 3, 6 }, { 4, 6 }, { 5, 1 }, { 5, 2 },
                                { 5, 3 }, { 5, 6 }, { 5, 7 }, { 6, 0 },
                                { 6, 1 }, { 6, 2 }, { 7, 4 }, { 8, 4 } };
    aStar.SetWalls(walls); //设置墙体
    auto path = aStar.FindPath({ 1, 7 }, { 7, 1 }); //寻路
    aStar.ShowMap(); //可视化
    return 0;
}