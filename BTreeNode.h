#ifndef _BTREENODE_H_
#define _BTREENODE_H_


#include "TreeNode.h"


enum BTNodePos
{
    ANY,
    LEFT,
    RIGHT
};


namespace JMLib
{

template < typename T >

class BTreeNode : public TreeNode<T>
{
public:
    BTreeNode<T>* left;
    BTreeNode<T>* right;

    BTreeNode()
    {
        left  = NULL;
        right = NULL;
    }


    // 从堆空间创建变量
    static BTreeNode<T>* NewNode()
    {
        BTreeNode<T>* ret = new BTreeNode<T>();

        if ( NULL != ret )
        {
            ret->m_flag = true;
        }

        return ret;
    }
};

}


#endif // _BTREENODE_H_
