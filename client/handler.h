#include"util.c"

int sendConnectRequest(int sockfd, char* serverIp, int serverPort);

int sendUserRequest(int sockfd, char* user);
int sendPassRequest(int sockfd, char*password);
int sendSystRequest(int sockfd);
int sendTypeRequest(int sockfd);
int sendRetrRequest(int sockfd, char* fileName);
int sendStorRequest(int sockfd, char* fileName);
int sendPasvRequest(int sockfd);
int sendPortRequest(int sockfd, char* param);

int handlePassResponse(int sockfd);
int handleUserResponse(int sockfd);
int handlePasvResponse(int sockfd,int* pFileSockfd);
int handlePortResponse(int sockfd, char* param, int* pListenFd);
int handleSystResponse(int sockfd);
int handleConnectResponse(int sockfd);
int handleStorResponse(int sockfd, int fileSockfd, char* fileName);
int handleRetrResponse(int sockfd, int fileSockfd, char* fileName);
int handleTypeResponse(int sockfd);

int acceptConnect(int listenfd);