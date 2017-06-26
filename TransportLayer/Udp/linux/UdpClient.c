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
//#define SERVER_IP "10.205.41.52"
char SERVER_IP[20] = "10.205.41.52";
#define MAX_BUFFER_SIZE 100

int clientSocket;
struct sockaddr_in serverAddr;
char buffer[MAX_BUFFER_SIZE];
char eventNum[20];
char valueNum[20];

char* getEventName();
int getValueNum();
void quit(int);

int main(int argc, char** argv) {

    if (argc == 2) {
        strcpy(SERVER_IP, argv[1]);
    }
    else if (argc != 1) {
        printf("Usage: ./client server_ip\n");
        return -1;
    }

    // 创建UDP客户端套接字
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("create client socket failed!");
        return -1;
    }

    signal(SIGINT, quit);

    // 配置服务端套接字
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    srand((unsigned)time(NULL));
    while (1) {
        strcpy(buffer, "<event name=\"");
        char* eventName = getEventName();
        strcat(buffer, eventName);
        strcat(buffer, "\" value=\"x = ");
        sprintf(valueNum, "%d", getValueNum());
        strcat(buffer, valueNum);
        strcat(buffer, "\"/>");
//        if (eventNumInt < 0) continue;  // 模拟事件丢失
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            printf("Send message %s failed!", buffer);
            break;
        }
        printf("%s\n", buffer);
        sleep(1);
    }

    close(clientSocket);
    return 0;
}

char* getEventName() {
    static double p = 0.7;  // increase事件的概率
    if (rand() > RAND_MAX * p) return "decrease";
    else return "increase";
}

//int getEventNum() {
//    static int eventNum = -1;
//    static double p = 0.2;  // 异常的概率
//
//    eventNum++;
//    if (rand() > RAND_MAX * (1 - p)) {
////        return rand() % 10 + 1;  // 返回一个异常的事件编号
//        return -1;
//    }
//    else return eventNum % 5 + 1;
//}

int getValueNum() {
//    static int valueNum = -1;
//    static double p = 0.2;
//
//    valueNum++;
//    if (rand() > RAND_MAX * (1 - p)) {
//        return rand();  // 模拟事件篡改
//    }
//    else return valueNum % 100 + 1;
    return rand() % 60;
}

void quit(int signum) {
    close(clientSocket);
    exit(0);
}