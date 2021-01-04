#ifndef _SEQLIST_H_
#define _SEQLIST_H_


/* 第一个链表实现：顺序存储的线性表：顺序表 */


#include "List.h"
#include "Exception.h"


namespace JMLib
{

template < typename T >

class SeqList : public List<T>
{
protected:
    T*  m_array;    // 顺序存储空间
    int m_length;   // 当前线性表长度


public:
    bool insert(int i, const T& e)      // 增
    {
        bool ret = ( (0<=i) && (i<=m_length) );         // 注意：这里可以是 i<= m_length
        ret = ret && ( (m_length+1) <= capacity() );    // capacity()为数组的存储量

        if (ret)
        {
            for (int p=m_length-1; p>=i; p --)
            {
                m_array[p+1] = m_array[p];
            }

            m_array[i] = e;     // 插入新数据
            m_length ++;
        }

        return ret;
    }   

    bool remove(int i)                  // 删
    {
        bool ret = ( (0<=i) && (i<m_length) );

        if (ret)    // 如果判断标志位不为0
        {
            for (int p=i; p<m_length-1; p ++)
            {
                m_array[p] = m_array[p+1];
            }

            m_length --;
        }

        return ret;
    }

    bool set(int i, const T& e)         // 改
    {
        bool ret = ( (0<=i) && (i<m_length) );

        if (ret)
        {
            m_array[i] = e;
        }

        return ret;
    }

    bool get(int i, T& e) const         // 查
    {
        bool ret = ( (0<=i) && (i<m_length) );

        if (ret)
        {
            e = m_array[i];
        }

        return ret;
    }


    int find(const T& e) const      // O(n)
    {
        int ret = -1;

        for(int i=0; i<m_length; i ++)
        {
            if( m_array[i] == e )
            {
                ret = i;    // 记录下标
                break;
            }
        }

        return ret;
    }


    int length() const              // 当前线性表的长度
    {
        return m_length;
    }

    void clear()                    // 清空当前的线性表
    {
        m_length = 0;               // 清空一个线性表意味着这个线性表没有任何元素
    }

    // 顺序存储线性表的数组访问方式
    T& operator[] (int i)           // [] 操作符重载
    {
        if ( (0<=i) && (i<m_length) )
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }
    }

    T& operator[] (int i) const
    {
        return ( const_cast<SeqList<T>&>(*this)[i] );   // 去除const属性，通过[]来调用上述重载函数
    }

    // 顺序存储空间的容量
    virtual int capacity() const = 0;
};

}   // end namespace JMLib


#endif // _SEQLIST_H_
