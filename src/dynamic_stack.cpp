// #include "dynamic_stack.hpp"
// #include "ErrorHandler.hpp"

// template <typename T>
// Stack<T>::Stack()
// {
//     head = new Node<T>(T{});
//     count = 1;
// }

// template <typename T>
// Stack<T>::~Stack() { clear(); }

// template <typename T>
// Stack<T>::Stack(const Stack &other)
// {
//     if (!other.head)
//         return;

//     Node<T> *src = other.head;
//     Node<T> *prev = nullptr;
//     Node<T> *newHead = nullptr;

//     while (src)
//     {
//         Node<T> *newHead = new Node<T>(src->data);
//         if (!newHead)
//         {
//             newHead = newNode;
//         }
//         else
//         {
//             prev->next = newNode;
//         }
//         prev = newNode;
//         src = src->next;
//     }
//     head = newHead;
//     count = other.count;
// }

// template <typename T>
// Stack<T> &Stack<T>::operator=(const Stack &rhs)
// {
//     if (this != rhs)
//     {
//         clear();
//         Stack temp(rhs);
//         std::swap(head, temp.head);
//         std::swap(count, temp.count);
//     }
//     return *this;
// }

// template <typename T>
// Stack<T>::Stack(Node<T> *headNode)
// {
//     head = headNode;

//     count = 0;
//     while (headNode)
//     {
//         count++;
//         headNode = headNode->next;
//     }
// }

// template <typename T>
// void Stack<T>::push(const T &val)
// {
//     head = new Node<T>(val, head);
//     ++count;
// }

// template <typename T>
// void Stack<T>::pop()
// {
//     if (empty())
//     {
//         ErrorHandler::getInstance().reportRuntimeError("Stack underflow");
//     }

//     Node<T> *temp = head;
//     head = head->next;
//     delete temp;
//     --count;
// }

// template <typename T>
// T Stack<T>::tos()
// {
//     if (empty())
//     {
//         ErrorHandler::getInstance.reportRuntimeError("Stack is empty");
//     }
//     return head->data;
// }

// template <typename T>
// const T Stack<T>::top() const
// {
//     if (empty())
//     {
//         ErrorHandler::getInstance.reportRuntimeError("Stack is empty");
//     }
//     return head->data;
// }

// template <typename T>
// void Stack<T>::clear()
// {
//     while (!empty())
//     {
//         pop();
//     }
// }

// template <typename T>
// T Stack<T>::peek(std::size_t n)
// {
//     if (n >= count)
//     {
//         ErrorHandler::getInstance.reportRuntimeError("Index out of range.");
//     }

//     Node<T> *curr = head;
//     for (std::size_t i = 0; i < n; ++i)
//     {
//         curr = curr->next;
//     }
//     return curr->data;
// }

// template <typename T>
// std::size_t Stack<T>::size() const
// {
//     return count;
// }

// template <typename T>
// bool Stack<T>::empty() const
// {
//     return count == 0;
// }

// template <typename T>
// std::string Stack<T>::to_string() const
// {
//     std::ostringstream os;
//     Node<T> *curr = head;
//     while (curr)
//     {
//         os << curr->data << " ";
//         curr = curr->next;
//     }
//     return os.str();
// }

// template <typename T>
// bool Stack<T>::operator==(const Stack<T> &rhs) const
// {
//     if (count != rhs.count)
//         return false;

//     Node<T> *l = head;
//     Node<T> *r = rhs.head;
//     while (l && r)
//     {
//         if (l->data != r->data)
//             return false;
//         l = l->next;
//         r = r->next;
//     }
//     return true;
// }

// template <typename T>
// bool Stack<T>::operator!=(const Stack<T> &rhs) const
// {
//     return !(*this == rhs);
// }

// template <typename T>
// std::ostream &operator<<(std::ostream &os, const Stack<T> &stack)
// {
//     Node<T> *curr = stack.head;
//     os << "[";
//     while (curr)
//     {
//         os << curr->data;
//         if (curr->next)
//         {
//             out << ", ";
//         }
//         curr = curr->next;
//     }
//     os << "]";
//     return os;
// }