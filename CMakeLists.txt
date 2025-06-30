cmake_minimum_required(VERSION 3.28)
project(recursion)

add_executable(nested-calls)
target_sources(nested-calls PRIVATE nested-calls.cpp)
target_compile_features(nested-calls PRIVATE cxx_std_20)

add_executable(shortest-recursive)
target_sources(shortest-recursive PRIVATE shortest-recursive.cpp)
target_compile_features(shortest-recursive PRIVATE cxx_std_20)

add_executable(recursion-3)
target_sources(recursion-3 PRIVATE recursion-3.cpp)
target_compile_features(recursion-3 PRIVATE cxx_std_20)

add_executable(recursion-4)
target_sources(recursion-4 PRIVATE recursion-4.cpp)
target_compile_features(recursion-4 PRIVATE cxx_std_20)
