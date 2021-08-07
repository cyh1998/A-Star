# A-Star
C++实现的A*算法

#### **用法示例**

```cpp
#include <iostream>
#include "source/AStar.h"

using namespace std;

int main()
{
    AStar aStaer;
    aStaer.SetWorldSize({ 7, 7 }); //设置世界大小
    std::vector<Vec2> walls = { { 2, 3 }, { 3, 3 }, { 4, 3 } };
    aStaer.SetWalls(walls); //设置墙体
    auto path = aStaer.FindPath({ 3, 1 }, { 3, 5 }); //寻路

    for (auto &i : path) { //路径打印
        std::cout << i.x << " " << i.y << "\n";
    }
}

```

