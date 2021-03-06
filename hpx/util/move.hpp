//  Copyright (c) 2012 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_UTIL_MOVE_HPP
#define HPX_UTIL_MOVE_HPP

#include <hpx/config.hpp>

#include <hpx/util/decay.hpp>

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#error HPX needs rvalue reference support
#endif

#include <utility>

#if !defined(HPX_HAVE_CXX11_DELETED_FUNCTIONS)
#define HPX_MOVABLE_BUT_NOT_COPYABLE(TYPE)                                    \
    private:                                                                  \
        TYPE(TYPE const &);                                                   \
        TYPE& operator=(TYPE const &);                                        \
/**/
#else
#define HPX_MOVABLE_BUT_NOT_COPYABLE(TYPE)                                    \
    public:                                                                   \
        TYPE(TYPE const &) = delete;                                          \
        TYPE& operator=(TYPE const &) = delete;                               \
    private:                                                                  \
/**/
#endif

namespace hpx { namespace util { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    BOOST_FORCEINLINE typename decay<T>::type
    decay_copy(T&& v)
    {
        return std::forward<T>(v);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct make_temporary_impl
    {
        typedef T && type;

        template <typename U>
        BOOST_FORCEINLINE static T && call(U& u)
        {
            return std::move(u);
        }
    };

    template <typename T>
    struct make_temporary_impl<T&>
    {
        typedef T type;

        BOOST_FORCEINLINE static T call(T& u)
        {
            return u;
        }
    };

    template <typename T>
    struct make_temporary_impl<T const&>
    {
        typedef T type;

        BOOST_FORCEINLINE static T call(T const& u)
        {
            return u;
        }
    };

    template <typename T>
    BOOST_FORCEINLINE typename detail::make_temporary_impl<T>::type
    make_temporary(typename std::remove_reference<T>::type& v)
    {
        return detail::make_temporary_impl<T>::call(v);
    }

    template <typename T>
    BOOST_FORCEINLINE typename detail::make_temporary_impl<T>::type
    make_temporary(typename std::remove_reference<T>::type&& v)
    {
        return detail::make_temporary_impl<T>::call(v);
    }
}}}

#endif
