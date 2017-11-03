#include "handler.h"

int handleFirstConnectRequest(int connfd)
{
	if(sendMsg(connfd, welcomeMsg) == 1)
		return 1;
	else
		return -1;
}

int handleInputSentence(int connfd, char* sentence)
{
	memset(sentence, 0, strlen(sentence));
	if(recvSentence(connfd, sentence) < 0)
		return -1;
    removeLineFeed(sentence);
    printf("sentence: %s\n", sentence);
    return 1;
}

int parseInputSentence(char* sentence, char* command, char* param)
{
	memset(command, 0, strlen(command));
	memset(param, 0, strlen(param));
    if(getCommandFromSentence(sentence, command, param) < 0)
    	return -1;
    convertToUpperCase(command);
    printf("command: %s\n", command);
    printf("param: %s\n", param);
    return 1;
}

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

int handlePassRequest(int connfd, char* param)
{
	if(checkPassParam(param) == -1)
	{
		sendMsg(connfd, typeError);
		return -1;
	}
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
		sendMsg(connfd, typeIMsg);
		return -1;
	}
	else if(strcmp(param, "A") == 0)
	{
		sendMsg(connfd, typeAMsg);
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
		printf("Client IP is %s\n", clientIp);
		printf("Client port is %d\n", *pClientPort);
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
	char pasvMsg[200];
	memset(pasvMsg, 0, 200);
	makePasvPortMsg(pasvMsg, pasvPort);
	if(sendMsg(connfd, pasvMsg) == -1)
		return -1;
	return 1;
}

int handleStorRequest(int connfd, int fileConnfd, char* param, char* pWorkingDir)
{
	if(checkPath(param) == -1)
	{
		sendMsg(connfd, invalidPathError);
		return -1;
	}
	char startTransferPasv[400];
	memset(startTransferPasv, 0, strlen(startTransferPasv));
	makeStartTransferMsg(startTransferPasv, param);
	if(sendMsg(connfd, startTransferPasv) == -1)
		return -1;
	char filePath[300];
	memset(filePath, 0, strlen(filePath));
	makeAbsolutePath(filePath, pWorkingDir, param);
	int state = recvFile(fileConnfd, filePath);
	if(state == 1)
	{
		sendMsg(connfd, fileSentMsg);
		return 1;
	}
	else
		return -1;
}

int handleRetrRequest(int connfd, int fileConnfd, char* param, char* pWorkingDir)
{
	if(checkPath(param) == -1)
	{
		sendMsg(connfd, invalidPathError);
		return -1;
	}
	char startTransferPasv[400];
	memset(startTransferPasv, 0, strlen(startTransferPasv));
	makeStartTransferMsg(startTransferPasv, param);
	if(sendMsg(connfd, startTransferPasv) == -1)
		return -1;
	char filePath[300];
	memset(filePath, 0, strlen(filePath));
	makeAbsolutePath(filePath, pWorkingDir, param);
	int state = sendFile(fileConnfd, filePath);
	if(state == 1)
	{
		sendMsg(connfd, fileSentMsg);
		return 1;
	}
	else
	{
		sendMsg(connfd, networkError);
		return -1;
	}
}

int handleListRequest(int connfd, int fileConnfd, char* param, char* pWorkingDir)
{
	char listTargetPath[100];
	memset(listTargetPath, 0, strlen(listTargetPath));
	if(param[0] == '\0' || strcmp(param, "-a") == 0)
		strcpy(listTargetPath, pWorkingDir);
	else
		makeAbsolutePath(listTargetPath, pWorkingDir, param);
	if(checkListParam(listTargetPath) == 0)
		return -1;
	char listData[8192];
	memset(listData, 0, 8192);
	char startTransferMsg[400];
	memset(startTransferMsg, 0, strlen(startTransferMsg));
	makeStartTransferMsg(startTransferMsg, listTargetPath);
	sendMsg(connfd, startTransferMsg);
	
	DIR* dir;
	dir = opendir(listTargetPath);
	//根据是文件还是目录传送不同数据
	if(dir != NULL)
	{
		if(sendDirectoryInfo(fileConnfd, listTargetPath, listData) == -1)
		{
			sendMsg(connfd, invalidPathError);
			return -1;
		}
		else
			sendMsg(fileConnfd, listData);
	}
	else
	{
		if(sendFileInfo(fileConnfd, listTargetPath, param, listData) == -1)
		{
			sendMsg(connfd, invalidPathError);
			return -1;
		}
		else
			sendMsg(fileConnfd, listData);
	}
	printf("List data is %s" , listData);
	sendMsg(connfd, fileSentMsg);
	return 1;
}

