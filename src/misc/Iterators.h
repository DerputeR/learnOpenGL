#pragma once
#include <iterator>
#include <cstddef>

template <class type>
struct ForwardIterator
{
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = type;
    using pointer = value_type*;
    using reference = value_type&;

    ForwardIterator(pointer ptr) : p(ptr) {}

    reference operator*() const
    {
        return *p;
    }

    pointer operator->()
    {
        return p;
    }

    ForwardIterator& operator++()
    {
        p++;
        return *this;
    }

    ForwardIterator operator++(int)
    {
        ForwardIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator== (const ForwardIterator& a, const ForwardIterator& b)
    {
        return a.p == b.p;
    };

    friend bool operator!= (const ForwardIterator& a, const ForwardIterator& b)
    {
        return a.p != b.p;
    };

private:
    pointer p;
};