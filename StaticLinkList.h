#ifndef _STATICLINKLIST_H_
#define _STATICLINKLIST_H_

#include "LinkList.h"


namespace JMLib
{

template < typename T, int N >
class StaticLinkList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;

    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* loc)
        {
            (void)size;
            return loc;
        }
    };

    unsigned char m_space[sizeof(Node) * N];
    int m_used[N];

    Node* create()
    {
        Node* ret = NULL;

        for(int i=0; i<N; i ++)
        {
            if( !m_used[i] )        // 找到可用的地址空间
            {
                ret = reinterpret_cast<Node*>(m_space) + i;
                // 这里只是分配内存，假如是对象，没有对象构造的过程
                ret = new(ret)SNode();
                m_used[i] = 1;
                break;
            }
        }

        return ret;
    }

    void destroy(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn);

        for(int i=0; i<N; i ++)
        {
            if( psn == (space + i) )
            {
                m_used[i] = 0;
                psn->~SNode();
                break;
            }
        }
    }

public:
    StaticLinkList()
    {
        for(int i=0; i<N; i ++)
        {
            m_used[i] = 0;
        }
    }

    int capacity()
    {
        return N;
    }


    ~StaticLinkList()
    {
        this->clear();   // 析构函数和构造函数中无法实现多态, 这里的clear是父类实现的clear，
                         // 在clear中实现destroy()函数 的多态
    }
};


}


#endif // _STATICLINKLIST_H_
