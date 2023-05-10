add_executable       (t-diagnostic t-diagnostic.cpp)
target_link_libraries(t-diagnostic PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-diagnostic)

add_executable       (t-lockfree_grow_only_bag t-lockfree_grow_only_bag.cpp)
target_link_libraries(t-lockfree_grow_only_bag PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_grow_only_bag)

add_executable       (t-lockfree_queue t-lockfree_queue.cpp)
target_link_libraries(t-lockfree_queue PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_queue)

add_executable       (t-events_waiting t-events_waiting.cpp)
target_link_libraries(t-events_waiting PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-events_waiting)

add_executable       (t-logging t-logging.cpp)
target_link_libraries(t-logging PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-logging)
