//  Copyright (c) 2007-2015 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/parallel/executors/this_thread_executors.hpp>
#include <hpx/util/lightweight_test.hpp>

#include <algorithm>
#include <cstdlib>
#include <vector>

#include <boost/range/functions.hpp>

///////////////////////////////////////////////////////////////////////////////
std::size_t test() { return hpx::get_worker_thread_num(); }

template <typename Executor>
void test_sync(Executor& exec)
{
    typedef hpx::parallel::executor_traits<Executor> traits;

    HPX_TEST(traits::execute(exec, &test) == hpx::get_worker_thread_num());
}

template <typename Executor>
void test_async(Executor& exec)
{
    typedef hpx::parallel::executor_traits<Executor> traits;

    HPX_TEST(
        traits::async_execute(exec, &test).get() ==
        hpx::get_worker_thread_num());
}

///////////////////////////////////////////////////////////////////////////////
void bulk_test(std::size_t tid, int value)
{
    HPX_TEST(tid == hpx::get_worker_thread_num());
}

template <typename Executor>
void test_bulk_sync(Executor& exec)
{
    typedef hpx::parallel::executor_traits<Executor> traits;

    std::size_t tid = hpx::get_worker_thread_num();

    std::vector<int> v(107);
    std::iota(boost::begin(v), boost::end(v), std::rand());

    using hpx::util::placeholders::_1;
    traits::execute(exec, hpx::util::bind(&bulk_test, tid, _1), v);
}

template <typename Executor>
void test_bulk_async(Executor& exec)
{
    typedef hpx::parallel::executor_traits<Executor> traits;

    std::size_t tid = hpx::get_worker_thread_num();

    std::vector<int> v(107);
    std::iota(boost::begin(v), boost::end(v), std::rand());

    using hpx::util::placeholders::_1;
    hpx::when_all(traits::async_execute(
        exec, hpx::util::bind(&bulk_test, tid, _1), v)).get();
}

template <typename Executor>
void test_this_thread_executor(Executor& exec)
{
    test_sync(exec);
    test_async(exec);
    test_bulk_sync(exec);
    test_bulk_async(exec);
}

int hpx_main(int argc, char* argv[])
{
#if defined(HPX_HAVE_STATIC_SCHEDULER)
    {
        hpx::parallel::this_thread_static_queue_executor exec;
        test_this_thread_executor(exec);
    }
#endif

#if defined(HPX_HAVE_STATIC_PRIORITY_SCHEDULER)
    {
        hpx::parallel::this_thread_static_priority_queue_executor exec;
        test_this_thread_executor(exec);
    }
#endif

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    // Initialize and run HPX
    HPX_TEST_EQ_MSG(hpx::init(argc, argv), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}
