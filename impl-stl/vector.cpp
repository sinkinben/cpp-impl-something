/* Implement std::vector in C++11, display the usaged of placement new, std::forward and std::move */
#include <assert.h>
#include <iostream>
#include <new>
#include <vector>

namespace impl
{
template <class T> class vector
{
  private:
    T *first, *last, *end;

    void reallocate()
    {
        int cap = capacity() * 2;
        int siz = size();
        T *ptr = new T[cap];
        for (int i = 0; i < siz; ++i)
        {
            // It's similar to ptr[i] = std::move(first[i]);
            // Note that not every class will have implement of operator=(&&)
            ptr[i].~T();
            new (&ptr[i]) T(std::move(first[i]));
        }

        delete[] first;
        first = ptr, last = ptr + siz, end = ptr + cap;
    }

  public:
    explicit vector(size_t n = 16) : first(new T[n]), last(first), end(first + n)
    {
    }

    virtual ~vector()
    {
        if (first != nullptr)
            delete[] first;
    }

    void push_back(const T &val)
    {
        std::cout << "vector::push_back(&) \n";
        if (size() == capacity())
        {
            reallocate();
        }
        last->~T();
        new (last) T(val);
        ++last;
    }

    void push_back(T &&val)
    {
        std::cout << "vector::push_back(&&) \n";
        emplace_back(std::forward<T>(val));
    }

    template <typename... Args> void emplace_back(Args &&...args)
    {
        if (size() == capacity())
        {
            reallocate();
        }
        last->~T();
        new (last) T(std::forward<Args>(args)...);
        ++last;
    }

    void pop_back()
    {
        if (first < last)
        {
            --last;
            last->~T();
        }
    }

    int size() const
    {
        return std::distance(first, last);
    }
    int capacity() const
    {
        return std::distance(first, end);
    }
};
} // namespace impl

class Node
{
  public:
    int *data;
    int N;
    static int num_copies; // the number of times to copy data
    static int nums_nodes; // the number of created objects
    Node(int n = 16) : data(new int[n]), N(n)
    {
        std::cout << "Node(int) \n";
        ++nums_nodes;
    }

    Node(const Node &node)
    {
        std::cout << "Node(&) \n";
        N = node.N;
        data = new int[N];
        std::copy(node.data, node.data + N, data);
        ++num_copies, ++nums_nodes;
    }

    Node(Node &&node)
    {
        std::cout << "Node(&&) \n";
        data = node.data;
        N = node.N;
        node.data = nullptr, node.N = 0;
        ++nums_nodes;
    }

    virtual ~Node()
    {
        std::cout << "~Node() \n";
        if (data)
            delete[] data;
        --nums_nodes;
    }
};

int Node::num_copies = 0, Node::nums_nodes = 0;

int main()
{
    using impl::vector;

    auto reset = []() {
        Node::num_copies = 0;
        std::cout << "---------\n";
        assert(Node::nums_nodes == 0);
    };

    Node::num_copies = 0;
    {
        vector<Node> vec(1);      // Create one Node
        vec.push_back(Node(123)); // Create the another one
        assert(Node::num_copies == 0);
    }

    reset();
    {
        vector<Node> vec(1);
        Node node(233);
        vec.push_back(node);
        assert(Node::num_copies == 1);
    }

    reset();
    {
        vector<Node> vec(1);
        vec.emplace_back(Node(1));
        assert(Node::num_copies == 0);
    }

    reset();
    {
        vector<Node> vec(1);
        vec.emplace_back(123);
        assert(Node::num_copies == 0);
    }

    reset();
    {
        vector<Node> vec(1);
        vec.emplace_back(1);
        vec.emplace_back(2);
        vec.emplace_back(3);
        assert(Node::num_copies == 0);
    }

    reset();
    {
        // Test 'last->~T()'
        vector<int> vec(10);
        vec.push_back(123);
        vec.pop_back();
    }
}