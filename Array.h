#ifndef _ARRAY_H_
#define _ARRAY_H_


#include "Object.h"
#include "Exception.h"


namespace JMLib
{

template < typename T >
class Array : public Object
{
protected:
    T* m_array;     // 原生数组首地址

public:
    virtual bool set(int i, const T& e)  // 时间复杂度分析 最复杂和最简单 O(1)
    {
        bool ret = ((0 <= i) && (i < length()));

        if( ret )
        {
            m_array[i] = e;
        }

        return ret;
    }

    virtual bool get(int i, T& e) const  // O(1)
    {
        bool ret = ((0 <= i) && (i < length()));

        if( ret )
        {
            e = m_array[i];
        }

        return ret;
    }


    // 数组访问操作符
    T& operator[] (int i)       // O(1)
    {
        if((0 <= i) && (i < length()))
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }
    }


    T operator [](int i) const  // O(1)
    {
        return (const_cast<Array<T>&>(*this))[i];
    }


    T* array() const        // 返回原生数组首地址，根据第50课内容添加
    {
        return m_array;
    }


    virtual int length() const = 0;    // 纯虚函数
};

}


#endif // _ARRAY_H_
