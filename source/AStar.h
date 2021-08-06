#pragma once

#include <vector>

struct Vec2
{
    int x;
    int y;
    bool operator == (const Vec2 &rhs)
    {
        return (x == rhs.x && y == rhs.y);
    };

};

struct Node
{
    Vec2 coordinate_; //坐标
    int G; //起点到该位置的成本
    int H; //该位置到终点的成本，启发式
    Node *parent_; //父结点

    Node(Vec2 coordinate, Node *parent = nullptr)
    {
        coordinate_ = coordinate;
        G = 0;
        H = 0;
        parent_ = parent;
    };

    int GetCost() { return G + H; } //获取总成本
};

using CoordinateList = std::vector<Vec2>;
using NodeArr = std::vector<Node *>;

class AStar
{
public:
    AStar();

    void SetWorldSize(Vec2 size); //设置地图大小
    void SetWalls(CoordinateList walls); //设置墙体
    CoordinateList FindPath(Vec2 source, Vec2 target); //A*寻路

private:
    bool IfCollision(Vec2 coordinate); //判断是否碰撞
    Node* FindNodeInArr(NodeArr &arr, Vec2 coordinate);
    int Manhattan(Vec2 source, Vec2 target); //曼哈顿距离
    void ReleaseNodes(NodeArr& arr);

private:
    Vec2 worldSize_; //地图大小
    int direction_; //搜索方位(4或8)
    CoordinateList directions_; //方向
    CoordinateList walls_; //墙
};