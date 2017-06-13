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
#define SERVER_IP "10.109.253.29"
#define SEND_TIMES 100000
#define MAX_BUFFER_SIZE

int clientSocket;
struct sockaddr_in serverAddr;
char buffer[MAX_BUFFER_SIZE];

void quit(int);

int main(int argc, char** argv) {

    // 创建TCP客户端套接字
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create client socket failed!");
        return -1;
    }

    signal(SIGINT, quit);

    // 配置服务端套接字
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接服务端
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect to server failed!");
        return -1;
    }

    char* alphabet = "abcdefghijklmnopqrstuvwxyz";
    int index = 0;
    for (int i = 0; i < SEND_TIMES; i++) {
        index = i % 26 + 1;
        if (send(clientSocket, alphabet, index, 0) < 0) {
            perror("Send message failed!");
            break;
        }
        for (int i = 0; i < index; i++) printf("%c", alphabet[i]);
        printf("\n");
        sleep(1);
    }

    close(clientSocket);
    return 0;
}

void quit(int signum) {
    close(clientSocket);
    exit(0);
}