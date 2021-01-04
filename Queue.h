#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Object.h"

namespace JMLib
{

template < typename T >
class Queue : public Object
{
public:
    virtual void add(const T& e) = 0;
    virtual void remove() = 0;
    virtual T front() const = 0;        // 获取队头元素
    virtual void clear() = 0;
    virtual int length() const = 0;     // 获取队列长度
};

}


#endif // _QUEUE_H_
