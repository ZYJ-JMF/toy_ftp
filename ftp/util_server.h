#include <sys/socket.h>
#include <netinet/in.h>
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
	printf("receive %d chars from client\n", n);
	sentence[n] = '\0';
    return 1;
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
//向client发送信息
void sendMsg(int connfd, char* sentence)
{
	int len = strlen(sentence);
	int n = send(connfd, sentence, len + 1, 0);
	if(n < 0)
	{
		printf("Error send():%s(%d)\n" , strerror(errno), errno);
	}
}

int checkPortParam(char* param)
{
	return 1;
}