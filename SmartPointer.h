#ifndef _SMARTPOINTER_H_
#define _SMARTPOINTER_H_

#include "Pointer.h"


namespace JMLib
{

template <typename T>
class SmartPointer : public Pointer<T>
{
public:
    SmartPointer(T* p = NULL) : Pointer<T>(p)
    {

    }

    // 完成堆空间最多只能由一个指针标识
    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer;
        const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;     // 强制类型转换，去掉const属性
    }

    // 使用引用见解析
    SmartPointer<T>& operator= (const SmartPointer<T>& obj)
    {
        if( this != &obj )  // 确保不是自赋值
        {
            T* p = this->m_pointer;

            this->m_pointer = obj.m_pointer;
            const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;

            delete p;
        }

        return *this;
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
};

}


#endif // SMARTPOINTER_H
