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

add_executable(hanoi)
target_sources(hanoi PRIVATE hanoi.cpp)
target_compile_features(hanoi PRIVATE cxx_std_20)

add_executable(flood-fill)
target_sources(flood-fill PRIVATE flood-fill.cpp)
target_compile_features(flood-fill PRIVATE cxx_std_20)

add_executable(ackermann)
target_sources(ackermann PRIVATE ackermann.cpp)
target_compile_features(ackermann PRIVATE cxx_std_20)

add_executable(concat)
target_sources(concat PRIVATE concat.cpp)
target_compile_features(concat PRIVATE cxx_std_20)

add_executable(product)
target_sources(product PRIVATE product.cpp)
target_compile_features(product PRIVATE cxx_std_20)

add_executable(min-distance)
target_sources(min-distance PRIVATE min-distance.cpp)
target_compile_features(min-distance PRIVATE cxx_std_20)

add_executable(trees)
target_sources(trees PRIVATE trees.cpp)
target_compile_features(trees PRIVATE cxx_std_20)

add_executable(maze)
target_sources(maze PRIVATE maze.cpp)
target_compile_features(maze PRIVATE cxx_std_20)

add_executable(binary-search)
target_sources(binary-search PRIVATE binary-search.cpp)
target_compile_features(binary-search PRIVATE cxx_std_20)

add_executable(quicksort)
target_sources(quicksort PRIVATE quicksort.cpp)
target_compile_features(quicksort PRIVATE cxx_std_20)

add_executable(mergesort)
target_sources(mergesort PRIVATE mergesort.cpp)
target_compile_features(mergesort PRIVATE cxx_std_20)
