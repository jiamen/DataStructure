#ifndef _STATICARRAY_H_
#define _STATICARRAY_H_


#include "Array.h"


namespace JMLib
{

template < typename T, int N >
class StaticArray : public Array<T>
{
protected:
    T m_space[N];       // 存储原生数组元素

public:
    StaticArray()       // O(1)
    {
        // 将父类指针m_array挂接到子类原生数组m_space上
        this->m_array = m_space;
    }

    // 拷贝构造函数和赋值操作
    StaticArray(const StaticArray<T, N>& obj)   // O(N)
    {
        this->m_array = m_space;

        for(int i=0; i<N; i ++)
        {
            m_space[i] = obj.m_space[i];
        }
    }

    StaticArray<T, N>& operator= (const StaticArray<T, N>& obj) // O(N)
    {
        if( this != &obj )  // 防止自赋值
        {
            for(int i=0; i<N; i ++)
            {
                m_space[i] = obj.m_space[i];
            }
        }

        return *this;
    }

    int length() const  // O(1)
    {
        return N;
    }
};

}


#endif // _STATICARRAY_H_
