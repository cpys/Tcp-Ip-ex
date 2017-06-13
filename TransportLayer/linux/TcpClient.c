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
#define MAX_BUFFER_SIZE 100

int clientSocket;
struct sockaddr_in serverAddr;
char buffer[MAX_BUFFER_SIZE];

int getEventNum();
int getValueNum();
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

    srand((unsigned)time(NULL));
    while (1) {
        strcpy(buffer, "<event name=\"event");
        strcat(buffer, itoa(getEventNum()));
        strcat(buffer, "\" value=\"x = ");
        strcat(buffer, itoa(getValueNum()));
        strcat(buffer, "\"/>");
        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            perror("Send message %s failed!", buffer);
            break;
        }
        printf("%s\n", buffer);
        sleep(1);
    }

//    char* alphabet = "abcdefghijklmnopqrstuvwxyz";
//    int index = 0;
//    for (int i = 0; i < SEND_TIMES; i++) {
//        index = i % 26 + 1;
//        if (send(clientSocket, alphabet, index, 0) < 0) {
//            perror("Send message failed!");
//            break;
//        }
//        for (int i = 0; i < index; i++) printf("%c", alphabet[i]);
//        printf("\n");
//        sleep(1);
//    }

    close(clientSocket);
    return 0;
}

int getEventNum() {
    static int eventNum = 1;
    static double p = 0.1;  // 异常的概率
    if (rand() > RAND_MAX * (1 - p)) {
        return rand() % 1000;  // 返回一个异常的事件编号
    }
    else return eventNum++;
}

int getValueNum() {
    static int valueNum = 100;
    static double p = 0.1;
    if (rand() > RAND_MAX * (1 - p)) {
        return rand() % 1000;
    }
    else return valueNum++;
}

void quit(int signum) {
    close(clientSocket);
    exit(0);
}