#include "bst.hpp"
#include <iostream>
#include <assert.h>

int main()
{
    auto is_valid = [](const std::vector<int> &seq) {
        int n = seq.size();
        for (int i = 0; i + 1 < n; ++i)
        {
            if (!(seq[i] < seq[i + 1]))
                return false;
        }
        return true;
    };

    constexpr int N = 1024;
    impl::bstree bst;

    // test 'insert'
    for (int i = 0; i < N; ++i)
    {
        bst.insert(random() % 114514);
        assert(is_valid(bst.flattern()));
    }

    auto seq = bst.flattern();

    // test 'search', existent values
    for (int val : seq)
        assert(bst.search(val)->val == val);

    // non-existent values
    assert(bst.search(114515) == nullptr);
    assert(bst.search(-114515) == nullptr);

    // test 'remove'
    for (int val : seq)
    {
        bst.remove(val);
        assert(is_valid(bst.flattern()));
    }
    
    assert(bst.root == nullptr);
}