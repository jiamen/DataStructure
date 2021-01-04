#ifndef _DYNAMICLIST_H_
#define _DYNAMICLIST_H_

#include "SeqList.h"

namespace JMLib
{

template < typename T >

class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;             // 顺序存储空间的大小

public:
    DynamicList(int capacity)   // 申请空间
    {
        this->m_array = new T[capacity];        // 这里写成()了, 应该是[]。

        if (NULL != this->m_array)
        {
            // 堆空间申请成功
            this->m_length = 0;
            this->m_capacity = capacity;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory tp create DynamicList object ...");
        }
    }

    int capacity() const
    {
        return m_capacity;
    }

    void resize(int capacity)   // 重新设置存储空间的大小
    {
        if (capacity != m_capacity)
        {
            T* array = new T[capacity];

            if (NULL != array)
            {
                // ∵要对数组中的元素进行备份，所以要判断大小，哪个值小以哪个为准
                int length = (this->m_length < capacity ? this->m_length : capacity);   // 空间可能改大也可能改小, 取最小值

                for (int i=0; i<length; i ++)
                {
                    array[i] = this->m_array[i];    // 这里必须用this->m_array，不能直接用m_array，其他在父类中定义的成员同理
                }


                T* temp = this->m_array;            // 中间变量承接重置前的老的空间，后边直接释放掉
                // 不直接delete[] this->m_array, 若T是一个类，直接delete会有异常, 进一步可能导致函数从头开始执行, 后面三个赋值语句不起作用


                this->m_array    = array;           // 新空间赋值
                this->m_length   = length;
                this->m_capacity = capacity;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize DynamicList object ...");
            }
        }
    }

    ~DynamicList()
    {
        delete[] this->m_array;
    }
};


}   // end namespace JMLib

#endif // _DYNAMICLIST_H_
