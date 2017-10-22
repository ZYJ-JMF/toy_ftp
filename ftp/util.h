/* 定义一些字符串处理函数*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>

#pragma once

//得到输入命令,返回命令长度
//TODO:限制最大长度
int getSentence(int connfd, char* sentence)
{
	int p = 0;
	while (1)
    {
		int n = read(connfd, sentence + p, 8191 - p);
		if (n <= 0) 
		{
			printf("Error read(): %s(%d)\n", strerror(errno), errno);
			return -1;
		} 
		else if (n == 0) 
			continue;
		else 
		{
			p += n;
			if (sentence[p - 1] == '\n') 
				break;
		}
	}
	sentence[p - 1] = '\0';
	return p - 1;
}

void convertToUpperCase(char* sentence)
{
	int len = strlen(sentence);
	for (int p = 0; p < len; p++) 
		sentence[p] = toupper(sentence[p]);
}
//从sentence中得到前面的command和parameter,出错返回-1
//保证两个字符串最后是"\0"
int getCommand(char* sentence, char* command, char* parameter)
{
	int len = strlen(sentence);
	printf("%d\n", len);
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
int sendMsg(int connfd, char* sentence)
{
	int len = strlen(sentence);
	int n = send(connfd, sentence, len + 1, 0);
	if(n < 0)
	{
		printf("Error send():%s(%d)\n" , strerror(errno), errno);
	}
	return n;
}