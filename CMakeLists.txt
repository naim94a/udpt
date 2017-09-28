project(udpt)
cmake_minimum_required(VERSION 3.2)
enable_testing()

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB src_files "src/*.c"
        "src/*.cpp"
        "src/db/*.cpp")

LIST(APPEND LIBS "pthread" "sqlite3" "boost_program_options" "boost_system" "event" "event_pthreads")
add_definitions(-Wall)

add_executable(udpt ${src_files})
target_link_libraries(udpt ${LIBS})

add_executable(udpt_tests EXCLUDE_FROM_ALL tests/main.cpp ${src_files})
target_compile_definitions(udpt_tests PRIVATE TEST=1)

target_link_libraries(udpt_tests gtest ${LIBS})

add_test(NAME udpt_tests COMMAND udpt_tests)

# Add version to code
execute_process(
        COMMAND git log -1 --format="%h"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(
        ${CMAKE_SOURCE_DIR}/src/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
