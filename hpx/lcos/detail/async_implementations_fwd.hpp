//  Copyright (c) 2007-2015 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_LCOS_ASYNC_IMPLEMENTATIONS_FWD_APR_13_2015_0829AM)
#define HPX_LCOS_ASYNC_IMPLEMENTATIONS_FWD_APR_13_2015_0829AM

namespace hpx { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, typename ...Ts>
    hpx::future<
        typename traits::promise_local_result<
            typename hpx::actions::extract_action<Action>::remote_result_type
        >::type>
    async_impl(BOOST_SCOPED_ENUM(launch) policy, hpx::id_type const& id,
        Ts&&... vs);

    template <typename Action, typename Callback, typename ...Ts>
    hpx::future<
        typename traits::promise_local_result<
            typename hpx::actions::extract_action<Action>::remote_result_type
        >::type>
    async_cb_impl(BOOST_SCOPED_ENUM(launch) policy, hpx::id_type const& id,
        Callback&& cb, Ts&&... vs);
}}

#endif
