#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

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
    ~AStar();
    void SetWorldSize(Vec2 size); //设置地图大小
    void SetDiagonal(bool enable); //设置对角线方向
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

// cv可视化
public:
    void ShowMap(); //显示地图

private:
    void _DrawWorld(); //绘制世界
    void _DrawWall(); //绘制墙体
    void _DrawSource(Vec2 source); //绘制起点
    void _DrawTarget(Vec2 target); //绘制终点
    void _DrawPath(CoordinateList path); //绘制路径

private:
    cv::Mat *mat;

    const int GRID_SIZE = 30; //网格大小
    const cv::Scalar SCALAR_COLOR_BACKGROUND = cv::Scalar(247, 247, 247); //背景颜色
    const cv::Scalar SCALAR_COLOR_GRID = cv::Scalar(0, 0, 0); //网格颜色
    const cv::Scalar SCALAR_COLOR_WALL = cv::Scalar(46, 46, 46); //墙体颜色
    const cv::Scalar SCALAR_COLOR_SOURCE = cv::Scalar(0, 255, 102); //起点颜色
    const cv::Scalar SCALAR_COLOR_TARGET = cv::Scalar(44, 44, 238); //终点颜色
    const cv::Scalar SCALAR_COLOR_PATH = cv::Scalar(255, 204, 0); //路径颜色
};