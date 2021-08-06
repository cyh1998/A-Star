#include "AStar.h"

Vec2 operator + (const Vec2 &lhs, const Vec2 &rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

AStar::AStar()
{
    worldSize_ = { 0, 0 };
    direction_ = 8;
    directions_ = {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
    };
    walls_ = {};
}

void AStar::SetWorldSize(Vec2 size)
{
    worldSize_ = size;
}

void AStar::SetWalls(CoordinateList walls)
{
    walls_ = walls;
}

CoordinateList AStar::FindPath(Vec2 source, Vec2 target)
{
    NodeArr openSet; //open表
    NodeArr closedSet; //close表
    Node* nodePtr = nullptr;

    // 添加起点
    openSet.emplace_back(new Node(source));

    while (!openSet.empty()) {
        // 获取open表中总成本最小的结点
        auto node = openSet.begin();
        nodePtr = *node;
        for (auto item = openSet.begin(); item != openSet.end(); ++item) {
            if ((*item)->GetCost() < (*node)->GetCost()) {
                node = item;
                nodePtr = *item;
            }
        }

        // 判断是否到达目标
        if (nodePtr->coordinate_ == target) {
            break;
        }

        // 添加到close表中，从open表中移除
        closedSet.emplace_back(nodePtr);
        openSet.erase(node);

        // 依次判断当前结点的各个方位
        for (int i = 0; i < direction_; ++i) {
            Vec2 findCoordinate(nodePtr->coordinate_ + directions_[i]);

            // 判读是否超出世界范围 是否不可达到(结点位置是墙体)
            if (IfCollision(findCoordinate) || FindNodeInArr(closedSet, findCoordinate)) {
                continue;
            }

            // 判断搜索结点是否已存在于open表中
            int gCost = nodePtr->G + ((i < 4) ? 10 : 14);
            Node* findNode = FindNodeInArr(openSet, findCoordinate);
            if (nullptr == findNode) { //不存在，添加添加新的结点
                findNode = new Node(findCoordinate, nodePtr);
                findNode->G = gCost;
                findNode->H = Manhattan(findNode->coordinate_, target);
                openSet.emplace_back(findNode);
            } else { //存在，判断是否需要更新
                if (gCost < findNode->G) {
                    findNode->parent_ = nodePtr;
                    findNode->G = gCost;
                }
            }
        }
    }

    // 生成最终路径
    CoordinateList path;
    while (nodePtr != nullptr) {
        path.emplace_back(nodePtr->coordinate_);
        nodePtr = nodePtr->parent_;
    }

    // 释放寻路结点
    ReleaseNodes(openSet);
    ReleaseNodes(closedSet);

    return path;
}

bool AStar::IfCollision(Vec2 coordinate)
{
    if (coordinate.x < 0 || coordinate.x >= worldSize_.x ||
        coordinate.y < 0 || coordinate.y >= worldSize_.y ||
        std::find(walls_.begin(), walls_.end(), coordinate) != walls_.end()) {
        return true;
    }
    return false;
}

Node* AStar::FindNodeInArr(NodeArr& arr, Vec2 coordinate)
{
    for (auto node : arr) {
        if (node->coordinate_ == coordinate) {
            return node;
        }
    }
    return nullptr;
}

int AStar::Manhattan(Vec2 source, Vec2 target)
{ 
    return 10 * (abs(source.x - target.x) + abs(source.y - target.y));
}

void AStar::ReleaseNodes(NodeArr& arr)
{
    for (auto i = arr.begin(); i != arr.end();) {
        delete *i;
        i = arr.erase(i);
    }
}
