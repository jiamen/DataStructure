#ifndef _DUALLINKLIST_H_
#define _DUALLINKLIST_H_


namespace JMLib
{

template < typename T >

class DualLinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
        Node* pre;
    };

    mutable struct : public Object
    {
        char reserved[sizeof(T)];
        Node* next;
        Node* pre;
    } m_header;

    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const
    {
        // Node* ret = ;
    }
};

}


#endif // _DUALLINKLIST_H_
