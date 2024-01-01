/* Implement shared_ptr of STL */

#pragma once
#include <functional>
#include <memory>
#include <stdint.h>

namespace impl
{
template <class T> struct default_deleter
{
    void operator()(T *p)
    {
        delete p;
    }
};

/* Control Block for shared_ptr */
template <class T> class ctl_block
{
  public:
    int shared_cnt;
    std::function<void(T *)> deleter;
    ctl_block(std::function<void(T *)> del) : shared_cnt(1), deleter(del)
    {
    }
};

template <class T> class shared_ptr
{
  private:
    T *ptr;
    ctl_block<T> *ctl;

    void clear()
    {
        if (--(ctl->shared_cnt) == 0)
        {
            if (ptr != nullptr)
                ctl->deleter(ptr);
            delete ctl;
            ptr = nullptr, ctl = nullptr;
        }
    }

  public:
    // Default ctor and common ctor
    shared_ptr(T *p = nullptr, std::function<void(T *)> del = default_deleter<T>()) : ptr(p), ctl(new ctl_block(del))
    {
    }

    // Default dtor
    virtual ~shared_ptr()
    {
        if (ctl != nullptr && ctl->shared_cnt > 0)
            clear();
    }

    // Copy ctor
    shared_ptr(const shared_ptr<T> &sp)
    {
        ptr = sp.ptr, ctl = sp.ctl;
        if (ptr != nullptr)
            ++(ctl->shared_cnt);
    }

    // Move ctor
    shared_ptr(shared_ptr<T> &&sp)
    {
        ptr = sp.ptr, ctl = sp.ctl;
        sp.ptr = nullptr, sp.ctl = nullptr;
    }

    // Copy assignment
    shared_ptr &operator=(const shared_ptr<T> &sp)
    {
        if (this != &sp)
        {
            clear();
            ptr = sp.ptr, ctl = sp.ctl;
            if (ptr != nullptr)
                ++(ctl->shared_cnt);
        }
        return *this;
    }

    // Move assignment
    shared_ptr &operator=(shared_ptr<T> &&sp)
    {
        if (this != &sp)
        {
            clear();
            ptr = sp.ptr, ctl = sp.ctl;
            sp.ptr = nullptr, sp.ctl = nullptr;
        }
        return *this;
    }

    int use_count() const
    {
        return ctl->shared_cnt;
    }

    T *operator->() const
    {
        return ptr;
    }

    T *get() const
    {
        return ptr;
    }
};
} // namespace impl
