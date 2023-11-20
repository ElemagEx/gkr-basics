add_executable        (t-log_consumer_text_file "t-log_consumer_text_file.cpp")
target_link_libraries (t-log_consumer_text_file PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
target_compile_options(t-log_consumer_text_file PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-log_consumer_text_file)
