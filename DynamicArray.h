#ifndef _DYNAMICARRAY_H_
#define _DYNAMICARRAY_H_


#include "Array.h"
#include "Exception.h"


namespace JMLib
{

template < typename T >
class DynamicArray : public Array<T>
{
protected:
    int m_length;

    T* copy(T* array, int len, int newlen)      // O(min(len, newlen))  ==> O(n)
    {
        T* ret = new T[newlen];

        if( NULL != ret )   // 是否申请成功
        {
            int size = (len < newlen) ? len : newlen;

            for(int i=0; i<size; i ++)
            {
                ret[i] = array[i];
            }

        }

        return ret;
    }

    void update(T* array, int length)   // O(1)
    {
        if(NULL != array)
        {
            T* temp = this->m_array;

            this->m_array  = array;
            this->m_length = length;

            delete[] temp;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to update DynamicArray object ...");
        }
    }

    void init(T* array, int length)     // O(1)
    {
        if( NULL != array )
        {
            this->m_array  = array;
            this->m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to init DynamicArray object ...");
        }
    }

public:
    DynamicArray(int length=0)            // O(1)
    {
        // this->m_array = new T[length];
        init(new T[length], length);
    }

    DynamicArray(const DynamicArray<T>& obj)    // O(n)
    {
        // T* array = copy(obj.m_array, obj.m_length, obj.m_length);
        init(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);
    }

    // 重载=, 实现数组对象赋值
    DynamicArray<T>& operator= (const DynamicArray<T>& obj) // O(n)
    {
        if(this != &obj)    // 防止自赋值
        {
            // T* array = copy(obj.m_array, obj.m_length, obj.m_length);
            update(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);
        }

        return *this;
    }

    int length() const      // O(1)
    {
        return m_length;
    }

    void resize(int length)       // O(n) // 动态重置数组的长度
    {
        if( length != m_length )
        {
            // T* array = copy(this->m_array, m_length, length);
            update(copy(this->m_array, m_length, length), length);
        }
    }

    ~DynamicArray()         // O(1)
    {
        delete[] this->m_array;
    }
};

}



#endif // _DYNAMICARRAY_H_
