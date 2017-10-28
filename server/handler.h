#pragma once

int handleFirstConnectRequest(int connfd);

int handleInputSentence(int connfd, char* sentence);
int parseInputSentence(char* sentence, char* command, char* param);

int handleUserRequest(int connfd, char* param);
int handlePassRequest(int connfd, char* param);
int handleSystRequest(int connfd, char* param);
int handleTypeRequest(int connfd, char* param);
int handlePortRequest(int connfd, char* param, char* clientIp, int* port);
int handlePasvRequest(int connfd, char* param, int* psockfd);

int connectToClient(int connfd, int* pFileConnfd, char* clientIp, int clientPort);
int handleStorRequest(int connfd, int fileConnfd, char* param);
int handleRetrRequest(int connfd, int fileConnfd, char* param);