int handleNlstRequest(int connfd, int fileConnfd, char* param, char* pWorkingDir)
{
	char listTargetPath[100];
	memset(listTargetPath, 0, strlen(listTargetPath));
	if(param[0] == '\0')
		strcpy(listTargetPath, pWorkingDir);
	else
		makeAbsolutePath(listTargetPath, pWorkingDir, param);
	if(checkListParam(listTargetPath) == 0)
	{
		return -1;
	}
	char listData[8192];
	memset(listData, 0, strlen(listData));
	char startTransferMsg[400];
	memset(startTransferMsg, 0, strlen(startTransferMsg));
	makeStartTransferMsg(startTransferMsg, listTargetPath);
	sendMsg(connfd, startTransferMsg);

	DIR* dir;
	dir = opendir(listTargetPath);
	//根据是文件还是目录传送不同数据
	if(dir != NULL)
	{
		if(sendDirectoryInfoSimple(fileConnfd, listTargetPath, listData) == -1)
		{
			sendMsg(connfd, invalidPathError);
			return -1;
		}
		else
			sendMsg(fileConnfd, listData);
	}
	else
	{
		if(sendFileInfoSimple(fileConnfd, listTargetPath, param, listData) == -1)
		{
			sendMsg(connfd, invalidPathError);
			return -1;
		}
		else
			sendMsg(fileConnfd, listData);
	}
	sendMsg(connfd, fileSentMsg);
	return 1;
}
int connectToClient(int connfd, int* pFileConnfd, char* clientIp, int clientPort)
{
	*pFileConnfd = createSocket();
	if(sendConnectRequest(*pFileConnfd, clientIp, clientPort) == -1)
		return -1;
	return 1;
}

int handleMkdRequest(int connfd, char* param, char* pWorkingDir)
{
	if(checkPath(param) == -1)
	{
		sendMsg(connfd, invalidPathError);
		return -1;
	}
	char filePath[200];
	memset(filePath, 0, strlen(filePath));
	makeAbsolutePath(filePath, pWorkingDir, param);
	DIR* dir = opendir(filePath);
	if(dir)
	{
		sendMsg(connfd, mkdFailError);
		return -1;
	}
	else if(mkdir(filePath, 0777) == -1)
	{
		sendMsg(connfd, mkdFailError);
		return -1;
	}
	else
	{
		char mkdSuccessMsg[100];
		memset(mkdSuccessMsg, 0, strlen(mkdSuccessMsg));
		char endOfLine[10] = "\r\n";
		strcat(mkdSuccessMsg, mkdSuccessMsgPart);
		strcat(mkdSuccessMsg, filePath);
		strcat(mkdSuccessMsg, endOfLine);
		sendMsg(connfd, mkdSuccessMsg);
	}
	return 1;
}

int handleRmdRequest(int connfd, char* param, char* pWorkingDir)
{
	if(checkPath(param) == -1)
	{
		sendMsg(connfd, invalidPathError);
		return -1;
	}
	char filePath[200];
	memset(filePath, 0, strlen(filePath));
	makeAbsolutePath(filePath, pWorkingDir, param);
	int status = rmdir(filePath);
	if(status == -1)
	{
		sendMsg(connfd, rmdFailError);
		return -1;
	}
	else if(status == 0)
	{
		char rmdSuccessMsg[100];
		memset(rmdSuccessMsg, 0, strlen(rmdSuccessMsg));
		char endOfLine[10] = "\r\n";
		strcat(rmdSuccessMsg, rmdSuccessMsgPart);
		strcat(rmdSuccessMsg, filePath);
		strcat(rmdSuccessMsg, endOfLine);
		sendMsg(connfd, rmdSuccessMsg);
	}
	return 1;
}

int handleCwdRequest(int connfd, char* param, char* pWorkingDir)
{
	char filePath[200];
	memset(filePath, 0, 200);
	if(param[0] != '/')
		makeAbsolutePath(filePath, pWorkingDir, param);
	else
		strcpy(filePath, param);
	DIR* dir = opendir(filePath);
	if(!dir)
	{
		sendMsg(connfd, cwdFailError);
		return -1;
	}
	else
	{
		char cwdSuccessMsg[100];
		char endOfLine[10] = "\r\n";
		memset(cwdSuccessMsg, 0, strlen(cwdSuccessMsg));
		memset(pWorkingDir, 0, strlen(pWorkingDir));
		strcpy(pWorkingDir, filePath);
		strcat(cwdSuccessMsg, cwdSuccessMsgPart);
		strcat(cwdSuccessMsg, filePath);
		strcat(cwdSuccessMsg, endOfLine);
		sendMsg(connfd, cwdSuccessMsg);
	}
	return 1;	
}

int handlePwdRequest(int connfd, char* param, char* pWorkingDir)
{
	char pwdSuccessMsg[100];
	char quotation[4] = "\"";
	memset(pwdSuccessMsg, 0, 100);
	char endOfLine[10] = "\r\n";
	char blank[2] = " ";
	strcat(pwdSuccessMsg, pwdSuccessMsgPart);
	strcat(pwdSuccessMsg, quotation);
	strcat(pwdSuccessMsg, pWorkingDir);
	strcat(pwdSuccessMsg, quotation);
	strcat(pwdSuccessMsg, blank);
	strcat(pwdSuccessMsg, endOfLine);
	sendMsg(connfd, pwdSuccessMsg);
	return 1;
}