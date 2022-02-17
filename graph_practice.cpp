#include <iostream>

#include "MatrixGraph.h"
#include "ListGraph.h"
#include "SharedPointer.h"
#include "StaticArray.h"

/* 问题1: 判断一幅图是否是二分图
 * 二分图定义 : 将图中顶点分为两部分,任意一条边的一个顶点属于集合A,另一个顶点属于集合B
 */
static bool exercise_1(void)
{
    MyLib::ListGraph<int, int> graph;
    enum { VERTEX_CNT = 4 };
    
    for (int i = 0; i < VERTEX_CNT; ++i)
        graph.addVertex(i);
    
    graph.setEdge(0, 1, 1);
    graph.setEdge(1, 0, 1);

    graph.setEdge(0, 2, 1);
    graph.setEdge(2, 0, 1);

    graph.setEdge(2, 3, 1);
    graph.setEdge(3, 2, 1);

    graph.setEdge(1, 3, 1);
    graph.setEdge(3, 1, 1);

    graph.setEdge(0, 3, 1);
    graph.setEdge(3, 0, 1);

    MyLib::StaticArray<bool, VERTEX_CNT> setA;
    MyLib::StaticArray<bool, VERTEX_CNT> setB;
    bool OK = true;

    for (int i = 0; i < VERTEX_CNT; ++i)
    {
        setA[i] = false;
        setB[i] = false;
    }

    for (int i = 0; OK && i < graph.vCount(); ++i)
    {
        /*看一下当前顶点是否已经被划分集合,都没被划分的话就扔给集合A*/
        bool isA = false;

        if (!setB[i])   /*简化成只要不在集合B就往A里边扔*/
        {
            setA[i] = true;
            isA = true;
        }

        MyLib::SharedPointer< MyLib::Array<int> > adj = graph.getAdjacent(i);
        /*遍历顶点i的所有邻居*/
        for (int j = 0; j < adj->length(); ++j)
        {
            int neighbour = (*adj)[j];
            /*判断一下,如果当前顶点i在集合A中,那么它的当前处理邻居要么未分配集合要么只能在集合B中,否则就是错的*/
            if (isA)
            {
                if (setA[neighbour])
                {
                    OK = false;
                    break;
                }

                setB[neighbour] = true;
            }
            else
            {
                if (setB[neighbour])
                {
                    OK = false;
                    break;
                }

                setA[neighbour] = true;
            }
        } 
    }

    return OK;
}

/*
 * 问题2: 矩阵中的距离
 *  一个矩阵M只由0和1组成,计算一个大小相同的矩阵D
 *  D中元素是M中对应元素离最近的0元素的距离
 *  水平或者垂直方向相邻的两个矩阵元素距离为1
 */
static void exercise_2(void)
{
    enum { ROW = 3 };
    enum { COL = 3 };

    MyLib::LinkQueue<int> queue;
    
    int map[ROW][COL] = 
    {
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 1} 
    };

    int dist[ROW * COL] = {-1};
    for (int r = 0; r < ROW; r++)
    {
        for (int c = 0; c < COL; c++)
        {
            dist[r * COL + c] = 0;  /*本身是0的顶点距离为0*/

            if (map[r][c])
                queue.add(r * COL + c);
        }
    }

}

int main(void)
{
    std::cout << exercise_1() << std::endl;

    return 0;
}
