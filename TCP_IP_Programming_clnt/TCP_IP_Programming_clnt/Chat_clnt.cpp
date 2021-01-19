#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <cstring>
#include <process.h>
#include <string>
using namespace std;

#define endl "\n"
#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);

string sname = "Defalut";
char msg[BUF_SIZE];

int main(int argc, char* argv[]) //argc는 옵션의 개수, argv는 옵션 배열
{
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAddr;
	HANDLE hSndThread, hRcvThread;

	//winsock 초기화, 프로그램에서 요구하는 윈도우 소켓 버전을 알리고 해당 버전을 지원하는 라이브러리 초기화 작업
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(주버전, 부버전), WSADATA 변수의 주소 값
		cout << "WSAStartup error" << endl;

	cout << "이름 입력" << endl;
	cin >> sname;

	hSock = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 등, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hSock == INVALID_SOCKET) // -1 상수값이지만 상수로 쓰는게 아니라 이렇게 사용자정의 변수로 사용하면 가독성도 좋고 후에 변화가 있어도 대처가능
		cout << "socket error" << endl;

	string sIP, sPORT;
	cout << "IP주소 입력(루프백 IP는 127.0.0.1) : ";
	cin >> sIP;
	cout << "PORT 번호 입력 : ";
	cin >> sPORT;

	//아래 함수들을 이용하기 위해 C 타입으로 형변환
	const char* IP = sIP.c_str();
	const char* PORT = sPORT.c_str();

	memset(&servAddr, 0, sizeof(servAddr)); //소켓의 sin_zero 부분을 0으로 채워야되는데 처음부터 0으로 채우고 필요한 앞부분에 새로 값 할당
	servAddr.sin_family = AF_INET; //주소 체계
	servAddr.sin_addr.S_un.S_addr = inet_addr(IP); //IP주소를 네트워크 바이트 형태의 값으로 변환
	servAddr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(타입형태)

	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //2번째인자를 SOCKADDR 형변환을 이용해 IP와 Port번호 할당 한번에 해주기
		cout << "connect error" << endl;

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	closesocket(hSock);
	WSACleanup(); //윈속 라이브러리 할당 해제
	return 0;
}

//Send thread main
unsigned WINAPI SendMsg(void* arg)
{
	SOCKET hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSock);
			exit(0);
		}
		sprintf(nameMsg, "[%s] : %s", sname.c_str(), msg); //sprintf 포맷을 위해 char* 형태로 형변환
		send(hSock, nameMsg, strlen(nameMsg), 0);
	}

	return 0;
}

//Read thread main
unsigned WINAPI RecvMsg(void* arg)
{
	SOCKET hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	int strLen;

	while (1)
	{
		strLen = recv(hSock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		if (strLen == -1)
			return -1;

		nameMsg[strLen] = 0; // C style string
		fputs(nameMsg, stdout);
	}

	return 0;
}