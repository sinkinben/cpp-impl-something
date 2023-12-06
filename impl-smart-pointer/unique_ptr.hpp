#pragma once
#include "shared_ptr.hpp" // using the default_deleter

#include <algorithm>
#include <cassert>
namespace impl
{
template <class T, class Deleter = default_deleter<T>> class unique_ptr
{
  private:
    T *ptr;
    Deleter deleter;

    void clear()
    {
        if (ptr != nullptr)
        {
            deleter(ptr);
            ptr = nullptr;
        }
    }

  public:
    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    // dtor
    virtual ~unique_ptr()
    {
        clear();
    }

    // Default ctor and common ctor
    explicit unique_ptr(T *p = nullptr, Deleter del = default_deleter<T>()) : ptr(p), deleter(del)
    {
    }

    // std::move ctor
    // Can not declare it with 'explicit', see case-1 of 'unique_ptr_test'
    unique_ptr(unique_ptr<T> &&up) noexcept
    {
        ptr = up.ptr, deleter = up.deleter;
        up.ptr = nullptr;
    }

    unique_ptr &operator=(unique_ptr &&up)
    {
        clear();
        ptr = up.ptr, deleter = up.deleter;
        up.ptr = nullptr;
        return *this;
    }

    T *get() const
    {
        return ptr;
    }

    T *operator->() const
    {
        return ptr;
    }

    explicit operator bool() const noexcept
    {
        return ptr != nullptr;
    }
};
} // namespace impl