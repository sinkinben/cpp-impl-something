/* Implement shared_ptr of STL */

#pragma once
#include <stdint.h>

namespace impl
{
template<class T>
struct default_deleter
{
    void operator()(T *p) { delete p; }
};

template<class T>
class shared_ptr
{
    private:
        T *ptr;
        uint32_t *ref_cnt;
        std::function<void(T *)> deleter;

        void clear()
        {
            if (--(*ref_cnt) == 0)
            {
                if (ptr != nullptr)
                    deleter(ptr);
                delete ref_cnt;
                ptr = nullptr, ref_cnt = nullptr;
            }
        }
    public:
        // Default ctor and common ctor
        shared_ptr(T *p = nullptr, std::function<void(T*)> del = default_deleter<T>()) : ptr(p), ref_cnt(new uint32_t{1}), deleter(del) {}

        // Default dtor
        virtual ~shared_ptr()
        {
            if (ref_cnt != nullptr && *ref_cnt != 0)
                clear();
        }

        // Copy ctor
        shared_ptr(const shared_ptr<T> &sp)
        {
            ptr = sp.ptr, ref_cnt = sp.ref_cnt, deleter = sp.deleter;
            if (ptr != nullptr)
                ++(*ref_cnt);
        }

        // Move ctor
        shared_ptr(shared_ptr<T> &&sp)
        {
            ptr = sp.ptr, ref_cnt = sp.ref_cnt, deleter = sp.deleter;
            sp.ptr = nullptr, sp.ref_cnt = nullptr;
        }

        // Copy assignment
        shared_ptr& operator=(const shared_ptr<T> &sp)
        {
            if (this != &sp)
            {
                clear();
                ptr = sp.ptr, ref_cnt = sp.ref_cnt, deleter = sp.deleter;
                if (ptr != nullptr)
                    ++(*ref_cnt);
            }
            return *this;
        }

        // Move assignment
        shared_ptr& operator=(shared_ptr<T> &&sp)
        {
            if (this != &sp)
            {
                clear();
                ptr = sp.ptr, ref_cnt = sp.ref_cnt, deleter = sp.deleter;
                sp.ptr = nullptr, sp.ref_cnt = nullptr;
            }
            return *this;
        }

        uint32_t use_count() const { return *ref_cnt; }
        T* operator->() const { return ptr; }
        T* get() const { return ptr; }
};
}
