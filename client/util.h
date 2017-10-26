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


//返回服务器返回的状态码，出错则返回-1
int getDigit(char*sentence)
{
	char digit[4];
	for(int i = 0; sentence[i] != ' '; i++)
	{
		digit[i] = sentence[i];
	}
	int digitInt = atoi(digit);
	return digitInt;
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
void strreplace(char*sentence, char src, char dest)
{
	for(int i = 0; i < strlen(sentence); i++)
	{
		if(sentence[i] == src)
			sentence[i] = dest;
	}
}
void getIPFromPasvResponse(char* response, char* serverIp)
{
	int counter = 0;
	int i = 0;
	int j = 0;
	while(response[i] != '(') 
		i++;
	for(j = i + 1; ;j++)
	{
		if(response[j] == ',')
			counter++;
		if(counter == 4)
			break;
		serverIp[j - i - 1] = response[j];
	}
	serverIp[j - i + 1] = '\0';
	strreplace(serverIp, ',', '.');
}

int getPortFromPasvResponse(char* response)
{
	int counter = 0;
	int i = 0, j = 0;
	for(i = 0; i < strlen(response); i++)
	{
		if(response[i] == ',')
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
	for(j = 1; response[i + j] != ',';j++)
	{
		p1c[j - 1] = response[i + j];
	}
	p1c[j - 1] = '\0';
	int k = j;
	for(j = j + 1; response[i + j] != ')'; j++)
	{
		p2c[j - k - 1] = response[i + j];
	}
	p2c[j - k - 1] = '\0';
	p1 = atoi(p1c);
	p2 = atoi(p2c);
	int port = p1 * 256 + p2;
	return port;
}