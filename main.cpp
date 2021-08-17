#include <iostream>
#include "source/AStar.h"

using namespace std;

int main()
{
    AStar aStar;
    aStar.SetWorldSize({ 9, 9 }); //���������С
    aStar.SetDiagonal(false); //����ȡ���Խ��߷���
    std::vector<Vec2> walls = { { 2, 3 }, { 3, 3 }, { 3, 4 }, { 3, 5 },
                                { 3, 6 }, { 4, 6 }, { 5, 1 }, { 5, 2 },
                                { 5, 3 }, { 5, 6 }, { 5, 7 }, { 6, 0 },
                                { 6, 1 }, { 6, 2 }, { 7, 4 }, { 8, 4 } };
    aStar.SetWalls(walls); //����ǽ��
    auto path = aStar.FindPath({ 1, 7 }, { 7, 1 }); //Ѱ·
    aStar.ShowMap(); //���ӻ�
    return 0;
}