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

    mat = nullptr;
}

AStar::~AStar()
{
    mat->release();
    delete mat;
}

void AStar::SetWorldSize(Vec2 size)
{
    worldSize_ = size;
    _DrawWorld();
}

void AStar::SetDiagonal(bool enable)
{
    direction_ = (enable ? 8 : 4);
}

void AStar::SetWalls(CoordinateList walls)
{
    walls_ = walls;
    _DrawWall();
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

    _DrawPath(path);
    _DrawSource(source);
    _DrawTarget(target);

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

void AStar::ShowMap()
{
    if (mat && !mat->empty()) {
        int width = worldSize_.x;
        int height = worldSize_.y;

        for (int i = 0; i <= width; ++i) {
            cv::line(*mat, cv::Point(i * GRID_SIZE, 0), cv::Point(i * GRID_SIZE, height * GRID_SIZE), SCALAR_COLOR_GRID, 1);
        }

        for (int i = 0; i <= height; ++i) {
            cv::line(*mat, cv::Point(0, i * GRID_SIZE), cv::Point(width * GRID_SIZE, i * GRID_SIZE), SCALAR_COLOR_GRID, 1);
        }

        cv::imshow("ImputImage", *mat);
        cv::waitKey(0);
    }
}

void AStar::_DrawWorld()
{
    mat = new cv::Mat(worldSize_.x * GRID_SIZE, worldSize_.y * GRID_SIZE, CV_8UC3, SCALAR_COLOR_BACKGROUND);
}

void AStar::_DrawWall()
{
    for (auto wall : walls_) {
        cv::rectangle(*mat, cv::Point(wall.x * GRID_SIZE, wall.y * GRID_SIZE), 
                      cv::Point((wall.x + 1) * GRID_SIZE, (wall.y + 1) * GRID_SIZE), SCALAR_COLOR_WALL, -1);
    }
}

void AStar::_DrawSource(Vec2 source)
{
    cv::rectangle(*mat, cv::Point(source.x * GRID_SIZE, source.y * GRID_SIZE), 
                  cv::Point((source.x + 1) * GRID_SIZE, (source.y + 1) * GRID_SIZE), SCALAR_COLOR_SOURCE, -1);
}

void AStar::_DrawTarget(Vec2 target)
{
    cv::rectangle(*mat, cv::Point(target.x * GRID_SIZE, target.y * GRID_SIZE), 
                  cv::Point((target.x + 1) * GRID_SIZE, (target.y + 1) * GRID_SIZE), SCALAR_COLOR_TARGET, -1);
}

void AStar::_DrawPath(CoordinateList path)
{
    for (auto i : path) {
        cv::rectangle(*mat, cv::Point(i.x * GRID_SIZE, i.y * GRID_SIZE),
                      cv::Point((i.x + 1) * GRID_SIZE, (i.y + 1) * GRID_SIZE), SCALAR_COLOR_PATH, -1);
    }
}
