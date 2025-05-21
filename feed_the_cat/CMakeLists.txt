add_library(crazy_cat Cat.cpp)
target_include_directories(crazy_cat PUBLIC .)
add_executable(feed_server main.cpp)
target_link_libraries(feed_server ws2_32 crazy_cat)
