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

int main(int argc, char* argv[]) //argc�� �ɼ��� ����, argv�� �ɼ� �迭
{
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAddr;
	HANDLE hSndThread, hRcvThread;

	//winsock �ʱ�ȭ, ���α׷����� �䱸�ϴ� ������ ���� ������ �˸��� �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(�ֹ���, �ι���), WSADATA ������ �ּ� ��
		cout << "WSAStartup error" << endl;

	cout << "�̸� �Է�" << endl;
	cin >> sname;

	hSock = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 ��, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hSock == INVALID_SOCKET) // -1 ����������� ����� ���°� �ƴ϶� �̷��� ��������� ������ ����ϸ� �������� ���� �Ŀ� ��ȭ�� �־ ��ó����
		cout << "socket error" << endl;

	string sIP, sPORT;
	cout << "IP�ּ� �Է�(������ IP�� 127.0.0.1) : ";
	cin >> sIP;
	cout << "PORT ��ȣ �Է� : ";
	cin >> sPORT;

	//�Ʒ� �Լ����� �̿��ϱ� ���� C Ÿ������ ����ȯ
	const char* IP = sIP.c_str();
	const char* PORT = sPORT.c_str();

	memset(&servAddr, 0, sizeof(servAddr)); //������ sin_zero �κ��� 0���� ä���ߵǴµ� ó������ 0���� ä��� �ʿ��� �պκп� ���� �� �Ҵ�
	servAddr.sin_family = AF_INET; //�ּ� ü��
	servAddr.sin_addr.S_un.S_addr = inet_addr(IP); //IP�ּҸ� ��Ʈ��ũ ����Ʈ ������ ������ ��ȯ
	servAddr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(Ÿ������)

	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //2��°���ڸ� SOCKADDR ����ȯ�� �̿��� IP�� Port��ȣ �Ҵ� �ѹ��� ���ֱ�
		cout << "connect error" << endl;

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	closesocket(hSock);
	WSACleanup(); //���� ���̺귯�� �Ҵ� ����
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
		sprintf(nameMsg, "[%s] : %s", sname.c_str(), msg); //sprintf ������ ���� char* ���·� ����ȯ
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