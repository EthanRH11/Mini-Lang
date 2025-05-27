// #ifndef DYNAMIC_STACK_HPP
// #define DYNAMIC_STACK_HPP

// #include <cstddef>
// #include <string>
// #include <ostream>
// #include <stdexcept>

// template <typename T>
// struct Node
// {
//    T data;
//    Node<T> *next = nullptr;

//    Node(const T &v, Node<T> *next = nullptr) : data(v), next(next) {}
// };

// template <typename T>
// class Stack
// {
// public:
//    // Constructors
//    Stack();
//    ~Stack();
//    Stack &operator=(const Stack &);
//    Stack(const Stack &other);
//    Stack(Stack &&other);
//    Stack(Node<T> *);

//    // Stack operations
//    void push(const T &val); // add value to stack
//    void pop();              // remove top of stack
//    T tos();                 // return value at top of stack
//    const T top() const;

//    void clear();             // clear the stack
//    T peek(std::size_t n);    // peek at the value at the nth position
//    std::size_t size() const; // return the size of the stack
//    bool empty() const;       // is the stack empty?

//    std::string to_string() const; // turn the stack into a string (printing)
//    friend std::ostream << (std::ostream & os, const Stack<T> &);

//    bool operator==(const Stack &rhs) const;
//    bool operator!=(const Stack &rhs) const;

// private:
//    Node<T> *head = nullptr;
//    std::size_t count = 0; // increment as stack increases.
// };

// #endif // DYNAMIC_STACK_HPP

// /*

//     //Head node points to node below it in the stack.

//     [NODE]
//        |            if adding a node to the stack, you would say
//        V           new->node->down = head;
//     [NODE]         head = new;
//        |
//        V
//     [NODE]          When removing a node to the stack you would say
//        |            Node* temp = head;
//        V            head = head->next;
//     [NODE]          delete temp;
//        |
//        V

// */
