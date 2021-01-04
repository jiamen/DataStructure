#ifndef _CIRCLELIST_H_
#define _CIRCLELIST_H_


#include "LinkList.h"

namespace JMLib
{

template < typename T >
class CircleList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;

    int mod(int i) const        // O(1)
    {
        return (this->m_length == 0) ? 0 : (i%this->m_length);
    }

    Node* last() const          // O(n)
    {
        return this->position(this->m_length-1)->next;
    }

    void last_to_first() const  // O(n)
    {
        last()->next = this->m_header.next;
    }


public:
    bool insert(const T& e)     // O(n)
    {
        return insert(this->m_length, e);
    }

    bool insert(int i, const T& e)          // O(1)+O(n)+O(n) ==> O(2n+1) ==> O(n)
    {
        bool ret = true;

        i = i % (this->m_length + 1);       // O(1)
        ret = LinkList<T>::insert(i, e);    // O(n)

        if( ret && (i==0) )     // 判断是否为收尾相连
        {
            last_to_first();                // O(n)
        }

        return ret;
    }

    bool remove(int i)      // 注意删除位置为0的情况 O(n)
    {
        bool ret = true;

        i = mod(i);

        if( 0 == i )        // 确定在队首
        {
            Node* toDel = this->m_header.next;

            if( NULL != toDel )
            {
                this->m_header.next = toDel->next;
                this->m_length --;

                if( this->m_length > 0 )
                {
                    last_to_first();            // O(n)

                    if( this->m_current == toDel )
                    {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }

                this->destroy(toDel);
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(i);       // O(n)
        }

        return ret;
    }

    bool set(int i, const T& e)     // O(n)
    {
        return LinkList<T>::set(mod(i), e);
    }

    T get(int i) const              // O(n)
    {
        return LinkList<T>::get(mod(i));
    }

    T get(int i, const T& e)
    {
        return LinkList<T>::get(mod(i), e);
    }

    int find(const T& e) const      // O(n)
    {
        int ret = -1;

        Node* slider = this->m_header.next;

        for(int i=0; i<this->m_length; i ++)
        {
            if( slider->value == e )
            {
                ret = i;
                break;
            }

            slider = slider->next;
        }

        // last()->next = NULL;            // 尾结点的next指针置空，将循环链表变为单链表
        // ret = LinkList<T>::find(e);     // 对象比较需要重载 == 比较操作符
        // last_to_first();

        return ret;
    }

    void clear()        // O(n)
    {
        /*if( this->m_length > 0 )
        {
            last()->next = NULL;
            LinkList<T>::clear();    // 如果clear()函数发生异常，无法保证当前链表还是循环链表
        }*/
        while( this->m_length > 1 )  // O(n)
        {
            remove(1);               // 看上面的remove()函数分支 O(n)
        }

        if( this->m_length == 1 )    // O(1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = NULL;
            this->m_length  = 0;
            this->m_current = NULL;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step=1)    // O(n)
    {
        return LinkList<T>::move(mod(i), step);
    }

    bool end()      // O(1)
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }

    ~CircleList()   // O(n)
    {
        clear();
    }
};


}

#endif // _CIRCLELIST_H_
