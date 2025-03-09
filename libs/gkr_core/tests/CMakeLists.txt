add_executable        (t-sys_path "t-sys_path.cpp")
target_link_libraries (t-sys_path PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-sys_path PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-sys_path)

add_executable        (t-sys_thread "t-sys_thread.cpp")
target_link_libraries (t-sys_thread PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-sys_thread PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-sys_thread)
