cmake_minimum_required(VERSION 3.28)
project(recursion)

add_executable(nested-calls)
target_sources(nested-calls PRIVATE nested-calls.cpp)
target_compile_features(nested-calls PRIVATE cxx_std_20)

add_executable(shortest-recursive)
target_sources(shortest-recursive PRIVATE shortest-recursive.cpp)
target_compile_features(shortest-recursive PRIVATE cxx_std_20)

add_executable(count-down-and-up)
target_sources(count-down-and-up PRIVATE count-down-and-up.cpp)
target_compile_features(count-down-and-up PRIVATE cxx_std_20)

add_executable(factorial)
target_sources(factorial PRIVATE factorial.cpp)
target_compile_features(factorial PRIVATE cxx_std_20)

add_executable(fibonacci)
target_sources(fibonacci PRIVATE fibonacci.cpp)
target_compile_features(fibonacci PRIVATE cxx_std_20)

add_executable(iterative-to-recursive)
target_sources(iterative-to-recursive PRIVATE iterative-to-recursive.cpp)
target_compile_features(iterative-to-recursive PRIVATE cxx_std_20)

add_executable(exponents)
target_sources(exponents PRIVATE exponents.cpp)
target_compile_features(exponents PRIVATE cxx_std_20)

add_executable(sums)
target_sources(sums PRIVATE sums.cpp)
target_compile_features(sums PRIVATE cxx_std_20)

add_executable(array-sum)
target_sources(array-sum PRIVATE array-sum.cpp)
target_compile_features(array-sum PRIVATE cxx_std_20)

add_executable(reverse-string)
target_sources(reverse-string PRIVATE reverse-string.cpp)
target_compile_features(reverse-string PRIVATE cxx_std_20)

add_executable(palindromes)
target_sources(palindromes PRIVATE palindromes.cpp)
target_compile_features(palindromes PRIVATE cxx_std_20)
