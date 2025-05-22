add_library(client Client.cpp)
target_include_directories(client PUBLIC .)
add_executable(main_client main.cpp)
target_link_libraries(main_client client ws2_32)