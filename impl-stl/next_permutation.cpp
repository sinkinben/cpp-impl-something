/* Impl std::next_permutation in STL
 * - From right to left, find 1st position that satisfies nums[i] < nums[i + 1].
 * - In range of [i + 1, n), find the (right-most) min-value nums[j] among the elements who are > nums[i] .
 * - Swap nums[i] and nums[j].
 * - Reverse the range [i + 1, n). 
 */
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

namespace impl
{
    void next_permutation(std::vector<int> &nums)
    {
        int n = nums.size();
        if (n < 2)
            return;
        int i = n - 2;
        while (i >= 0 && !(nums[i] < nums[i + 1]))
            --i;
        
        if (i >= 0)
        {
            int k = i + 1;
            for (int j = i + 1; j < n; ++j)
            {
                if (nums[j] > nums[i] && nums[j] <= nums[k])
                    k = j;
            }
            std::swap(nums[i], nums[k]);
        }
        std::reverse(begin(nums) + i + 1, end(nums));
    }
};

int main()
{
    int n = 8;
    std::vector<int> impl_nums(n);
    std::iota(begin(impl_nums), end(impl_nums), 1);  // fill with number from 1 to n

    auto std_nums = impl_nums;

    while (std::next_permutation(begin(std_nums), end(std_nums)))
    {
        impl::next_permutation(impl_nums);
        assert(impl_nums == std_nums);
    }

    impl::next_permutation(impl_nums);
    assert(impl_nums == std_nums);

}