add_library(orchestrator Orchestrator.cpp)
target_link_libraries(orchestrator server cat)
target_include_directories(orchestrator PUBLIC .)