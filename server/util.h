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
#define VALUE_PORT 12345
#define VALUE_ROOT 54321

#pragma once
//字符串中字符替换，将所有src替换为dest
void strreplace(char*sentence, char src, char dest)
{
	for(int i = 0; i < strlen(sentence); i++)
	{
		if(sentence[i] == src)
			sentence[i] = dest;
	}
}
void intToString(int n, char* s)
{
	int integer = n;
	int r = 0;
	int len = 1;
	while(integer / 10 != 0)
	{
		integer /= 10;
		len++;
	}
	integer = n;
	for(int i = len - 1; i >= 0; i--)
	{
		s[i] = integer % 10 + '0';
		integer /= 10;
	}
	s[len] = '\0';
}

void convertToUpperCase(char* sentence)
{
	int len = strlen(sentence);
	for (int p = 0; p < len; p++) 
		sentence[p] = toupper(sentence[p]);
}
//删去命令末尾的'\n'
void removeLineFeed(char* sentence)
{
	int len = strlen(sentence);
	if(sentence[len - 1] == '\n' || sentence[len - 1] == '\r')
	{
		sentence[len - 1] = '\0';
	}
}

//TODO:多次读取
int getSentence(int connfd, char* sentence)
{
	int n;
	n = recv(connfd, sentence, 8191 , 0);
	if(n < 0)
	{
		printf("Error read(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	sentence[n] = '\0';
    return 1;
}

//从sentence中得到前面的command和parameter,出错返回-1
int getCommand(char* sentence, char* command, char* parameter)
{
	int len = strlen(sentence);
	int spacePos = -1;
	for(int i = 0; i < len; i++)
	{
		if(sentence[i] == ' ')
		{
			spacePos = i;
			break;
		}
	}
	if(spacePos > 0)
	{
		for(int i = 0; i < spacePos; i++)
		{
			command[i] = sentence[i];
		}
		command[spacePos] = '\0';
		for(int i = spacePos + 1; i <= len; i++)
		{
			parameter[i - spacePos - 1] = sentence[i];
		}
	}
	else
	{
		for(int i = 0; i <= len; i++)
		{
			command[i] = sentence[i];
		}
	}
	return 1;
}

int getIpFromPortMsg(char* param, char* clientIp)
{
	int counter = 0;
	int i = 0;
	while(1)
	{
		if(param[i] == '\0')
		{
			printf("format error.\n");
			return -1;
		}
		if(counter == 4)
		{
			clientIp[i - 1] = '\0';
			break;
		}
		if(param[i] == ',')
		{
			clientIp[i] = '.';
			counter++;
		}
		else
		{
			clientIp[i] = param[i];
		}
		i++;
	}
	return 1;
}

int getPortFromPortMsg(char* param, int* pClientPort)
{
	int counter = 0;
	int i = 0, j = 0;
	for(i = 0; i < strlen(param); i++)
	{
		if(param[i] == ',')
		{
			counter++;
		}
		if(counter == 4)
		{
			break;
		}
	}
	int p1;
	int p2;
	char p1c[10];
	char p2c[10];
	for(j = 1; param[i + j] != ',';j++)
	{
		p1c[j - 1] = param[i + j];
	}
	p1c[j - 1] = '\0';
	int k = j;
	for(j = j + 1; param[i + j] != ')'; j++)
	{
		p2c[j - k - 1] = param[i + j];
	}
	p2c[j - k - 1] = '\0';
	p1 = atoi(p1c);
	p2 = atoi(p2c);
	*pClientPort = p1 * 256 + p2;
	return 1;
}

//向client发送信息
int sendMsg(int connfd, char* sentence)
{
	int len = strlen(sentence);
	int n = send(connfd, sentence, len + 1, 0);
	if(n < 0)
	{
		printf("Error send():%s(%d)\n" , strerror(errno), errno);
		return -1;
	}
	return 1;
}

int recvFile(int connfd, char* fileName)
{
	FILE* f = fopen(fileName, "wb");
	char buffer[8192];
	int size;

	while(1)
	{
		size = read(connfd, buffer, 8191);
		if(size <= 0)
			break;
		fwrite(buffer, 1, size, f);
		printf("receive %d bytes.\n", size);
	}

	fclose(f);
	return 1;
}
int sendFile(int connfd, char* fileName)
{
	FILE * f = fopen(fileName, "rb");
	char buffer[8192];
	int size;
	while(1)
	{
		size = fread(buffer, 1, 8190, f);
		if(size <= 0)
			break;
		send(connfd, buffer, size, 0);
	}
	return 1;
}
int checkPortParam(char* param)
{
	return 1;
}

//建立新的socket，返回该socket的描述符
int createSocket()
{
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
	{
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	printf("socketfd is: %d\n", sockfd);
	return sockfd;
}
//将socket绑定到服务器指定端口
int bindSocketToServer(int sockfd, int port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
	{
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return -1;	
	}
	return 1;
}
//设置socket为可立即复用
int setSocketOption(int sockfd)
{
	int reuse = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse) == -1)
	{
		printf("Error set(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

//10是最大可以排队连接的个数，在这里（并发情况下）应该无意义
int startSocketListening(int sockfd, int maxQueueLength)
{
	if (listen(sockfd, maxQueueLength) == -1) 
	{
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	return 1;
}

void closeFileSockets(int* pFileConnfd, int* pPasvListenfd)
{
	if(*pFileConnfd != -1)
	{
		close(*pFileConnfd);
		*pFileConnfd = -1;
	}
	if(*pPasvListenfd != -1)
	{
		close(*pPasvListenfd);
		*pPasvListenfd = -1;
	}
}