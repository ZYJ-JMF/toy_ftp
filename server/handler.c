#include "handler.h"


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
	if(recvSentence(connfd, sentence) < 0)
		return -1;
    removeLineFeed(sentence);
    printf("sentence: %s\n", sentence);
    return 1;
}

int parseInputSentence(char* sentence, char* command, char* param)
{
	command[0] = '\0';
	param[0] ='\0';
    if(getCommandFromSentence(sentence, command, param) < 0)
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
		printf("Client IP is %s.\n", clientIp);
		printf("Client port is %d.\n", *pClientPort);
		if(sendMsg(connfd, portMsg) == -1)
			return -1;
	}
	return 1;
}

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
	char pasvMsg[200] = "\0";
	makePasvPortMsg(pasvMsg, pasvPort);
	if(sendMsg(connfd, pasvMsg) == -1)
		return -1;
	return 1;
}

//TODO:处理错误情况
int handleStorRequest(int connfd, int fileConnfd, char* param)
{
	char startTransferPasv[400];
	makeStartTransferMsg(startTransferPasv, param);
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

int handleRetrRequest(int connfd, int fileConnfd, char* param)
{
	char startTransferPasv[400];
	makeStartTransferMsg(startTransferPasv, param);
	if(sendMsg(connfd, startTransferPasv) == -1)
		return -1;
	int state = sendFile(fileConnfd, param);
	if(state == 1)
	{
		sendMsg(connfd, fileSentMsg);
		printf("successfully sent.\n");
		return 1;
	}
	else
		return -1;
}
int connectToClient(int connfd, int* pFileConnfd, char* clientIp, int clientPort)
{
	*pFileConnfd = createSocket();
	if(sendConnectRequest(*pFileConnfd, clientIp, clientPort) == -1)
		return -1;
	return 1;
}
