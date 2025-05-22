add_library(handler Handler.cpp)
target_include_directories(handler PUBLIC .)
target_link_libraries(handler ws2_32)