#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <regex.h>

#include "common.h"

#define VALUE_PORT 12345
#define VALUE_ROOT 54321

//字符串处理
void strreplace(char*sentence, char src, char dest);
void intToString(int n, char* s);
void convertToUpperCase(char* sentence);
void removeLineFeed(char* sentence);

int getIpFromPortMsg(char* param, char* clientIp);
int getPortFromPortMsg(char* param, int* pClientPort);
int getCommandFromSentence(char* sentence, char* command, char* parameter);

void makeStartTransferMsg(char*startTransferMsg, char*fileName);
void makePasvPortMsg(char* pasvMsg, int pasvPort);

//参数检查
int checkPortParam(char* param);

//信息传输
int recvSentence(int connfd, char* sentence);
int sendMsg(int connfd, char* sentence);
int recvFile(int connfd, char* fileName);
int sendFile(int connfd, char* fileName);

//socket相关
int createSocket();
int bindSocketToServer(int sockfd, int port);
int setSocketOption(int sockfd);
int startSocketListening(int sockfd, int maxQueueLength);
void closeFileSockets(int* pFileConnfd, int* pPasvListenfd);
int sendConnectRequest(int sockfd, char* serverIp, int serverPort);

int getParamsFromCli(int argc, char**argv);
