add_library(server Server.cpp)
target_link_libraries(server handler)
target_include_directories(server PUBLIC .)