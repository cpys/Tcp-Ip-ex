//
// Created by yingzi on 2017/6/26.
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

#define SERVER_PORT 5556
#define MAX_BUFFER_SIZE 1024

int serverSocket;
struct sockaddr_in serverAddr, clientAddr;
int len = sizeof(clientAddr);
char buffer[MAX_BUFFER_SIZE];

void quit(int);

int main() {
    // 创建UDP服务端套接字
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Create UDP server socket error!");
        return -1;
    }

    signal(SIGINT, quit);

    // 设置服务端地址
    bzero(&serverAddr, sizeof(serverAddr));   // 清空
    serverAddr.sin_family = AF_INET;       // 协议簇
    serverAddr.sin_port = htons(SERVER_PORT);      // 端口
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);    // ip

    // 将套接字与地址绑定
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error!");
        return -1;
    }

    printf("Start server on port: %d\n", SERVER_PORT);

    ssize_t recvNum = 0;
    int count = 0;
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recvNum = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &len);
        if (recvNum < 0) {
            perror("Receive from client failed!");
            break;
        } else if (recvNum == 0) {
            printf("Client %s:%d disconnected. ", inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));
            printf("Receive message %d times\n", count);
            break;
        } else {
            printf("%d:%s\n", ++count, buffer);
        }
    }
    return 0;
}

void quit(int signum) {
    close(serverSocket);
    exit(0);
}