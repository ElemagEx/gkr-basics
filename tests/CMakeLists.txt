add_executable       (t-lockfree_grow_only_bag t-lockfree_grow_only_bag.cpp)
target_link_libraries(t-lockfree_grow_only_bag PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_grow_only_bag)

add_executable       (t-thread_waiting t-thread_waiting.cpp)
target_link_libraries(t-thread_waiting PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-thread_waiting)

add_executable       (t-thread_worker t-thread_worker.cpp)
target_link_libraries(t-thread_worker PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-thread_worker)
