# Copyright (c) 2007-2015 Hartmut Kaiser
# Copyright (c) 2011-2012 Bryce Adelstein-Lelbach
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(tests
    apply_colocated
    apply_local
    apply_local_executor
    apply_remote
    async_cb_colocated
    async_cb_remote
    async_continue
    async_continue_cb
    async_continue_cb_colocated
    async_local
    async_local_executor
    async_remote
    broadcast
    broadcast_apply
    condition_variable
    barrier
    fold
    future
    future_ref
    future_then
    future_then_executor
    future_wait
    local_latch
    local_barrier
    local_dataflow
    local_dataflow_executor
    local_event
    local_mutex
    packaged_action
    promise
    reduce
    remote_latch
    run_guarded
    shared_future
    unwrapped
    when_all
    when_any
    when_some
   )

if(HPX_WITH_AWAIT)
  set(tests ${tests} await)
  set(await_PARAMETERS THREADS_PER_LOCALITY 4)
endif()

set(apply_colocated_PARAMETERS LOCALITIES 2)
set(apply_local_PARAMETERS THREADS_PER_LOCALITY 4)
set(apply_local_executor_PARAMETERS THREADS_PER_LOCALITY 4)
set(apply_remote_PARAMETERS LOCALITIES 2)
set(async_cb_colocated_PARAMETERS LOCALITIES 2)

set(async_continue_PARAMETERS LOCALITIES 2)
set(async_continue_cb_PARAMETERS LOCALITIES 2)
set(async_continue_cb_colocated_PARAMETERS LOCALITIES 2)
set(async_local_PARAMETERS THREADS_PER_LOCALITY 4)
set(async_local_executor_PARAMETERS THREADS_PER_LOCALITY 4)
set(async_remote_PARAMETERS LOCALITIES 2)
set(async_cb_remote_PARAMETERS LOCALITIES 2)

set(broadcast_PARAMETERS LOCALITIES 2)
set(broadcast_apply_PARAMETERS LOCALITIES 2)

set(future_PARAMETERS THREADS_PER_LOCALITY 4)
set(future_then_PARAMETERS THREADS_PER_LOCALITY 4)
set(future_then_executor_PARAMETERS THREADS_PER_LOCALITY 4)
set(future_wait_PARAMETERS THREADS_PER_LOCALITY 4)

set(local_barrier_PARAMETERS THREADS_PER_LOCALITY 4)

set(local_latch_PARAMETERS THREADS_PER_LOCALITY 4)
set(remote_latch_PARAMETERS LOCALITIES 2)

set(local_event_PARAMETERS THREADS_PER_LOCALITY 4)

set(local_mutex_PARAMETERS THREADS_PER_LOCALITY 4)

set(packaged_action_PARAMETERS THREADS_PER_LOCALITY 4)

set(promise_PARAMETERS THREADS_PER_LOCALITY 4)

set(reduce_PARAMETERS LOCALITIES 2)

set(run_guarded_PARAMETERS THREADS_PER_LOCALITY 4)

foreach(test ${tests})
  set(sources
      ${test}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${test}_test
                     SOURCES ${sources}
                     ${${test}_FLAGS}
                     EXCLUDE_FROM_ALL
                     HPX_PREFIX ${HPX_BUILD_PREFIX}
                     FOLDER "Tests/Unit/LCOs")

  add_hpx_unit_test("lcos" ${test} ${${test}_PARAMETERS})

  # add a custom target for this example
  add_hpx_pseudo_target(tests.unit.lcos.${test})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(tests.unit.lcos
                              tests.unit.lcos.${test})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(tests.unit.lcos.${test}
                              ${test}_test_exe)
endforeach()

if(HPX_WITH_COMPILE_ONLY_TESTS)
  # add compile time tests
  set(compile_tests
      fail_compile_mutex_move
      fail_compile_spinlock_move
      fail_compile_spinlock_no_backoff_move
     )

  set(fail_compile_mutex_move_FLAGS FAILURE_EXPECTED)
  set(fail_compile_spinlock_move_FLAGS FAILURE_EXPECTED)
  set(fail_compile_spinlock_no_backoff_move_FLAGS FAILURE_EXPECTED)

  foreach(compile_test ${compile_tests})
    set(sources
        ${compile_test}.cpp)

    source_group("Source Files" FILES ${sources})

    if(MSVC)
      # add dummy library for MSVC to generate a project in VS
      add_hpx_library(${compile_test}_compile_test
                      SOURCES ${sources}
                      EXCLUDE_FROM_ALL
                      FOLDER "Tests/Unit/LCOs/CompileOnly")
      set(target ${compile_test}_compile_test_lib)
    else()
      add_hpx_unit_compile_test(
        "lcos"
        ${compile_test}
        SOURCES ${sources}
        ${${compile_test}_FLAGS}
        FOLDER "Tests/Unit/LCOs/CompileOnly")
      set(target ${compile_test}_test_exe)
    endif()

    # add a custom target for this example
    add_hpx_pseudo_target(tests.unit.lcos.${compile_test})

    # make pseudo-targets depend on master pseudo-target
    add_hpx_pseudo_dependencies(tests.unit.lcos
                                tests.unit.lcos.${compile_test})

    # add dependencies to pseudo-target
    add_hpx_pseudo_dependencies(tests.unit.lcos.${compile_test} ${target})
  endforeach()
endif()

