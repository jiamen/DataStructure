#ifndef _MATRIXGRAPH_H_
#define _MATRIXGRAPH_H_


#include "Graph.h"
#include "Exception.h"
#include "DynamicArray.h"


namespace JMLib
{

template < int N, typename V, typename E >
//          顶点数 顶点的数据类型   权值的类型
class MatrixGraph : public Graph<V, E>
{
protected:
    V* m_vertexes[N];       // 数据元素类型与工程使用挂钩, 比方结点是字符串或者int
    E* m_edges[N][N];
    int m_eCount;           // 记录当前图中有多少条边


public:
    MatrixGraph()
    {
        for( int i=0; i<vCount(); i ++ )
        {
            m_vertexes[i] = NULL;

            for( int j=0; j<vCount(); j ++ )
            {
                m_edges[i][j] = NULL;
            }
        }

        m_eCount = 0;
    }


    // 获取顶点i相关联的值
    V getVertex(int i)                  // O(1)
    {
        V ret;

        if( !getVertex(i, ret) )    // 返回为false
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    // 获取顶点的值
    bool getVertex(int i, V& value)     // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) );

        if( ret )
        {
            if( NULL != m_vertexes[i] )
            {
                value = *(m_vertexes[i]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex ...");
            }
        }

        return ret;
    }


    // 设置顶点的值
    bool setVertex(int i, const V& value)   // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) );

        if( ret )
        {
            V* data = m_vertexes[i];

            if( NULL == data )
            {
                data = new V();
            }

            if( NULL != data )
            {
                *(data) = value;

                m_vertexes[i] = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory to store new vertex value ...");
            }
        }

        return ret;
    }


    // 获取邻接顶点
    SharedPointer< Array<int> > getAdjacent(int i)  // O(n), 耗时与顶点规模有关
    {
        DynamicArray<int>* ret = NULL;

        if( (0<=i) && (i<vCount()) )
        {
            int n = 0;      // 与i相关联的顶点的个数

            for( int j=0; j<vCount(); j ++ )
            {
                if( NULL != m_edges[i][j] )     // 与当前顶点的边/权重不为0的顶点都记录下来
                {
                    n ++;
                }
            }

            ret = new DynamicArray<int>(n);
            if( NULL != ret )
            {
                for(int j=0, k=0; j<vCount(); j ++)
                {
                    if( NULL != m_edges[i][j] )
                    {
                        ret->set(k++, j);       // 记录对应的顶点编号
                    }
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory to create ret object value ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }


    // 获取边
    E getEdge(int i, int j)     // O(1)
    {
        E ret;

        if( !getEdge(i, j, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Index <i, j> is invalid ...");
        }

        return ret;
    }

    // 获取边
    bool getEdge(int i, int j, E& value)        // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) &&
                     (0 <= j) && (j < vCount()) );

        if( ret )
        {
            if( NULL != m_edges[i][j] )
            {
                value = *(m_edges[i][j]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge ...");
            }
        }

        return ret;
    }


    // 设置边
    bool setEdge(int i, int j, const E& value)      // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) &&
                     (0 <= j) && (j < vCount()) );

        if( ret )
        {
            E* ne = m_edges[i][j];

            if( NULL == ne )        // i与j之间没有边
            {
                ne = new E();
                if( NULL != ne )
                {
                    *ne = value;

                    m_edges[i][j] = ne;
                    m_eCount ++;

                    // delete ne;      // 这里多了释放
                }
                else
                {
                    THROW_EXCEPTION(NoEnoughMemoryException, "No Enough Memory to store new edge value ...");
                }
            }
            else                    // i与j之间有边, 直接修改就可以
            {
                *ne = value;
            }
        }

        return ret;
    }

    // 删除边
    bool removeEdge(int i, int j)           // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) &&
                     (0 <= j) && (j < vCount()) );

        if( ret )
        {
            E* toDel = m_edges[i][j];   // 使用中间变量来删除结点，为了异常安全

            m_edges[i][j] = NULL;

            if( NULL != toDel )
            {
                m_eCount --;

                delete toDel;
            }
        }

        return ret;
    }

    // 获取顶点数
    int vCount()        // O(1)
    {
        return N;
    }

    // 获取边数
    int eCount()        // O(1)
    {
        return m_eCount;
    }

    // 获取顶点的出度
    int OD(int i)       // O(n)
    {
        int ret = 0;

        if( (0 <= i) && (i < vCount()) )
        {
            for( int j=0; j<vCount(); j ++ )    // 遍历邻接矩阵中每个顶点的 “行”
            {
                if( NULL != m_edges[i][j] )
                {
                    ret ++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    // 获取顶点的入度
    int ID(int i)       // O(n)
    {
        int ret = 0;

        if( (0 <= i) && (i < vCount()) )
        {
            for( int j=0; j<vCount(); j ++ )    // 遍历邻接矩阵中每个顶点的 “行”
            {
                if( NULL != m_edges[j][i] )
                {
                    ret ++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }


    // 最小生成树中的：判断在当前图中顶点i到顶点j是否邻接
    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount())
                && (m_edges[i][j] != NULL);
    }


    ~MatrixGraph()
    {
        for( int i=0; i<vCount(); i ++ )
        {
            for( int j=0; j<vCount(); j ++ )
            {
                delete m_edges[i][j];
            }

            delete m_vertexes[i];
        }

        m_eCount = 0;       // 析构中加不加这一句？？？
    }
};

}


#endif // _MATRIXGRAPH_H_
