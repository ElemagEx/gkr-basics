add_executable        (t-tester "main.cpp")
target_link_libraries (t-tester PRIVATE gkr_core)

#add_executable        (t-diagnostic "t-diagnostic.cpp")
#target_link_libraries (t-diagnostic PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
#target_compile_options(t-diagnostic PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-diagnostic)

add_executable        (t-lockfree_grow_only_bag "t-lockfree_grow_only_bag.cpp")
target_link_libraries (t-lockfree_grow_only_bag PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-lockfree_grow_only_bag PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-lockfree_grow_only_bag)

#add_executable        (t-lockfree_queue "t-lockfree_queue.cpp")
#target_link_libraries (t-lockfree_queue PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
#target_compile_options(t-lockfree_queue PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-lockfree_queue)

add_executable        (t-address "t-address.cpp")
target_link_libraries (t-address PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-address PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-address)

add_executable        (t-socket "t-socket.cpp")
target_link_libraries (t-socket PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-socket PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-socket)

add_executable        (t-logging "t-logging.cpp")
target_link_libraries (t-logging PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
target_compile_options(t-logging PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-logging)

add_executable        (t-udp_socket_consumer_receiver "t-udp_socket_consumer_receiver.cpp")
target_link_libraries (t-udp_socket_consumer_receiver PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_data gkr_log gkr_comm)
target_compile_options(t-udp_socket_consumer_receiver PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-udp_socket_consumer_receiver)
