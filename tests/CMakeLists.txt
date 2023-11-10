find_package(Catch2 3 REQUIRED)
include(Catch)

if(MSVC)
add_compile_options(/wd4388) # '==': signed/unsigned mismatch
add_compile_options(/wd4623) # default constructor was implicitly defined as deleted
add_compile_options(/wd4625) # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626) # assignment operator was implicitly defined as deleted
add_compile_options(/wd5026) # move constructor was implicitly defined as deleted
add_compile_options(/wd5027) # move assignment operator was implicitly defined as deleted
else()
endif()

add_executable       (t-diagnostic t-diagnostic.cpp)
target_link_libraries(t-diagnostic PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-diagnostic)

add_executable       (t-lockfree_grow_only_bag t-lockfree_grow_only_bag.cpp)
target_link_libraries(t-lockfree_grow_only_bag PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_grow_only_bag)

add_executable       (t-lockfree_queue t-lockfree_queue.cpp)
target_link_libraries(t-lockfree_queue PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_queue)

add_executable       (t-raw_buffer t-raw_buffer.cpp)
target_link_libraries(t-raw_buffer PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-raw_buffer)

add_executable       (t-events_waiting t-events_waiting.cpp)
target_link_libraries(t-events_waiting PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-events_waiting)

add_executable       (t-logging t-logging.cpp)
target_link_libraries(t-logging PRIVATE gkr-basics Catch2::Catch2WithMain)
catch_discover_tests (t-logging)
