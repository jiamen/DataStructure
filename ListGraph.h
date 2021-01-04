#ifndef _LISTGRAPH_H_
#define _LISTGRAPH_H_


#include "Graph.h"
#include "LinkList.h"
#include "Exception.h"
#include "DynamicArray.h"


namespace JMLib
{

            // 点集   边集 的类型
template <typename V, typename E>
class ListGraph : public Graph<V, E>
{
protected:
    struct Vertex : public Object   // 定义顶点结构体
    {
        V* data;                    // 当前图顶点的自身值
        LinkList< Edge<E> > edge;   // 存放与 当前顶点 连接的 邻接顶点的链表, 详情见《图的存储结构——邻接链表法示例图》
        // 需要注意《Edge 边》的定义在Graph中


        Vertex()
        {
            data = NULL;
        }
    };


    LinkList< Vertex* >  m_list;    // 使用上面定义的结构体构建（邻接链表）图



public:
    ListGraph(unsigned int n = 0)   // 指定图中有n个顶点
    {
        for(unsigned int i=0; i<n; i ++)
        {
            addVertex();
        }
    }

    int addVertex()                 // O(n)
    {
        int ret = -1;
        Vertex* v = new Vertex();

        if( NULL != v )
        {
            m_list.insert(v);           // 在最后加入顶点
            ret = m_list.length() - 1;  // 返回插入顶点的编号, 最后一个位置
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create new vertex object ...");
        }

        return ret;
    }


    int addVertex(const V& value)   // O(n)
    {
        int ret = addVertex();      // 创建顶点的同时返回 顶点编号

        if( ret >= 0 )
        {
            setVertex(ret, value);  // 设置ret位置的顶点值 为value
        }

        return ret;
    }


    bool setVertex(int i, const V& value)   // O(n)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if( ret )
        {
            Vertex* vertex = m_list.get(i);
            V* data = vertex->data;

            if( NULL == data )
            {
                data = new V();
            }

            if( NULL != data )
            {
                *data = value;
                vertex->data = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create new vertex value ...");
            }
        }

        return ret;
    }


    V getVertex(int i)                  // O(n)
    {
        V ret;

        if( !getVertex(i, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    bool getVertex(int i, V& value)     // O(n)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if( ret )
        {
            Vertex* v = m_list.get(i);  // O(n)

            if( NULL != v->data )
            {
                value = *(v->data);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex ...");
            }
        }

        return ret;
    }

    void removeVertex()                 // O(n^2)
    {
        if( m_list.length() > 0 )
        {
            int index = m_list.length() - 1;
            Vertex* v = m_list.get(index);      // 从邻接链表图中 获得要删除的顶点

            if( m_list.remove(index) )      // 首先整体删除顶点
            {
                // 删除当前图中与其他顶点 这个待删除顶点 相关的 边的信息
                for( int i=(m_list.move(0), 0); !m_list.end(); i++, m_list.next() )     // 遍历其他顶点的各自邻接链表  O(n)
                {
                    int pos = m_list.current()->edge.find(Edge<E>(i, index));           // O(n)

                    if( pos >= 0 )
                    {
                        m_list.current()->edge.remove(pos);
                        // m_list.current()->edge 当前顶点的邻接链表
                    }
                }

                delete v->data;         // 释放与删除顶点相关的数据元素值
                delete v;               // 删除图链表 自身占用的空间
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No vertex in current graph ...");
        }
    }

    SharedPointer< Array<int> > getAdjacent(int i)      // O(n)
    {
        DynamicArray<int>* ret = NULL;

        if( (0<=i) && (i < vCount()) )
        {
            Vertex* vertex = m_list.get(i);     // 获取当前图顶点

            ret = new DynamicArray<int>(vertex->edge.length());         // 在堆空间中创建返回值数组

            if( NULL != ret )
            {
                // 取出每个顶点邻接链表的第二个值(end点)
                for(int k=(vertex->edge.move(0), 0); !vertex->edge.end(); k++, vertex->edge.next())
                {
                    ret->set(k, vertex->edge.current().e);      // 数组的值设为邻接链表的end值
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object ... ");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "Index i is invalid ...");
        }

        return ret;
    }


    /* 下面开始边的操作 */
    E getEdge(int i, int j)                         // O(n)
    {
        E ret;

        if( !getEdge(i, j, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Edge <i, j> is invalid ...");
        }

        return ret;
    }
                            // E是边集类型
    bool getEdge(int i, int j, E& value)            // O(n)
    {
        bool ret = ( (0<=i) && (i<vCount()) &&
                     (0<=j) && (j<vCount()) );

        if( ret )
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));     // 当前顶点的邻接链表边中是否有到j的那条

            if( pos >= 0 )
            {
                value = vertex->edge.get(pos).data;     // 得到该边的权值
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge ...");
            }
        }

        return ret;
    }


    bool setEdge(int i, int j, const E& value)      // O(n)
    {
        bool ret = ( (0<=i) && (i<vCount()) &&
                     (0<=j) && (j<vCount()) );

        if( ret )
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if( pos >= 0 )
            {
                ret = vertex->edge.set(pos, Edge<E>(i, j, value));      // 对该邻接链表的边值进行重新设置
            }
            else
            {
                ret = vertex->edge.insert(0, Edge<E>(i, j, value));     // 原邻接链表边中没有该条边则创建插入
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j)           // O(n)
    {
        bool ret = ( (0<=i) && (i<vCount()) &&
                     (0<=j) && (j<vCount()) );

        if( ret )
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if( pos >= 0 )
            {
                ret = vertex->edge.remove(pos);
            }
        }

        return ret;
    }

    int vCount()        // O(1)
    {
        return m_list.length();
    }

    int eCount()        // O(n)
    {
        int ret = 0;

        for(m_list.move(0); !m_list.end(); m_list.next())
        {
            ret += m_list.current()->edge.length();     // 所有顶点的边的数目
        }

        return ret;
    }

    int ID(int i)       // 求顶点i的入度  // O(n^2)
    {
        int ret = 0;

        if( (0<=i) && (i<vCount()) )
        {
            for(m_list.move(0); !m_list.end(); m_list.next())
            {
                LinkList< Edge<E> >& edge = m_list.current()->edge;

                for(edge.move(0); !edge.end(); edge.next())
                {
                    if( edge.current().e == i )     // 比较顶点编号
                    {
                        ret ++;

                        break;          // 只有一条到达当前顶点的边
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    int OD(int i)           // O(n)
    {
        int ret = 0;

        if( (0<=i) && (i<vCount()) )
        {
            ret = m_list.get(i)->edge.length();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }


    // 判断在当前图中顶点i到顶点j是否邻接
    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (i < vCount())
                && (m_list.get(i)->edge.find(Edge<E>(i, j)) >= 0);
    }


    ~ListGraph()
    {
        while( m_list.length() > 0 )
        {
            Vertex* toDel = m_list.get(0);

            m_list.remove(0);

            delete toDel->data;         // 删除顶点数据元素
            delete toDel;               // 删除对应顶点
        }
    }
};


}


#endif // _LISTGRAPH_H_
