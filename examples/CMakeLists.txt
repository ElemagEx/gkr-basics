add_library               (windows_debugger_consumer STATIC  windowsDebuggerConsumer.h windowsDebuggerConsumer.cpp)
target_link_libraries     (windows_debugger_consumer PRIVATE gkr-basics)
target_include_directories(windows_debugger_consumer PUBLIC  ".")

add_library               (android_log_consumer STATIC  androidLogConsumer.h androidLogConsumer.cpp)
target_link_libraries     (android_log_consumer PRIVATE gkr-basics)
target_include_directories(android_log_consumer PUBLIC  ".")

add_library               (console_consumer STATIC  appConsoleConsumer.h appConsoleConsumer.cpp)
target_link_libraries     (console_consumer PRIVATE gkr-basics)
target_include_directories(console_consumer PUBLIC  ".")

add_library               (udp_socket_consumer STATIC  udpSocketConsumer.h udpSocketConsumer.cpp)
target_link_libraries     (udp_socket_consumer PRIVATE gkr-basics)
target_include_directories(udp_socket_consumer PUBLIC  ".")

add_library               (udp_socket_receiver STATIC  udpSocketReceiver.h udpSocketReceiver.cpp)
target_link_libraries     (udp_socket_receiver PRIVATE gkr-basics)
target_include_directories(udp_socket_consumer PUBLIC  ".")
