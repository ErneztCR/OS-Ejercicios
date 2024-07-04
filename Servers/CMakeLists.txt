cmake_minimum_required(VERSION 3.22)
project(Servers C)

set(CMAKE_C_STANDARD 11)

add_executable(Servers 
    base_socket.c
    main.c 
    request_handler.c 
    shared.c
)