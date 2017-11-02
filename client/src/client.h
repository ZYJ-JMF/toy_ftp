#pragma once
#include"common.h"
#include"handler.h"
#include"util.h"

#define NO_MODE 0
#define PASV_MODE 1
#define PORT_MODE 2

int getServerInfo();
int getUserInfo();

void showWelcomMsg();
void showHelpMsg();

int handleBrowseRequest(int sockfd, int* pFileSockfd, char* param);
int handleDownloadRequest(int sockfd, int* pFileSockfd, char* param);
int handleUploadRequest(int sockfd, int* pFileSockfd, char* param);
