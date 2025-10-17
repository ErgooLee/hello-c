cmake_minimum_required(VERSION 3.10.0)
project(helloworld VERSION 0.1.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(socket_server socket_server.c)

add_executable(socket_client socket_client.c)

add_executable(pipe pipe.c)

add_executable(fifo FIFO.c)

add_executable(msg_queue_sender msg_queue_sender.c)

add_executable(msg_queue_receiver msg_queue_receiver.c)

add_executable(sem_reader sem_reader.c)

add_executable(sem_writer sem_writer.c)

add_executable(unnamed_sem unnamed_sem.c)

add_executable(named_sem named_sem.c)

if(LINUX)
    add_executable(socket_server_epoll socket_server_epoll.c)
endif()