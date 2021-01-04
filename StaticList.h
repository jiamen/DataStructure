#ifndef _STATICLIST_H_
#define _STATICLIST_H_


#include "SeqList.h"

namespace JMLib
{

template <typename T, int N>

class StaticList : public SeqList<T>
{
protected:
    T m_space[N];           // 顺序存储空间, N 为模板参数

public:
    StaticList()
    {
        this->m_space  = m_space;   // 指定一个最大容量为5的数组空间的位置
        this->m_length = 0;         // 目前数组内长度是0
    }

    int capacity() const
    {
        return N;
    }
};

}

#endif // _STATICLIST_H_
