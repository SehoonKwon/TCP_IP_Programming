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

int main(int argc, char* argv[]) //argc�� �ɼ��� ����, argv�� �ɼ� �迭
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	HANDLE hThread;
	int clntAdrsz;

	//winsock �ʱ�ȭ, ���α׷����� �䱸�ϴ� ������ ���� ������ �˸��� �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(�ֹ���, �ι���), WSADATA ������ �ּ� ��
		cout << "WSAStartup error" << endl;

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 ��, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hServSock == INVALID_SOCKET) // -1 ����������� ����� ���°� �ƴ϶� �̷��� ��������� ������ ����ϸ� �������� ���� �Ŀ� ��ȭ�� �־ ��ó����
		cout << "socket error" << endl;

	string sIP, sPORT;
	cout << "IP�ּ� �Է�(������ IP�� 127.0.0.1) : ";
	cin >> sIP;
	cout << "PORT ��ȣ �Է� : ";
	cin >> sPORT;

	//�Ʒ� �Լ����� �̿��ϱ� ���� C Ÿ������ ����ȯ
	const char* IP = sIP.c_str();
	const char* PORT = sPORT.c_str();

	memset(&servAdr, 0, sizeof(servAdr)); //������ sin_zero �κ��� 0���� ä���ߵǴµ� ó������ 0���� ä��� �ʿ��� �պκп� ���� �� �Ҵ�
	servAdr.sin_family = AF_INET; //�ּ� ü��
	servAdr.sin_addr.S_un.S_addr = inet_addr(IP); //IP�ּҸ� ��Ʈ��ũ ����Ʈ ������ ������ ��ȯ
	servAdr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(Ÿ������)

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) //2��°���ڸ� SOCKADDR ����ȯ�� �̿��� IP�� Port��ȣ �Ҵ� �ѹ��� ���ֱ�
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
	WSACleanup(); //���� ���̺귯�� �Ҵ� ����
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