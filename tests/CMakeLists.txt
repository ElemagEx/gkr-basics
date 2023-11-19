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
target_link_libraries(t-diagnostic PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-diagnostic)

add_executable       (t-lockfree_grow_only_bag t-lockfree_grow_only_bag.cpp)
target_link_libraries(t-lockfree_grow_only_bag PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-lockfree_grow_only_bag)

add_executable       (t-lockfree_queue t-lockfree_queue.cpp)
target_link_libraries(t-lockfree_queue PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-lockfree_queue)

add_executable       (t-raw_buffer t-raw_buffer.cpp)
target_link_libraries(t-raw_buffer PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-raw_buffer)

add_executable       (t-events_waiting t-events_waiting.cpp)
target_link_libraries(t-events_waiting PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-events_waiting)

add_executable       (t-address t-address.cpp)
target_link_libraries(t-address PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-address)

add_executable       (t-socket t-socket.cpp)
target_link_libraries(t-socket PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing)
catch_discover_tests (t-socket)

add_executable       (t-logging t-logging.cpp)
target_link_libraries(t-logging PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing gkr_log)
catch_discover_tests (t-logging)

add_executable       (t-udp_socket_consumer_receiver t-udp_socket_consumer_receiver.cpp)
target_link_libraries(t-udp_socket_consumer_receiver PRIVATE Catch2::Catch2WithMain gkr_core gkr_testing gkr_data gkr_log gkr_comm)
catch_discover_tests (t-udp_socket_consumer_receiver)
