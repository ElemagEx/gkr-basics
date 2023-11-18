find_package(Catch2 3 REQUIRED)
include(Catch)

if(MSVC)
add_compile_options(/wd4388) # '==': signed/unsigned mismatch
add_compile_options(/wd4623) # default constructor was implicitly defined as deleted
add_compile_options(/wd4625) # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626) # assignment operator was implicitly defined as deleted
add_compile_options(/wd5026) # move constructor was implicitly defined as deleted
add_compile_options(/wd5027) # move assignment operator was implicitly defined as deleted
endif()

add_executable       (t-diagnostic t-diagnostic.cpp)
target_link_libraries(t-diagnostic PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-diagnostic)

add_executable       (t-lockfree_grow_only_bag t-lockfree_grow_only_bag.cpp)
target_link_libraries(t-lockfree_grow_only_bag PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_grow_only_bag)

add_executable       (t-lockfree_queue t-lockfree_queue.cpp)
target_link_libraries(t-lockfree_queue PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-lockfree_queue)

add_executable       (t-raw_buffer t-raw_buffer.cpp)
target_link_libraries(t-raw_buffer PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-raw_buffer)

add_executable       (t-events_waiting t-events_waiting.cpp)
target_link_libraries(t-events_waiting PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-events_waiting)

add_executable       (t-logging t-logging.cpp)
target_link_libraries(t-logging PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-logging)

add_executable       (t-address t-address.cpp)
target_link_libraries(t-address PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-address)

add_executable       (t-socket t-socket.cpp)
target_link_libraries(t-socket PRIVATE gkr_core Catch2::Catch2WithMain)
catch_discover_tests (t-socket)

if(GKR_BUILD_EXAMPLES)
add_executable       (t-udp_socket_consumer_receiver t-udp_socket_consumer_receiver.cpp)
target_link_libraries(t-udp_socket_consumer_receiver PRIVATE gkr_core udp_socket_consumer udp_socket_receiver Catch2::Catch2WithMain)
catch_discover_tests (t-udp_socket_consumer_receiver)
endif()
