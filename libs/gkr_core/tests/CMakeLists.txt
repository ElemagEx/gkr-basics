add_executable        (t-raw_buffer "t-raw_buffer.cpp")
target_link_libraries (t-raw_buffer PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-raw_buffer PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-raw_buffer)

add_executable        (t-sys_path "t-sys_path.cpp")
target_link_libraries (t-sys_path PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-sys_path PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-sys_path)

add_executable        (t-sys_thread "t-sys_thread.cpp")
target_link_libraries (t-sys_thread PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-sys_thread PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-sys_thread)

add_executable        (t-waitable_object "t-waitable_object.cpp")
target_link_libraries (t-waitable_object PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-waitable_object PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-waitable_object)

add_executable        (t-waitable_event "t-waitable_event.cpp")
target_link_libraries (t-waitable_event PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-waitable_event PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-waitable_event)

add_executable        (t-waitable_semaphore "t-waitable_semaphore.cpp")
target_link_libraries (t-waitable_semaphore PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-waitable_semaphore PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-waitable_semaphore)
