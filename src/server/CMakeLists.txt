add_library(chatapp-sv STATIC Core.cpp)
target_link_libraries(chatapp-sv chatapp-sockets)
add_subdirectory(cli)