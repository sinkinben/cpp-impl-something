#include "thread_pool.hpp"

// tasks with returned value, no arguments
void simple_test()
{
    impl::thread_pool pool(16);
    std::vector<std::future<int>> res;

    for (int i = 0; i < 100; ++i)
    {
        auto future = pool.enqueue([i]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i + 1;
        });

        res.emplace_back(std::move(future));
    }

    for (auto &f : res)
        std::cout << f.get() << '\n';
    std::cout << '\n';
}

// tasks with returned values, and arguments
void test_sorting()
{
    // sorting with multiple threads
    constexpr int N = 1e6 + 7;
    std::vector<int> nums(N);

    srand(time(nullptr));
    for (int i = 0; i < N; ++i)
        nums[i] = rand() % 100;

    constexpr size_t nr_threads = 32;
    impl::thread_pool pool(nr_threads);
    std::vector<std::future<std::pair<int, int>>> res;

    auto sorting_task = [](std::vector<int> &nums, int l, int r) {
        std::sort(nums.begin() + l, nums.begin() + r);
        return std::pair{l, r};
    };

    constexpr int step = N / nr_threads;
    for (size_t i = 0; i < nr_threads; ++i)
    {
        int l = i * step;
        int r = std::min(l + step, N);
        auto future = pool.enqueue(sorting_task, std::ref(nums), l, r);
        res.emplace_back(std::move(future));
    }

    // x.get() will wait for the completion of thread
    for (auto &f : res)
    {
        auto [l, r] = f.get();
        assert(std::is_sorted(nums.begin() + l, nums.begin() + r));
        std::printf("range l = %d, r = %d is sorted\n", l, r);
    }
}

int main()
{
    // simple_test();
    test_sorting();
}