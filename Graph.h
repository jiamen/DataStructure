#ifndef _GRAPH_H_
#define _GRAPH_H_


#include "Object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"      // 广度优先遍历
#include "LinkStack.h"      // 深度优先遍历
//#include "Sort.h"


namespace JMLib
{

template <typename E>
struct Edge : public Object     // 定义邻接链表图中的每个顶点的边的表示，本结构体在图的邻接链表结构(LinkGraph.h)中会用到
{
    int b;      // 起始点begin
    int e;      // 终止点end
    E data;     // 权值

    Edge(int i=-1, int j=-1)
    {
        b = i;
        e = j;
    }

    Edge(int i, int j, const E& value)
    {
        b = i;
        e = j;
        data =value;
    }

    // 删除顶点时，还要删除其他顶点与该顶点的边
    bool operator == (const Edge<E>& obj)
    {
        return (b == obj.b) && (e == obj.e);
    }
    bool operator != (const Edge<E>& obj)
    {
        return !(*this == obj);     // 使用上面重载的"=="比较操作操作符
    }
};


template <typename V, typename E>
           // 点集       边集
class Graph : public Object
{
protected:
    template <typename T>
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if( NULL != ret )
        {
            for(int i=0; i<ret->length(); i ++,queue.remove())
            {
                ret->set(i, queue.front());
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create ret object ...");
        }

        return ret;
    }


    SharedPointer< Array< Edge<E> > > getUndirectedEdges()
    {
        DynamicArray< Edge<E> >* ret = NULL;

        if( asUndirected() )        // 当前图能否看做无向图
        {
            LinkQueue< Edge<E> > queue;

            for(int i=0; i<vCount(); i ++)
            {
                for(int j=i; j<vCount(); j ++)
                {
                    if( isAdjacent(i, j) )
                    {
                        queue.add(Edge<E>(i, j, getEdge(i, j)));
                    }
                }
            }

            ret = toArray(queue);   // 注意队列与数组的转换
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "This function is only for undirected graph ...");
        }

        return ret;
    }


public:

    // 获取顶点的值
    virtual V getVertex(int i) = 0;
    // 获取顶点的值
    virtual bool getVertex(int i, V& value) = 0;

    // 设置顶点的值
    virtual bool setVertex(int i, const V& value) = 0;
    // 获取邻接顶点
    virtual SharedPointer< Array<int> > getAdjacent(int i) = 0;

    // 获取边
    virtual E getEdge(int i, int j) = 0;
    // 获取边

    virtual bool getEdge(int i, int j, E& value) = 0;
    // 设置边
    virtual bool setEdge(int i, int j, const E& value) = 0;
    // 删除边
    virtual bool removeEdge(int i, int j) = 0;
    // 获取顶点数
    virtual int vCount() = 0;
    // 获取边数
    virtual int eCount() = 0;
    // 获取顶点的出度
    virtual int OD(int i) = 0;
    // 获取顶点的入度
    virtual int ID(int i) = 0;
    // 获取顶点的度
    virtual int TD(int i)
    {
        return OD(i) + ID(i);
    }


