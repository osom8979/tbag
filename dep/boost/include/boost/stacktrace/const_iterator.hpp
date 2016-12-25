// Copyright Antony Polukhin, 2016.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STACKTRACE_CONST_ITERATOR_HPP
#define BOOST_STACKTRACE_CONST_ITERATOR_HPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

//#include <boost/iterator/iterator_facade.hpp>
#include <boost/stacktrace/frame.hpp>

#include <iterator>



namespace boost { namespace stacktrace {


#ifdef BOOST_STACKTRACE_DOXYGEN_INVOKED
/// Random access iterator over frames that returns boost::stacktrace::frame on dereference
class const_iterator: implementation_details {};

#else

// Forward declarations
template <std::size_t> class basic_stacktrace;

class const_iterator
{
    // boost::iterator_facade
    typedef const_iterator Derived;
    typedef frame          value_type;
    typedef std::ptrdiff_t difference_type;
    typedef frame        * pointer;
    typedef frame          reference;
    typedef std::random_access_iterator_tag iterator_category;

    const boost::stacktrace::detail::backend* impl_;
    std::size_t frame_no_;

    const_iterator(const boost::stacktrace::detail::backend* impl, std::size_t frame_no) BOOST_NOEXCEPT
        : impl_(impl)
        , frame_no_(frame_no)
    {}

    template <std::size_t> friend class basic_stacktrace;
    //friend class ::boost::iterators::iterator_core_access;

    frame dereference() const BOOST_NOEXCEPT {
        return frame(impl_->get_address(frame_no_));
    }

    bool equal(const const_iterator& it) const BOOST_NOEXCEPT {
        return impl_ == it.impl_ && frame_no_ == it.frame_no_;
    }

    void increment() BOOST_NOEXCEPT {
        ++frame_no_;
    }

    void decrement() BOOST_NOEXCEPT {
        --frame_no_;
    }

    void advance(std::size_t n) BOOST_NOEXCEPT {
        frame_no_ += n;
    }

    difference_type distance_to(const const_iterator& it) const {
        assert(impl_ == it.impl_);
        return it.frame_no_ - frame_no_;
    }

public:
    reference operator*() const
    {
        return dereference();
    }

    template <class Reference, class Pointer>
    struct operator_arrow_dispatch // proxy references
    {
        struct proxy
        {
            explicit proxy(Reference const & x) : m_ref(x) {}
            Reference* operator->() { return &m_ref; }
            // This function is needed for MWCW and BCC, which won't call
            // operator-> again automatically per 13.3.1.2 para 8
            operator Reference*() { return &m_ref; }
            Reference m_ref;
        };
        typedef proxy result_type;
        static result_type apply(Reference const & x)
        {
            return result_type(x);
        }
    };

    typedef operator_arrow_dispatch<reference, frame*> operator_arrow_dispatch_;

    pointer operator->() const
    {
        return operator_arrow_dispatch_::apply(*this->derived());
    }

    Derived & operator++()
    {
        increment();
        return this->derived();
    }

    Derived & operator+(std::size_t n)
    {
        advance(n);
        return this->derived();
    }

protected:
    //
    // Curiously Recurring Template interface.
    //
    Derived & derived()
    {
        return *static_cast<Derived*>(this);
    }

    Derived const & derived() const
    {
        return *static_cast<Derived const*>(this);
    }
};

template <class Iter>
class reverse_iterator
{
public:
    typedef const_iterator Derived;
    typedef frame          value_type;
    typedef std::ptrdiff_t difference_type;
    typedef frame        * pointer;
    typedef frame          reference;
    typedef std::random_access_iterator_tag iterator_category;
    typedef Iter iterator_type;

private:
    mutable Iter __t;  // no longer used as of LWG #2360, not removed due to ABI break

protected:
    Iter current;

public:
    reverse_iterator() : current() {}
    explicit reverse_iterator(Iter __x) : __t(__x), current(__x) {}
    template <class _Up>  reverse_iterator(const reverse_iterator<_Up>& __u)
        : __t(__u.base()), current(__u.base()) {}
    Iter base() const {return current;}
    reference operator*() const {Iter __tmp = current; return *--__tmp;}
    pointer  operator->() const {return (pointer)(operator*());}
    reverse_iterator& operator++() {--current; return *this;}
    reverse_iterator  operator++(int)
    {reverse_iterator __tmp(*this); --current; return __tmp;}
    reverse_iterator& operator--() {++current; return *this;}
    reverse_iterator  operator--(int)
    {reverse_iterator __tmp(*this); ++current; return __tmp;}
    reverse_iterator  operator+ (difference_type __n) const
    {return reverse_iterator(current - __n);}
    reverse_iterator& operator+=(difference_type __n)
    {current -= __n; return *this;}
    reverse_iterator  operator- (difference_type __n) const
    {return reverse_iterator(current + __n);}
    reverse_iterator& operator-=(difference_type __n)
    {current += __n; return *this;}
    reference         operator[](difference_type __n) const
    {return *(*this + __n);}
};

#endif // #ifdef BOOST_STACKTRACE_DOXYGEN_INVOKED

}} // namespace boost::stacktrace

#endif // BOOST_STACKTRACE_CONST_ITERATOR_HPP
