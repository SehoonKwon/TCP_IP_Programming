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
#define MAX_CLNT 256
#define NAME_SIZE 20

unsigned WINAPI HandleClnt(void* arg);
void SendMsg(char* msg, int len);

int clntCnt;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main(int argc, char* argv[]) //argc는 옵션의 개수, argv는 옵션 배열
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	HANDLE hThread;
	int clntAdrsz;

	//winsock 초기화, 프로그램에서 요구하는 윈도우 소켓 버전을 알리고 해당 버전을 지원하는 라이브러리 초기화 작업
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(주버전, 부버전), WSADATA 변수의 주소 값
		cout << "WSAStartup error" << endl;

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 등, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hServSock == INVALID_SOCKET) // -1 상수값이지만 상수로 쓰는게 아니라 이렇게 사용자정의 변수로 사용하면 가독성도 좋고 후에 변화가 있어도 대처가능
		cout << "socket error" << endl;

	string sIP, sPORT;
	cout << "IP주소 입력(루프백 IP는 127.0.0.1) : ";
	cin >> sIP;
	cout << "PORT 번호 입력 : ";
	cin >> sPORT;

	//아래 함수들을 이용하기 위해 C 타입으로 형변환
	const char* IP = sIP.c_str();
	const char* PORT = sPORT.c_str();

	memset(&servAdr, 0, sizeof(servAdr)); //소켓의 sin_zero 부분을 0으로 채워야되는데 처음부터 0으로 채우고 필요한 앞부분에 새로 값 할당
	servAdr.sin_family = AF_INET; //주소 체계
	servAdr.sin_addr.S_un.S_addr = inet_addr(IP); //IP주소를 네트워크 바이트 형태의 값으로 변환
	servAdr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(타입형태)

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) //2번째인자를 SOCKADDR 형변환을 이용해 IP와 Port번호 할당 한번에 해주기
		cout << "bind error" << endl;

	if (listen(hServSock, 5) == SOCKET_ERROR)
		cout << "listen error" << endl;

	while (1)
	{
		clntAdrsz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrsz);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
		cout << "Connected client IP : " << inet_ntoa(clntAdr.sin_addr) << endl;
	}

	closesocket(hServSock);
	WSACleanup(); //윈속 라이브러리 할당 해제
	return 0;
}

//HandleClnt thread main
unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0;
	char msg[BUF_SIZE];

	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
		SendMsg(msg, strLen);

	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < clntCnt; i++)
	{
		if (hClntSock == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}

//Send func
void SendMsg(char* msg, int len)
{
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
	ReleaseMutex(hMutex);
}