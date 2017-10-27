#include "handler.h"
#include "reply.h"
#include "util.h"

int handleFirstConnectRequest(int connfd)
{
	if(sendMsg(connfd, welcomeMsg) == 1)
	{
		printf("Welcome message sent.\n");
		return 1;
	}
	else
	{
		printf("Error send welcome.\n");
		return -1;
	}
}

int handleInputSentence(int connfd, char* sentence)
{
	if(getSentence(connfd, sentence) < 0)
		return -1;
    removeLineFeed(sentence);
    printf("sentence: %s\n", sentence);
    return 1;
}

int parseInputSentence(char* sentence, char* command, char* param)
{
	command[0] = '\0';
	param[0] ='\0';
    if(getCommand(sentence, command, param) < 0)
    	return -1;
    convertToUpperCase(command);
    printf("command: %s\n", command);
    printf("param: %s\n", param);
    return 1;
}

//登录成功返回1，失败返回-1
//TODO:建立用户表
int handleUserRequest(int connfd, char* param)
{
	if(strcmp(param, "anonymous") == 0)
    {
	   	if(sendMsg(connfd, requirePassMsg) == -1)
	   		return -1;
	   	return 1;
	}
	else
	{
	    sendMsg(connfd, unknownUserError);
	    return -1;
	}
}

//TODO:检查密码格式
int handlePassRequest(int connfd, char* param)
{
	if(sendMsg(connfd, loginSuccessMsg) == -1)
		return -1;
	return 1;
}

int handleSystRequest(int connfd, char* param)
{
	if(sendMsg(connfd, sysStatusMsg))
		return -1;
	return 1;
}

int handleTypeRequest(int connfd, char* param)
{
	if(strcmp(param, "I") == 0)
	{
		sendMsg(connfd, typeMsg);
		return -1;
	}
	else if(strcmp(param, "") == 0)
	{
		sendMsg(connfd, syntaxError);
		return -1;
	}
	else
		sendMsg(connfd, typeError);
	return 1;
}

int handlePortRequest(int connfd, char* param, char* clientIp, int* pClientPort)
{
	if(checkPortParam(param) == -1)
	{
		sendMsg(connfd, typeError);
		return -1;
	}
	else
	{
		getIpFromPortMsg(param, clientIp);
		getPortFromPortMsg(param, pClientPort);
		printf("Client IP is %s\n", param);
		printf("Client port is %d\n", *pClientPort);
		if(sendMsg(connfd, portMsg) == -1)
			return -1;
	}
	return 1;
}

//TODO:把底层函数抽到util.h中
int handlePasvRequest(int connfd, char* param, int* psockfd)
{
	int pasvPort = 30000;
	*psockfd = createSocket();
	while(1)
	{
		if(bindSocketToServer(*psockfd, pasvPort) > 0)
		{
			setSocketOption(*psockfd);
			startSocketListening(*psockfd, 10);
			break;
		}
		else
			pasvPort += 1;
		if(pasvPort >= 65535)
		{
			sendMsg(connfd, noAvailablePortError);
			return -1;
		}
	}
	printf("Listening port: %d\n", pasvPort);
	char serverIpWithComma[80];
	strcpy(serverIpWithComma, serverIp);
	strreplace(serverIpWithComma, '.', ',');
	int p1 = pasvPort / 256;
	int p2 = pasvPort % 256;
	char leftBracket[2] = "(";
	char rightBracket[2] = ")";
	char comma[2] = ",";
	char endOfLine[10] = "\r\n";
	char p1c[10];
	char p2c[10];
	intToString(p1, p1c);
	intToString(p2, p2c);
	char pasvMsg[200] = "\0";
	strcat(pasvMsg, pasvMsgPart);
	strcat(pasvMsg, leftBracket);
	strcat(pasvMsg, serverIpWithComma);
	strcat(pasvMsg, comma);
	strcat(pasvMsg, p1c);
	strcat(pasvMsg, comma);
	strcat(pasvMsg, p2c);
	strcat(pasvMsg, rightBracket);
	strcat(pasvMsg, endOfLine);
	if(sendMsg(connfd, pasvMsg) == -1)
		return -1;
	return 1;
}

//TODO:处理错误情况
int handleStorRequestPasv(int connfd, int fileConnfd, char* param)
{
	printf("prepare to send msg to client.\n");
	char startTransferPasv[400];
	char endOfLine[10] = "\r\n";
	startTransferPasv[0] = '\0';
	strcat(startTransferPasv, startTransferPart);
	strcat(startTransferPasv, param);
	strcat(startTransferPasv, endOfLine);
	if(sendMsg(connfd, startTransferPasv) == -1)
		return -1;
	int state = recvFile(fileConnfd, param);
	if(state == 1)
	{
		sendMsg(connfd, fileSentMsg);
		printf("successfully received.\n");
		return 1;
	}
	else
		return -1;
}