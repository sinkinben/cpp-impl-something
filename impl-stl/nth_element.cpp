/* Impl std::nth_element in C++ 11, refer to https://en.cppreference.com/w/cpp/algorithm/nth_element */
#include <assert.h>
#include <iostream>
#include <vector>

namespace impl
{
// The range of 'n' is [0, seq.size)
template <class T> void nth_element(std::vector<T> &seq, int n)
{
    auto partition = [&](int l, int r) -> int {
        int key = seq[r];
        int i = l - 1;
        for (int j = l; j < r; ++j)
        {
            if (seq[j] < key)
                std::swap(seq[++i], seq[j]);
        }
        std::swap(seq[++i], seq[r]);
        return i;
    };

    int p = -1;
    int l = 0, r = seq.size() - 1;
    while ((p = partition(l, r)) != n)
    {
        if (p < n)
            l = p + 1;
        else
            r = p - 1;
    }
}
} // namespace impl

int main()
{
    constexpr int N = 128;
    srand(114514);
    std::vector<int> nums(N);
    for (int i = 0; i < N; ++i)
        nums[i] = random();

    // Exectue tests for 100 times
    for (int i = 0; i < 100; ++i)
    {
        auto seq_impl = nums, seq_std = nums;
        int n = random() % N;
        std::nth_element(begin(seq_std), begin(seq_std) + n, end(seq_std));
        impl::nth_element(seq_impl, n);
        assert(seq_impl[n] == seq_std[n]);
    }
}