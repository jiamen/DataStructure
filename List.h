#ifndef _LIST_H_
#define _LIST_H_


#include "Object.h"


namespace JMLib
{

template < typename T >

class List : public Object
{
protected:
    List(const List&);
    List& operator= (const List&);

public:
    List() {  }
    // 均为纯虚函数
    virtual bool insert(int i, const T& e) = 0;     // 增
    virtual bool remove(int i) = 0;                 // 删
    virtual bool set(int i, const T& e) = 0;        // 改
    virtual bool get(int i, T& e) const = 0;        // 查

    virtual int  find(const T& e) const = 0;        // 查找指定的数据元素


    virtual int  length() const = 0;                // 获得链表长度
    virtual void clear() = 0;                       // 清空链表
};


}   // end namespace JMLib


#endif // LIST_H
