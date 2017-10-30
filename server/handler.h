#pragma once
#include "common.h"
#include "util.h"

int handleFirstConnectRequest(int connfd);

int handleInputSentence(int connfd, char* sentence);
int parseInputSentence(char* sentence, char* command, char* param);

int handleUserRequest(int connfd, char* param);
int handlePassRequest(int connfd, char* param);
int handleSystRequest(int connfd, char* param);
int handleTypeRequest(int connfd, char* param);
int handlePortRequest(int connfd, char* param, char* clientIp, int* port);
int handlePasvRequest(int connfd, char* param, int* psockfd);
int handleMkdRequest(int connfd, char* param, char* pWorkingDir);
int handleRmdRequest(int connfd, char* param, char* pWorkingDir);
int handleCwdRequest(int connfd, char* param, char* pWorkingDir);

int connectToClient(int connfd, int* pFileConnfd, char* clientIp, int clientPort);
int handleStorRequest(int connfd, int fileConnfd, char* param, char* pWorkingDir);
int handleRetrRequest(int connfd, int fileConnfd, char* param, char* pWorkingDir);
