cmake_minimum_required(VERSION 3.8)
project(Tcp-Ip-ex)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES TransportLayer/Udp/linux/UdpServer.c)
add_executable(Tcp-Ip-ex ${SOURCE_FILES})