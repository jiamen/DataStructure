#ifndef _LINKQUEUE_H_
#define _LINKQUEUE_H_


#include "Queue.h"
// #include "LinkList.h"
#include "LinuxList.h"
#include "Exception.h"


namespace JMLib
{

template < typename T >
class LinkQueue : public Queue<T>
{
protected:
    struct Node : public Object
    {
        list_head head;     // 链表头成员
        T value;
    };

    list_head m_header;
    int m_length;       // 记录队列长度

public:
    LinkQueue()         // O(1)
    {
        m_length = 0;

        INIT_LIST_HEAD(&m_header);
    }

    // 入队
    void add(const T& e)        // O(n) --> O(1)
    {
        Node* node = new Node();       // 遍历到最后才进行元素插入，也就是在队尾插入

        if( node != NULL )
        {
            node->value = e;
            list_add_tail(&node->head, &m_header);  // O(1)
            m_length ++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No memory to add new element ...");
        }
    }

    // 出队
    void remove()       // O(1)
    {
        // if( m_list.length() > 0 )  // 判断队列中有没有 元素
        if( m_length > 0 )
        {
            // m_list.remove(0);
            list_head* toDel = m_header.next;

            list_del(toDel);        // O(1)

            m_length --;

            delete list_entry(toDel, Node, head);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current queue ...");
        }
    }

    T front() const     // O(1)
    {
        // if( m_list.length() > 0 )
        if( m_length > 0 )
        {
            // return m_list.get(0);
            return list_entry(m_header.next, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current queue ...");
        }
    }

    void clear()        // O(n)
    {
        // m_list.clear();

        while( m_length > 0 )
        {
            remove();
        }
    }

    int length() const  // O(1)
    {
        // return m_list.length();
        return m_length;
    }

    ~LinkQueue()        // O(n)
    {
        clear();
    }
};

}


#endif // _LINKQUEUE_H_
