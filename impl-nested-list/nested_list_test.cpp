#include "nested_list.hpp"

impl::nested_list make_list(int l, int r)
{
    impl::nested_list list;
    for (int i = r; i >= l; --i)
        list.push_front(i);
    return list;
}

int main()
{
    {
        impl::nested_list l1 = make_list(1, 5), l2 = make_list(6, 10);

        l1.display();
        l2.display();

        l2.push_front(l1);
        l2.display();

        impl::nested_list l3;
        l3.push_front(l2);
        l3.display();

        l3.push_front(233);
        l3.display();

        auto vec = l3.flattern();
        for (uint64_t x : vec)
            std::cout << x << " ";
        std::cout << "\n";
    }
    assert(impl::nested_node::num_nodes == 0);
}
