#ifndef DYNAMIC_STACK_HPP
#define DYNAMIC_STACK_HPP

template <typename T>
class Node
{
public:
private:
    T *data;
    Node *
};

template <typename T>
class Stack
{
public:
    Stack();
    ~Stack();
    Stack &operator=(const Stack &);
    Stack(const Stack &other);
    Stack(Stack &&other);

private:
};

#endif // DYNAMIC_STACK_HPP

/*

    //Head node points to node below it in the stack.

    [NODE]
       |            if adding a node to the stack, you would say
       V           new->node->down = head;
    [NODE]         head = new;
       |
       V
    [NODE]          When removing a node to the stack you would say
       |            Node* temp = head;
       V            head = head->next;
    [NODE]          delete temp;
       |
       V

*/
