#include "shared_ptr.hpp"
#include <assert.h>
#include <iostream>

class Node
{
  public:
    Node()
    {
        std::cout << "Node()\n";
    }
    ~Node()
    {
        std::cout << "~Node()\n";
    }
    void print()
    {
        std::cout << "Node::print()\n";
    }
};

void test_shared_ptr()
{
    // using std::shared_ptr;
    using impl::shared_ptr;

    // Case 1: allocate a object
    shared_ptr<Node> node(new Node());

    // Case 1.1: allocate a object and customize deleter with functor
    struct customed_deleter
    {
        void operator()(Node *node)
        {
            std::cout << "customed_deleter \n";
            delete node;
        }
    };
    shared_ptr<Node> node2(new Node(), customed_deleter());

    // Case 2: allocate an array of class Node, customize deleter with lambda
    shared_ptr<Node> arr(new Node[3], [](Node *p) { delete[] p; });
    assert(arr.use_count() == 1);

    // Case 3: init with copy ctor
    shared_ptr<Node> arr2 = arr;
    assert(arr.use_count() == 2 && arr2.use_count() == 2);

    // Case 4: init with move ctor
    shared_ptr<Node> arr3 = std::move(arr2);
    assert(arr.use_count() == 2 && arr3.use_count() == 2 && arr2.get() == nullptr);

    // Case 5: assign with copy operator =
    shared_ptr<Node> arr4;
    arr4 = arr;
    assert(arr.use_count() == 3 && arr3.use_count() == 3 && arr4.use_count() == 3);

    // Case 6: assign with move operator =
    arr4 = std::move(arr);
    assert(arr.get() == nullptr && arr3.use_count() == 2 && arr4.use_count() == 2);
    assert(arr3.get() == arr4.get());

    // Case 7: use as a common pointer
    node->print();
    for (int i = 0; i < 3; ++i)
        arr3.get()[i].print();
}

int main()
{
    test_shared_ptr();
    return 0;
}