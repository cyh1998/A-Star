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
    Vec2 coordinate_; //����
    int G; //��㵽��λ�õĳɱ�
    int H; //��λ�õ��յ�ĳɱ�������ʽ
    Node *parent_; //�����

    Node(Vec2 coordinate, Node *parent = nullptr)
    {
        coordinate_ = coordinate;
        G = 0;
        H = 0;
        parent_ = parent;
    };

    int GetCost() { return G + H; } //��ȡ�ܳɱ�
};

using CoordinateList = std::vector<Vec2>;
using NodeArr = std::vector<Node *>;

class AStar
{
public:
    AStar();

    void SetWorldSize(Vec2 size); //���õ�ͼ��С
    void SetWalls(CoordinateList walls); //����ǽ��
    CoordinateList FindPath(Vec2 source, Vec2 target); //A*Ѱ·

private:
    bool IfCollision(Vec2 coordinate); //�ж��Ƿ���ײ
    Node* FindNodeInArr(NodeArr &arr, Vec2 coordinate);
    int Manhattan(Vec2 source, Vec2 target); //�����پ���
    void ReleaseNodes(NodeArr& arr);

private:
    Vec2 worldSize_; //��ͼ��С
    int direction_; //������λ(4��8)
    CoordinateList directions_; //����
    CoordinateList walls_; //ǽ
};