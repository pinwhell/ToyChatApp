add_executable(sv_playground sv_playground.cpp)
add_executable(cl_playground cl_playground.cpp)

target_link_libraries(sv_playground chatapp-sockets chatapp-sv)
target_link_libraries(cl_playground chatapp-sockets chatapp-clnt)