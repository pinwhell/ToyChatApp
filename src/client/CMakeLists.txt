add_library(chatapp-clnt STATIC Core.cpp)
target_link_libraries(chatapp-clnt chatapp-sockets)
add_subdirectory(cli)