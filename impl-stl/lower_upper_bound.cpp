/* Impl std::lower_bound and std::upper_bound in STL */

#include <algorithm>
#include <assert.h>
#include <vector>

namespace impl
{
int lower_bound(std::vector<int> &vec, int key)
{

    int l = 0, r = vec.size();
    // Binary search in range of [l, r)
    while (l < r)
    {
        int m = l + (r - l) / 2;
        if (key <= vec[m])
            r = m;
        else if (key > vec[m])
            l = m + 1;
    }
    return l;
}

int upper_bound(std::vector<int> &vec, int key)
{
    int l = 0, r = vec.size();
    while (l < r)
    {
        int m = l + (r - l) / 2;
        if (key < vec[m])
            r = m;
        else if (key >= vec[m])
            l = m + 1;
    }
    return l;
}
}; // namespace impl

int main()
{
    auto std_lower_bound = [](std::vector<int> &nums, int key) {
        return std::distance(begin(nums), std::lower_bound(begin(nums), end(nums), key));
    };

    auto std_upper_bound = [](std::vector<int> &nums, int key) {
        return std::distance(begin(nums), std::upper_bound(begin(nums), end(nums), key));
    };

    // tests which are simple and easy to read
    {
        std::vector<int> nums = {1, 1, 3, 3, 3, 5, 8, 9};

        // non-existent number
        assert(std_lower_bound(nums, 0) == 0 && impl::lower_bound(nums, 0) == 0);
        assert(std_upper_bound(nums, 123) == nums.size() && impl::upper_bound(nums, 123) == nums.size());
        assert(std_lower_bound(nums, 4) == 5 && impl::lower_bound(nums, 4) == 5);
        assert(std_upper_bound(nums, 4) == 5 && impl::upper_bound(nums, 4) == 5);

        // duplicate number
        assert(std_lower_bound(nums, 3) == 2 && impl::lower_bound(nums, 3) == 2);
        assert(std_upper_bound(nums, 3) == 5 && impl::upper_bound(nums, 3) == 5);

        // unique number
        assert(std_lower_bound(nums, 5) == 5 && impl::lower_bound(nums, 5) == 5);
        assert(std_upper_bound(nums, 5) == 6 && impl::upper_bound(nums, 5) == 6);
    }

    // random tests with duplicate
    {
        srand(time(nullptr));
        int n = 1024;
        std::vector<int> nums(n);
        for (int i = 0; i < n; ++i)
            nums[i] = random() % (n / 2);

        std::sort(begin(nums), end(nums));

        for (int i = 0; i < n; ++i)
        {
            assert(std_lower_bound(nums, i) == impl::lower_bound(nums, i));
            assert(std_upper_bound(nums, i) == impl::upper_bound(nums, i));
        }
    }

    // common random tests
    {
        srand(time(nullptr));
        int n = 2048;
        std::vector<int> nums(n);
        for (int i = 0; i < n; ++i)
            nums[i] = random();

        for (int i = 0; i < n; ++i)
        {
            int j = random();
            assert(std_lower_bound(nums, j) == impl::lower_bound(nums, j));
            assert(std_upper_bound(nums, j) == impl::upper_bound(nums, j));
        }
    }
}