#include <iostream>
#include <climits>

#include "MatrixGraph.h"
#include "ListGraph.h"
#include "SharedPointer.h"
#include "StaticArray.h"
#include "DynamicArray.h"

namespace Exercise_1
{

/* 问题1: 判断一幅图是否是二分图
 * 二分图定义 : 将图中顶点分为两部分,任意一条边的一个顶点属于集合A,另一个顶点属于集合B
 */

enum { VERTEX_CNT = 5 };
void set_input(MyLib::ListGraph<int, int> &graph)
{
    for (int i = 0; i < VERTEX_CNT; ++i)
        graph.addVertex(i);
#if 0
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
#else
    graph.setEdge(0, 3, 1);
    graph.setEdge(3, 0, 1);

    graph.setEdge(0, 2, 1);
    graph.setEdge(2, 0, 1);

    graph.setEdge(3, 4, 1);
    graph.setEdge(4, 3, 1);

    graph.setEdge(2, 5, 1);
    graph.setEdge(5, 2, 1);

    graph.setEdge(4, 1, 1);
    graph.setEdge(1, 4, 1);

    graph.setEdge(5, 1, 1);
    graph.setEdge(1, 1, 1);
#endif
}

/*这种解法属于以顶点为出发点解决问题*/
bool solutionV1(MyLib::ListGraph<int, int> &graph)
{
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

/*版本2是版本1的优化版,是直播里边的思路*/
bool solutionV2(MyLib::Graph<int, int> &graph)
{
    bool ret = true;
    MyLib::DynamicArray<int> mark(graph.vCount());

    /*m_mark记录每个顶点所在的子集,分别为-1,0,1. -1表示未分配,0和1分别表示两子集,采用这种方式有一个好处是当我们已知一条边的一个顶点所在子集为x,则这条边的另一个顶点为 (1-x)*/
    for (int i = 0; i < graph.vCount(); i++)
        mark[i] = -1;

    for (int i = 0; ret && i < graph.vCount(); i++)
    {
        MyLib::SharedPointer< MyLib::Array<int> > res = graph.getAdjacent(i);
        MyLib::Array<int> &arr = res->self();

        /*如果没分配则塞到集合A*/
        if (-1 == mark[i])
            mark[i] = 0;

        for (int j = 0; j < arr.length(); j++)
        {
            int index = arr[j];
            if (-1 == mark[index])
            {
                mark[index] = 1 - mark[i];
            }
            else if (mark[i] == mark[index])
            {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

/*版本3从边出发,检查边的两个端点是否满足要求*/
bool solutionV3(MyLib::ListGraph<int, int> &graph)
{
    bool ret = true;
    MyLib::DynamicArray<int> mark(graph.vCount());

    for (int i = 0; i < graph.vCount(); i++)
        mark[i] = -1;

    for (int i = 0; i < mark.length(); i++)
    {
        /*
         * 这里虽然循环次数是顶点数,但是实际上循环体执行次数是边数e
         * 这就是版本3与版本2最大的不同
         * 因此在实际应用中当图的顶点数较多时可以使用版本3,当边数较多时则使用版本2
         */
        for (int j = i; j < mark.length(); j++)
        {
            /*查找顶点i所在的边的另一个端点j,检查顶点i和顶点j所在集合*/
            /*此处采用的方法是通过图的getEdge接口,因为i和j之间不一定存在边,因此需要try catch*/
            try
            {
                /*只要没有触发异常,则此时的i和j必然存在一条无向边*/
                graph.getEdge(i, j);

                /*检查顶点状态*/
                if (-1 == mark[i] && -1 == mark[j])
                {
                    /*两个顶点都没有被塞入集合,则将它们放入集合中*/
                    mark[i] = 0;
                    mark[j] = 1;
                }
                else if (-1 != mark[i] && -1 == mark[j])
                {
                    /*顶点i在一个集合中,而顶点j未知,则顶点j只能存入另一个集合*/
                    mark[j] = 1 - mark[i];
                }
                else if (-1 == mark[i] && -1 != mark[j])
                {
                    mark[i] = 1 - mark[j];
                }
                else
                {
                    /*i和j都已经在集合,检查是否位于两个不同集合*/
                    if (mark[i] == mark[j])
                    {
                        ret = false;
                        break;
                    }
                }

            }
            catch (const MyLib::Exception &)
            {
            }
        }
    }

    return ret;
}

/*功能函数:对顶点idx进行标记,将之标记到flag集合中,并递归对顶点idx的所有邻居进行标记,标记为(1-flag)*/
bool setMark(MyLib::ListGraph<int, int> &graph, MyLib::DynamicArray<int> &mark, int idx, int flag = 0)
{
    bool ret = true;

    if (-1 == mark[idx])
    {
        mark[idx] = flag;
        /*找出顶点idx的所有邻居,将其标记为(1 - flag),并依次深入所有邻居的相邻顶点进行标记,直到所有能访问的邻居都被标记上或者发现不满足要求的邻居时即退出递归*/
        /*也就是图的深度优先搜索DFS*/
        MyLib::SharedPointer< MyLib::Array<int> > p = graph.getAdjacent(idx);
        MyLib::Array<int> &adj = p->self();

        for (int j = 0; ret && j < adj.length(); j++)
            ret = setMark(graph, mark, adj[j], 1 - mark[idx]) && (mark[idx] != mark[adj[j]]);
    }

    return ret;
}

bool solutionV4(MyLib::ListGraph<int, int> &graph)
{
    bool ret = true;
    MyLib::DynamicArray<int> mark(graph.vCount());

    for (int i = 0; i < graph.vCount(); i++)
        mark[i] = -1;

    /*遍历所有顶点,并深入其所有相邻的顶点进行相应的标记*/
    for (int i = 0; ret && i < mark.length(); i++)
        ret = setMark(graph, mark, i);

    return ret;
}

bool check(void)
{
    MyLib::ListGraph<int, int> graph;

    set_input(graph);

    int seed = 3;//(rand() % 4);
    switch (seed)
    {
        case 0 : { return solutionV1(graph); }
        case 1 : { return solutionV2(graph); }
        case 2 : { return solutionV3(graph); }
        case 3 : { return solutionV4(graph); }
    }

    return false;
}

}

namespace Exercise_2
{

/*
 * 问题2: 矩阵中的距离
 *  一个矩阵M只由0和1组成,计算一个大小相同的矩阵D
 *  D中元素是M中对应元素离最近的0元素的距离
 *  水平或者垂直方向相邻的两个矩阵元素距离为1
 *  实际应用:在沙漠中求距离当前位置最近的水源位置
 */
#define ROW_COL_TO_LINEAR(r, c) ((r) * COL + (c))

#define GET_ROW(index) ((index) / COL)
#define GET_COL(index) ((index) % COL)
#define INF (INT_MAX)

enum { ROW = 4 };
enum { COL = 4 };

int map[ROW][COL] =
{
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {1, 1, 0, 0},
    {1, 1, 1, 1}
};

/*模仿数据结构课里边八皇后的棋盘访问方式*/
/*0-3分别对应上下左右*/
const int row_step[4] = {-1, 1, 0, 0};
const int col_step[4] = {0, 0, -1, 1};

inline bool pos_invalid(int r, int c)
{
    return (((0 <= r) && (r < ROW)) && ((0 <= c) && (c < COL)));
}

void print_rst(int *dist)
{
    for (int r = 0; r < ROW; r++)
    {
        int *dist_r = &dist[ROW_COL_TO_LINEAR(r, 0)];
        for (int c = 0; c < COL; c++)
            std::cout << dist_r[c] << "\t";

        std::cout << std::endl;
    }
}

void solution(void)
{
    MyLib::LinkQueue<int> queue;
    MyLib::StaticArray<bool, ROW * COL> discovered;

    /*思路:分别从各个值为0的顶点出发,其前后左右4个方向的顶点如果值为0,则距离为0,否则就是1(0 + 1 = 1),再分别从这4个当中值不为0的顶点出发,依次类推,迭代到4个方向的顶点值都不为0就可以返回了*/
    /*整个算法类似于BFS,或者说二叉树的层次遍历更形象一点,从距离为0的点出发,一层一层向下走,layer[i] = layer[i - 1]*/

    //int dist[ROW * COL];
    MyLib::StaticArray<int, ROW * COL>  dist;

    for (int r = 0; r < ROW; r++)
    {
        for (int c = 0; c < COL; c++)
        {
            int index = ROW_COL_TO_LINEAR(r, c);
            if (0 == map[r][c])
            {
                queue.add(index);
                dist[index] = 0;
                discovered[index] = true;   /*标记为已发现*/
            }
            else
            {
                /*初始化阶段值不为0的顶点距离初始化为无限大*/
                dist[index] = INF;
                discovered[index] = false;
            }
        }
    }

    while (queue.length())
    {
        int index = queue.front();
        queue.remove();

        int row = GET_ROW(index);
        int col = GET_COL(index);
        /*index为已经处理过的顶点,它的距离为已知值,以index为出发点,访问其4个方向的顶点,如果当中存在未发现的顶点,则这个顶点的距离值应当为dist[index] + 1*/
        for (int i = 0; i < 4; i++)
        {
            int nr = row + row_step[i];
            int nc = col + col_step[i];

            if (!pos_invalid(nr, nc))
                continue;

            int next_idx = ROW_COL_TO_LINEAR(nr, nc);
            if (!discovered[next_idx])
            {
                /*发现一个可计算距离的顶点*/
                /*新发现顶点可作为后续某次迭代的顶点,将其入队列*/
                queue.add(next_idx);

                /*标记为已发现*/
                discovered[next_idx] = true;
                /*新发现顶点的距离应当为 dist[index] + 1*/
                dist[next_idx] = dist[index] + 1;
            }

        }
    }

    print_rst(dist.array());
}

/*
 * 算法核心步骤实现函数
 * 功能:递归函数,求顶点i距离最近的数值为0的顶点距离记录到ret中
 */
void MinMatrixDistance(MyLib::Graph<int, int> &graph, int v, MyLib::DynamicArray<int> &ret, MyLib::DynamicArray<bool> &path)
{
    path[v] = true; /*将顶点i标记为已访问*/

    /*如果顶点i的值为0,即就是水源,则令ret[i] = 0即可*/
    /*主要处理当前位置不是水源的情况*/
    if (0 != graph.getVertex(v))
    {
        //if (-1 == ret[v])
        {
            /*访问顶点i的所有邻居*/
            MyLib::SharedPointer< MyLib::Array<int> > p = graph.getAdjacent(v);
            MyLib::Array<int> &adj = p->self();

            /*由于顶点i不是水源,先来检查它的直接邻居有没有水源,如果有,则ret[i]必然等于1*/
            for (int i = 0; i < adj.length(); i++)
            {
                int neighbour = adj[i];
                if (0 == graph.getVertex(neighbour))
                {
                    ret[i] = 1;
                    break;
                }
            }

            /*如果还没找到水源,则需要递归查找,同时这里要注意通过path变量避免重复求解顶点i*/
            if (-1 == ret[v])
            {
                for (int i = 0; i < adj.length(); i++)
                {
                    int neighbour = adj[i];
                    if (path[neighbour])
                        continue;

                    /*递归求解顶点neighbour的最短距离*/
                    MinMatrixDistance(graph, neighbour, ret, path);
                }

                /*现在从i的各个直接邻居出发求解得到的最短距离已经知道了,ret[i] = min(ret{neighbours}) + 1*/
                int dist = -1;
                for (int i = 0; i < adj.length(); i++)
                {
                    int neighbour = adj[i];
                    if ((uint32_t)dist > (uint32_t)ret[neighbour])
                        dist = ret[neighbour];
                }

                /*注意有一种情况,就是整个图中没有水源,这时候dist还是-1*/
                if (-1 != dist)
                {
                    /*顶点i的最短距离就是dist+1*/
                    ret[v] = dist + 1;
                }
            }

            /*此时再去更新顶点i的所有邻居的最短距离,因为在上面的递归中实际上由于path的设定导致遍历过程中不能发现经过顶点i的路径(path为true时不递归),也就是说经过递归后neighbour的ret只是中间值而有可能不是最终值,必须将经过顶点i的路径计算上才能做为最终值*/
            for (int i = 0; i < adj.length(); i++)
            {
                int neighbour = adj[i];
                /*查看顶点neighbour是否能经过顶点i能更快找到水源,如果是,更新之*/
                if (ret[neighbour] > (ret[v] + 1))
                    ret[neighbour] = ret[v] + 1;
            }
        }
    }
    else
    {
        ret[v] = 0;
    }

    path[v] = false; /*函数退出时,顶点i的所求必然已求解,当前顶点已访问完*/
}

/*唐老师直播讲解版本*/
MyLib::DynamicArray<int> solutionV2(MyLib::Graph<int, int> &graph)
{
    MyLib::DynamicArray<int> ret(graph.vCount());   /*结果矩阵*/
    MyLib::DynamicArray<bool> path(graph.vCount());  /*图在遍历时可能出现环路,需要标记已经访问过的顶点来避免反复访问一个顶点*/

    //std::cout << graph.vCount() << std::endl;

    /*初始化,path初始化为false表示所有顶点未访问,ret初始化为-1*/
    for (int i = 0; i < graph.vCount(); i++)
    {
        ret[i] = -1;
        path[i] = false;
    }

    /*遍历图的所有顶点*/
    for (int i = 0; i < graph.vCount(); i++)
        MinMatrixDistance(graph, i, ret, path);

    return ret;
}

/*将矩阵信息转化为图,这是解决问题的第一步*/
void set_input(MyLib::ListGraph<int, int> &graph)
{
    /*先设置顶点信息*/
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            graph.setVertex(ROW_COL_TO_LINEAR(i, j), map[i][j]);

    /*设置边信息,横向竖向都存在边*/
    /*横向边信息*/
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL - 1; col++)
        {
            /*无向边*/
            int vertex = ROW_COL_TO_LINEAR(row, col);
            graph.setEdge(vertex, vertex + 1, 1);
            graph.setEdge(vertex + 1, vertex, 1);
        }
    }

    /*纵向*/
    for (int col = 0; col < COL; col++)
    {
        for (int row = 0; row < ROW - 1; row++)
        {
            /*无向边*/
            int vi = ROW_COL_TO_LINEAR(row, col);
            int vj = ROW_COL_TO_LINEAR(row + 1, col);
            graph.setEdge(vi, vj, 1);
            graph.setEdge(vj, vi, 1);
        }
    }
}

void check(void)
{
    MyLib::ListGraph<int, int> graph(ROW * COL);

    set_input(graph);

    MyLib::DynamicArray<int> ret = solutionV2(graph);

    print_rst(ret.array());
}

}


void graph_practice(void)
{
    std::cout << Exercise_1::check() << std::endl;

    std::cout << std::endl << std::endl;
    //
    Exercise_2::solution();
    Exercise_2::check();

}
