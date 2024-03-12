cmake_minimum_required(VERSION 3.21)
project(CAOS C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(tech07)

add_executable(t06-2 tech06/2.c)
add_executable(t07-0 tech07/0.c)
add_executable(t07-1 tech07/1.c)
add_executable(t07-2 tech07/2.c)

find_package(PythonLibs 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(t07-0 ${PYTHON_LIBRARIES})