/* Implement heap operations in STL.
 * - std::make_heap, std::push_heap, std::pop_heap
 * - std::priority_queue
 */

#include <assert.h>
#include <queue>
#include <vector>

namespace impl
{
// O(log{n}) time, heapify from top (the 'idx' position) to bottom (the leaf position)
void heapify(std::vector<int> &nums, int idx)
{
    int n = nums.size();
    int l = 2 * idx + 1;
    int r = l + 1;
    int largest = idx;
    // max heap
    if (l < n && nums[l] > nums[largest])
        largest = l;
    if (r < n && nums[r] > nums[largest])
        largest = r;

    if (largest != idx)
    {
        std::swap(nums[largest], nums[idx]);
        heapify(nums, largest);
    }
}

// \SUM{log{i}} = O(n)
void make_heap(std::vector<int> &nums)
{
    // [n / 2, n) is ranged of the leaf nodes in the heap
    for (int i = nums.size() / 2; i >= 0; --i)
        heapify(nums, i);
}

// Add element into heap, O(log{n}) time
void push_heap(std::vector<int> &heap, int val)
{
    int child, parent, n;
    heap.emplace_back(val);

    // adjust heap[n - 1] from bottom to top
    n = heap.size();
    child = n - 1;
    parent = (child - 1) / 2;

    // max heap
    while (child > 0 && !(heap[child] < heap[parent]))
    {
        std::swap(heap[child], heap[parent]);
        child = parent;
        parent = (child - 1) / 2;
    }
}

// Pop the largest number in 'heap', that is 'heap[0]', O(log{n}) time
int pop_heap(std::vector<int> &heap)
{
    if (heap.empty())
        return -1;
    int n = heap.size();
    std::swap(heap[0], heap[n - 1]);
    int res = heap.back();
    heap.pop_back();
    heapify(heap, 0);
    return res;
}

class priority_queue
{
  public:
    std::vector<int> max_heap;
    priority_queue() = default;
    priority_queue(const priority_queue &pq)
    {
        max_heap = pq.max_heap;
    }
    explicit priority_queue(const std::vector<int> nums)
    {
        max_heap = nums;
        make_heap(max_heap);
    }

    void push(int val)
    {
        push_heap(max_heap, val);
    }

    int pop()
    {
        return pop_heap(max_heap);
    }

    int top()
    {
        return max_heap.empty() ? -1 : max_heap[0];
    }

    size_t size()
    {
        return max_heap.size();
    }
};
}; // namespace impl

void assert_compare(impl::priority_queue pq_impl, std::priority_queue<int> pq_std)
{
    assert(pq_std.size() == pq_impl.size());
    while (!pq_std.empty())
    {
        int x, y;
        x = pq_std.top(), pq_std.pop();
        y = pq_impl.pop();
        assert(x == y);
    }
    assert(pq_std.empty() && pq_impl.max_heap.empty());
}

int main()
{
    impl::priority_queue pq_impl;
    std::priority_queue<int> pq_std;


    // Test push and pop, and compare with std::priority_queue
    srand(114514);
    std::vector<int> nums;
    for (int i = 0; i < 1000; ++i)
    {
        int val = random();
        pq_impl.push(val), pq_std.push(val);
        assert_compare(pq_impl, pq_std);

        nums.emplace_back(val);
    }

    // Test impl::make_heap
    assert_compare(impl::priority_queue(nums), std::priority_queue<int>(nums.begin(), nums.end()));
}