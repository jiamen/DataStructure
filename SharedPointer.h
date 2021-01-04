#ifndef _SHAREDPOINTER_H_
#define _SHAREDPOINTER_H_


#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"


namespace JMLib
{

template < typename T >
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if( this->m_ref )
        {
            (*this->m_ref) ++;
        }
    }

public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if( p )
        {
            this->m_ref = static_cast<int*>( (std::malloc(sizeof(int))) );

            if( this->m_ref )
            {
                *(this->m_ref)  = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create SharedPointer object ...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)
    {
        assign(obj);
    }

    SharedPointer<T>& operator= (const SharedPointer<T>& obj)
    {
        if( this != &obj )
        {
            clear();    // 先置空，保证当前指针不指向任一空间

            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;         // 准备清空本次指针计数，用ref接管原计数空间

        this->m_pointer = NULL;
        this->m_ref = NULL;

        if( ref )
        {
            (*ref) --;  // 当前指针不指向该堆空间了，那么原堆空间计数指针应该减一

            if( *ref == 0 )     // 进一步判断，已经没有指针指向关联的堆空间了，可以直接释放该堆空间
            {
                free(ref);      // 对应前面的malloc函数
                delete toDel;   // 释放对应堆空间
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};


template < typename T >
bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.get() == r.get());
}


template < typename T >
bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !(l == r);
}

}


#endif // _SHAREDPOINTER_H_
