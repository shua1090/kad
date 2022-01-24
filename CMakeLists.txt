cmake_minimum_required(VERSION 3.16)
project(kademlia_rt)

set(CMAKE_CXX_STANDARD 11)

add_library(kademlia_rt kademlia/RoutingTable.cpp)

add_executable(kademlia_rt_test kademlia/main.cpp)
target_link_libraries(kademlia_rt_test kademlia_rt)
target_include_directories(kademlia_rt_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
