cmake_minimum_required(VERSION 3.18)
project(double_linked_list C)

set(CMAKE_C_STANDARD 11)

include_directories(assignment)

add_executable(double_linked_list
        assignment/allocator.c
        assignment/allocator.h
        assignment/config.h
        assignment/doubly_linked_list_with_iterator.c
        assignment/doubly_linked_list_with_iterator.h
        assignment/general.h
        assignment/list.h
        assignment/list_iterator.h
        # assignment/list_main_driver.c
        assignment/list_test_driver.c
        assignment/list_types.h
        assignment/mainpage.h
        assignment/shortcut.c
        assignment/shortcut.h
        assignment/test_list.c
        assignment/test_list.h)
