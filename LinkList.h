#ifndef _LINKLIST_H_
#define _LINKLIST_H_

/*
 * 第二个链表实现： 单链表的实现
 */

#include "List.h"
#include "Exception.h"
#include "SmartPointer.h"

using namespace std;


namespace JMLib
{

template < typename T >
class LinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
        //SmartPointer<Node> next;
    };

    mutable struct : public Object  // bool get(int i, T& e) const 函数中可能会影响这个值, 所以使用mutable
    {
        char  reserved[sizeof(T)];  // 仅仅为了占用空间，保证 头结点内存空间结构 与 Node结点空间结构类似
        Node* next;
        // SmartPointer<Node> next;
    } m_header;

    int m_length;
    int m_step;         // 游标每次移动的结点的数目
    // SmartPointer<Node> m_current;    // 游标
    Node* m_current;


    Node* position(int i) const     // O(n)
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);     // 新型类型转换，处理无关类型之间的转换
        // SmartPointer<Node> ret = reinterpret_cast<Node*>(&m_header);

        for(int p=0; p<i; p ++)
        {
            ret = ret->next;
        }

        return ret;     // ret.get()
    }

    virtual Node* create()
    {
        return new Node();
    }

    virtual void destroy(Node* pn)
    {
        delete pn;
    }


public:
    LinkList()
    {
        m_header.next = NULL;
        m_length = 0;
        m_step = 0;
        m_current = NULL;
    }


    bool insert(const T& e)
    {
        return insert(m_length, e);
    }

    bool insert(int i, const T& e)      // O(n)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        if( ret )
        {
            Node* node = create();

            if( NULL != node )
            {
                Node* current = position(i);

                node->value = e;
                node->next = current->next;
                current->next = node;

                m_length ++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert new element ...");
            }
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }

        return ret;
    }

    bool remove(int i)          // O(n)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        if( ret )
        {
            Node* current = position(i);
            Node* toDel = current->next;
            // SmartPointer<Node> current = position(i);
            // SmartPointer<Node> toDel   = current->next;

            if( m_current == toDel )
            {
                m_current = toDel->next;
            }

            current->next = toDel->next;

            m_length --;

            destroy(toDel);       // .get() 智能指针，这里可以不用
        }
        else
        {
            printf("not2222!!!");
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }

        return ret;
    }

    bool set(int i, const T& e)     // O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            // Node* current = position(i);
            position(i)->next->value = e;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }

        return ret;
    }

    T get(int i) const              // O(n)
    {
        T ret;

        if( get(i, ret))
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i to get element ...");
        }

        return ret;
    }

    bool get(int i, T& e) const     // O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            // Node* current = position(i);
            e = position(i)->next->value;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }

        return ret;
    }

    int find(const T& e) const     // 发现当前值为 e 的节点所处的链表 位置 i ；
    {
        int ret = -1;
        int i = 0;
        Node* node = m_header.next;
        // SmartPointer<Node> node = m_header.next;

        while( node )        // O(n)
        // while( node.isNull() )
        {
            if(node->value == e)
            {
                ret = i;
                break;
            }
            else
            {
                node = node->next;
                i++;
            }
        }

        return ret;
    }

    int  length() const     // O(1)
    {
        return m_length;
    }

    void clear()            // O(n)
    {
        while( m_header.next )
        // while( m_header.next.isNull() )
        {
            Node* toDel = m_header.next;
            // SmartPointer<Node> toDel = m_header.next;

            m_header.next = toDel->next;

            m_length --;
            destroy(toDel);     // toDel.get()
        }

        // m_length = 0;

        m_step = 0;     // 这一句是自己加的☆
    }



    //  4 个函数降低单链表 遍历 时间复杂度   ///////////////////////////////////////////////////
    bool move(int i, int step = 1)
    {
        bool ret = (0 <= i) && (i <= m_length) && (step > 0);

        if( ret )
        {
            m_current = position(i)->next;
            m_step = step;              // 保存每次移动的结点数目
        }
        else
        {
            cout << "ret = " << ret << endl;
            printf("not3333!!!");
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }


        return ret;
    }

    bool end()
    {
        return (m_current == NULL);     // 是结尾返回1，不是结尾返回0
        // return m_current.isNull();
    }

    T current()
    {
        if( !end() )
        {
            return m_current->value;
        }
        else
        {
            printf("not4444!!!");
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }
    }

    bool next()
    {
        int i = 0;

        while( (i<m_step) && !end() )
        {
            m_current = m_current->next;
            i ++;
        }

        return (i == m_step);
    }
    // ////////////////////////////////////////////////////////////


    ~ LinkList()            // O(n)
    {
        clear();

        m_length = 0;
        m_step = 0;
    }
};

}


#endif // _LINKLIST_H_
