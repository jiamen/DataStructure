#ifndef _GTREENODE_H_
#define _GTREENODE_H_


// 通用树结点


#include "TreeNode.h"
#include "LinkList.h"

namespace JMLib
{

template < typename T >

class GTreeNode : public TreeNode<T>
{
public:
    LinkList<GTreeNode<T>*> child;     // 任意多个指向后继结点的指针


    static GTreeNode<T>* NewNode()
    {
        GTreeNode<T>* ret = new GTreeNode<T>();

        if ( NULL != ret )
        {
            ret->m_flag = true;     // 设置生成对象的标志位
        }

        return ret;
    }

};

}


#endif // _GTREENODE_H_
