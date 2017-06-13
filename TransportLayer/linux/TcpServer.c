//
// Created by yingzi on 2017/6/11.
//

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define SERVER_PORT 5555
#define MAX_BUFFER_SIZE 1024

int serverSocket, clientSocket;
struct sockaddr_in serverAddr, clientAddr;
char buffer[MAX_BUFFER_SIZE];

void quit(int);

int main() {
    // 创建TCP服务端套接字
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Create server socket error:%s\n", strerror(errno));
        return -1;
    }

    signal(SIGINT, quit);

    // 设置套接字复用
    unsigned int value = 0x1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (void *) &value, sizeof(value));

    // 设置服务端地址
    /* // 描述套接字地址的结构体
     * struct sockaddr_in{
     *     short sin_family;    // 地址簇，一般为AF_INET
     *     unsigned short sin_port; // 端口号，网络字节顺序
     *     struct in_addr sin_addr; // ip地址
     *     unsigned char sin_zero[8];   // 无意思，内存对齐用
     * };
     * // 描述ip地址的结构体
     * typedef uint32_t in_addr_t;  // ip地址采用无符号32位整型
     * struct in_addr{
     *     in_addr_t s_addr;    // ip地址
     * };
     */
    bzero(&serverAddr, sizeof(serverAddr));   // 清空
    serverAddr.sin_family = AF_INET;       // 协议簇
    serverAddr.sin_port = htons(SERVER_PORT);      // 端口
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);    // ip

    // 将套接字与地址绑定
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        printf("Bind error:%s\n",strerror(errno));
        return -1;
    }

    // 服务端开始监听，最大连接数为5
    if (listen(serverSocket, 5) < 0) {
        printf("Server start listening failed:%s\n",strerror(errno));
        return -1;
    }

    printf("Listening on port: %d\n", SERVER_PORT);

    while (1) {
        // 接收客户端请求，建立一个客户端套接字
        int addr_len = sizeof(clientAddr);
        if ((clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addr_len)) < 0) {
            printf("Accept from client failed:%s\n", strerror(errno));
            return -1;
        }

        printf("Client %s:%d connected\n", inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));

        // 接受客户端发来的消息
        ssize_t recvNum = 0;
        int count = 0;
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            recvNum = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
            if (recvNum < 0) {
                printf("Receive from client failed:%s\n", strerror(errno));
                break;
            } else if (recvNum == 0) {
                printf("Client %s:%d disconnected. ", inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));
                printf("Receive message %d times\n", count);
                break;
            } else {
                printf("%d:%s\n", ++count, buffer);
//            if (send(clientSocket, buffer, MAX_BUFFER_SIZE, 0) < 0) {
//                printf("Send message failed:%s\n", strerror(errno));
//                break;
//            }
            }
        }
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}

void quit(int signum) {
    close(clientSocket);
    close(serverSocket);
    exit(0);
}