#pragma once
#include"util.h"
#include"common.h"

int sendConnectRequest(int sockfd, char* serverIp, int serverPort);

int sendUserRequest(int sockfd, char* user);
int sendPassRequest(int sockfd, char*password);
int sendSystRequest(int sockfd);
int sendTypeRequest(int sockfd);
int sendRetrRequest(int sockfd, char* param);
int sendStorRequest(int sockfd, char* param);
int sendListRequest(int sockfd, char* param);
int sendNlstRequest(int sockfd, char* param);
int sendPasvRequest(int sockfd);
int sendPortRequest(int sockfd, char* param);
int sendMkdRequest(int sockfd, char* param);
int sendRmdRequest(int sockfd, char* param);
int sendCwdRequest(int sockfd, char* param);
int sendPwdRequest(int sockfd, char* param);

/* --------------------------------------------*/

int handlePassResponse(int sockfd);
int handleUserResponse(int sockfd);
int handlePasvResponse(int sockfd,int* pFileSockfd);
int handlePortResponse(int sockfd, char* param, int* pListenFd);
int handleSystResponse(int sockfd);
int handleConnectResponse(int sockfd);
int handleStorResponse(int sockfd, int fileSockfd, char* param);
int handleRetrResponse(int sockfd, int fileSockfd, char* param);
int handleListResponse(int sockfd, int fileSockfd, char* param);
int handleNlstResponse(int sockfd, int fileSockfd, char* param);
int handleTypeResponse(int sockfd);
int handleMkdResponse(int sockfd);
int handleRmdResponse(int sockfd);
int handleCwdResponse(int sockfd);
int handlePwdResponse(int sockfd);

int acceptConnect(int listenfd);