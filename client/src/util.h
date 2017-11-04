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


void strreplace(char*sentence, char src, char dest);
void intToString(int n, char* s);
void convertToUpperCase(char* sentence);
void removeLineFeed(char* sentence);

int getDigit(char*sentence);
int getCommandFromSentence(char* sentence, char* command, char* parameter);
void getIPFromPasvResponse(char* response, char* serverIp);
int getPortFromPasvResponse(char* response);
int getPortFromPortMsg(char* param, int* pClientPort);
void getFileNameFromPath(char* fileName ,char* filePath);

int recvFile(int connfd, char* fileName);
int sendFile(int connfd, char* fileName);

int createSocket();
int bindSocket(int sockfd, int port);
int setSocketOption(int sockfd);
int startSocketListening(int sockfd, int maxQueueLength);
int closeSocket(int* pSocket);
