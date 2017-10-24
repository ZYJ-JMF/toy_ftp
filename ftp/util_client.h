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
	for(int i = 0; i <= 2; i++)
	{
		digit[i] = sentence[i];
	}
	int digitInt = atoi(digit);
	return digitInt;
}