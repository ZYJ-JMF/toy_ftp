#pragma once

//服务器的回复字符串
extern char* startTransferPart;
extern char* typeIMsg;
extern char* typeAMsg;
extern char* portMsg;
extern char* sysStatusMsg;
extern char* welcomeMsg;
extern char* goodbyeMsg;
extern char* fileSentMsg;
extern char* directorySentMsg;
extern char* pasvMsgPart;
extern char* loginSuccessMsg;
extern char* mkdSuccessMsgPart;
extern char* rmdSuccessMsgPart;
extern char* cwdSuccessMsgPart;
extern char* pwdSuccessMsgPart;

extern char* requirePassMsg;

extern char* cannotConnectError;
extern char* noPortError;
extern char* networkError;
extern char* readDirError;
extern char* retrError;

extern char* syntaxError;
extern char* typeError;
extern char* portSyntaxError;
extern char* unknownUserError;
extern char* invalidPathError;
extern char* noAvailablePortError;
extern char* notLoginError;
extern char* fileNotExistError;
extern char* mkdFailError;
extern char* rmdFailError;
extern char* cwdFailError;

//服务器的配置
extern int port; 
extern char rootPath[1000];
extern char serverIp[100];