    // 图的遍历方式一：广度优先
    SharedPointer< Array<int> > BFS(int i)      // 通过数组的次序反映访问的次序
    {
        DynamicArray<int>* ret = NULL;  // 这里定义的数组

        if( (0<=i) && (i<vCount()) )
        {
            LinkQueue<int> q;
            LinkQueue<int> r;           // 这里定义的队列
            DynamicArray<bool> visited(vCount());

            for(int j=0; j<visited.length(); j++)
            {
                visited[j] = false;
            }

            q.add(i);

            while( q.length() > 0 )
            {
                int v = q.front();

                q.remove();     // 删掉队列 队首元素，出队

                if( !visited[v] )
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for(int k=0; k<aj->length(); k ++)
                    {
                        q.add((*aj)[k]);
                    }

                    r.add(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    // 图的遍历方式二：深度优先
    SharedPointer< Array<int> > DFS(int i)      // 通过数组的次序反映访问的次序
    {
        DynamicArray<int>* ret = NULL;

        if( (0 <= i) && (i < vCount()) )
        {
            LinkStack<int> s;       // 这里是栈
            LinkQueue<int> r;       // 队列
            DynamicArray<bool> visited(vCount());

            for(int j=0; j<visited.length(); j ++)
            {
                visited[j] = false;
            }

            s.push(i);              // 压入起始遍历顶点编号

            while( s.size() > 0 )
            {
                int v = s.top();    // 取出栈顶元素

                s.pop();            // 弹出栈顶元素

                if( !visited[v] )
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for(int j=aj->length()-1; j>=0; j --)   // 注意这里是逆顺序压栈
                    {
                        s.push((*aj)[j]);
                    }

                    r.add(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }


    // 解决最小生成树问题
    virtual bool isAdjacent(int i, int j) = 0;  // 判断在当前图中顶点i到顶点j是否邻接

    bool asUndirected()        // 判断当前的有向图是否能够看做无向图, 因为最小生成树问题仅对无向图而言有意义
    {
        bool ret = true;

        for(int i=0; i<vCount(); i ++)
        {
            for(int j=0; j<vCount(); j ++)
            {
                if( isAdjacent(i, j) )
                {
                    ret = ret && isAdjacent(j, i) && (getEdge(i, j) == getEdge(j, i));
                }
            }
        }

        return ret;
    }

    SharedPointer< Array<Edge<E>> > prim(const E& LIMIT, const bool MINIMUM = true)      // E 是权值的数据类型
    {
        LinkQueue< Edge<E> > ret;

        if( asUndirected() )
        {
            DynamicArray<int>  adjVex(vCount());        // 当前顶点的邻接顶点数组
            DynamicArray<bool> mark(vCount());
            DynamicArray<E> cost(vCount());
            SharedPointer< Array<int> > aj = NULL;      // 邻接顶点
            bool end = false;
            int v = 0;      // 初始顶点

            for(int i=0; i<vCount(); i ++)
            {
                adjVex[i] = -1;     // 邻接顶点都设为-1
                mark[i] = false;    // 标记
                cost[i] = LIMIT;    // 损失cost都初始化为最大上限值
            }

            mark[v] = true;

            aj = getAdjacent(v);

            // 设置损失函数和邻接顶点的邻接顶点(就是当前顶点自身)
            for(int j=0; j<aj->length(); j ++)
            {
                cost[(*aj)[j]] = getEdge(v, (*aj)[j]);
                adjVex[(*aj)[j]] = v;       // 当前顶点的邻接顶点数组中，设置邻接顶点为当前顶点
            }


            // 在F集合的每个顶点的所有边中挑选最小权值（损失）的边
            for(int i=0; (i<vCount()) && !end; i ++)        // 遍历次数为定点数N，也可能提前结束，∴这里添加中断条件
            {
                E m = LIMIT;
                int k = -1;     // k用于记录最小值的顶点

                for(int j=0; j<vCount(); j ++)
                {
                    if( !mark[j] && (MINIMUM ? (cost[j] < m) : (cost[j] > m)) )     // !mark[j]是因为F中的顶点还没有被标记
                    {
                        m = cost[j];    // 更新m，找到最小的边权值
                        k = j;          // 记录最小顶点的位置
                    }
                }

                end = (k == -1);       // 遍历完cost数组发现找不到满足要求的最小权值

                if( !end )
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));
                                  // 当前顶点  F中的顶点
                    mark[k] = true;

                    aj = getAdjacent(k);

                    for(int j=0; j<aj->length(); j ++)
                    {
                        if( !mark[(*aj)[j]] && ( MINIMUM ? (getEdge(k, (*aj)[j]) < cost[(*aj)[j]]) : (getEdge(k, (*aj)[j]) > cost[(*aj)[j]]) ) )    // 找最大 or 最小
                        {
                            cost[(*aj)[j]] = getEdge(k, (*aj)[j]);
                            adjVex[(*aj)[j]] = k;
                        }
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "Prim operation is only for undirected graph ...");
        }

        if( ret.length() != (vCount() - 1) )        // 边的数量 = 顶点的数量-1
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edge for prim operation ...");
        }

        return toArray(ret);        // 注意返回值类型的转换
    }


    SharedPointer< Array< Edge<E> > > kruskal()
    {
        LinkQueue< Edge<E> > ret;
        SharedPointer< Array< Edge<E> > > edges = getUndirectedEdges(); // 得到所有边的集合
        DynamicArray<int> p(vCount());      // ☆☆☆  前驱顶点标记数组  △△△

        // 前驱顶点初始化
        for(int i=0; i<p.length(); i ++)
        {
            p[i] = -1;
        }


    }
};

}

#endif // _GRAPH_H_
