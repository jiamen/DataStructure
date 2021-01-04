#ifndef _STATICSTACK_H_
#define _STATICSTACK_H_

#include "Stack.h"
#include "Exception.h"


namespace JMLib
{

template < typename T, int N >
class StaticStack : public Stack<T>
{
protected:
    T m_space[N];   // 缺陷： 当泛指类型为类类型时，会连续多次调用类的构造函数
    int m_top;      // 当前栈的栈顶
    int m_size;     // 当前栈的元素数量

public:
    StaticStack()           // O(1)
    {
        m_top = -1;
        m_size = 0;
    }

    int capacity() const    // O(1)
    {
        return N;
    }

    void push(const T& e)   // O(1)
    {
        if( m_size < N )
        {
            m_space[m_top + 1] = e;     // 赋值成功后再计数+1
            m_top ++;
            m_size ++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No space in current stack ...");
        }
    }

    void pop()          // O(1)
    {
        if( m_size > 0 )
        {
            m_top --;
            m_size --;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
        }
    }

    T top() const       // O(1)
    {
        if( m_size > 0 )
        {
            return m_space[m_top];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
        }
    }

    void clear()        // O(1)
    {
        m_top  = -1;
        m_size = 0;
    }

    int size() const    // O(1)
    {
        return m_size;
    }
};

}


#endif // _STATICSTACK_H_
