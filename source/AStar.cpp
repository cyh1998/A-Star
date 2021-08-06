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
    NodeArr openSet; //open��
    NodeArr closedSet; //close��
    Node* nodePtr = nullptr;

    // ������
    openSet.emplace_back(new Node(source));

    while (!openSet.empty()) {
        // ��ȡopen�����ܳɱ���С�Ľ��
        auto node = openSet.begin();
        nodePtr = *node;
        for (auto item = openSet.begin(); item != openSet.end(); ++item) {
            if ((*item)->GetCost() < (*node)->GetCost()) {
                node = item;
                nodePtr = *item;
            }
        }

        // �ж��Ƿ񵽴�Ŀ��
        if (nodePtr->coordinate_ == target) {
            break;
        }

        // ��ӵ�close���У���open�����Ƴ�
        closedSet.emplace_back(nodePtr);
        openSet.erase(node);

        // �����жϵ�ǰ���ĸ�����λ
        for (int i = 0; i < direction_; ++i) {
            Vec2 findCoordinate(nodePtr->coordinate_ + directions_[i]);

            // �ж��Ƿ񳬳����緶Χ �Ƿ񲻿ɴﵽ(���λ����ǽ��)
            if (IfCollision(findCoordinate) || FindNodeInArr(closedSet, findCoordinate)) {
                continue;
            }

            // �ж���������Ƿ��Ѵ�����open����
            int gCost = nodePtr->G + ((i < 4) ? 10 : 14);
            Node* findNode = FindNodeInArr(openSet, findCoordinate);
            if (nullptr == findNode) { //�����ڣ��������µĽ��
                findNode = new Node(findCoordinate, nodePtr);
                findNode->G = gCost;
                findNode->H = Manhattan(findNode->coordinate_, target);
                openSet.emplace_back(findNode);
            } else { //���ڣ��ж��Ƿ���Ҫ����
                if (gCost < findNode->G) {
                    findNode->parent_ = nodePtr;
                    findNode->G = gCost;
                }
            }
        }
    }

    // ��������·��
    CoordinateList path;
    while (nodePtr != nullptr) {
        path.emplace_back(nodePtr->coordinate_);
        nodePtr = nodePtr->parent_;
    }

    // �ͷ�Ѱ·���
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
