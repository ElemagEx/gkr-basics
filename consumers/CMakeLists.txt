add_library          (windows-debugger-consumer STATIC  windowsDebuggerConsumer.h windowsDebuggerConsumer.cpp)
target_link_libraries(windows-debugger-consumer PRIVATE gkr-basics)

add_library          (android-log-consumer STATIC  androidLogConsumer.h androidLogConsumer.cpp)
target_link_libraries(android-log-consumer PRIVATE gkr-basics)

add_library          (console-consumer STATIC  consoleConsumer.h consoleConsumer.cpp)
target_link_libraries(console-consumer PRIVATE gkr-basics)

add_library          (udp-socket-consumer STATIC  udpSocketConsumer.h udpSocketConsumer.cpp)
target_link_libraries(udp-socket-consumer PRIVATE gkr-basics)
