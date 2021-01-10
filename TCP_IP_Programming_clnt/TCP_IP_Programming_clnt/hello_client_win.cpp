#define _WINSOCK_DEPRECATED_NO_WARNINGS //������ inet_addr ����
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <WinSock2.h>
using namespace std;

#define endl "\n"

int main(int argc, char* argv[]) //argc�� �ɼ��� ����, argv�� �ɼ� �迭
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	int strLen;
	char message[30];

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
		exit(1);
	}

	//winsock �ʱ�ȭ, ���α׷����� �䱸�ϴ� ������ ���� ������ �˸��� �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(�ֹ���, �ι���), WSADATA ������ �ּ� ��
		cout << "WSAStartup Error" << endl;

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 ��, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hSocket == INVALID_SOCKET) // -1 ����������� ����� ���°� �ƴ϶� �̷��� ��������� ������ ����ϸ� �������� ���� �Ŀ� ��ȭ�� �־ ��ó����
		cout << "socket Error" << endl;

	memset(&servAddr, 0, sizeof(servAddr)); //������ sin_zero �κ��� 0���� ä���ߵǴµ� ó������ 0���� ä��� �ʿ��� �պκп� ���� �� �Ҵ�
	servAddr.sin_family = AF_INET; //�ּ� ü��
	servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //IP�ּҸ� ��Ʈ��ũ ����Ʈ ������ ������ ��ȯ
	servAddr.sin_port = htons(atoi(argv[2])); //htons = host byte to network byte short(Ÿ������)

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //2��°���ڸ� SOCKADDR ����ȯ�� �̿��� IP�� Port��ȣ �Ҵ� �ѹ��� ���ֱ�
		cout << "connect Error" << endl;

	strLen = recv(hSocket, message, sizeof(message) - 1, 0); //���� -> ���� ����� ���� �ڵ� ��, ���ŵ� �����͸� ������ �ּ� ��, ������ �� �ִ� ����Ʈ ��, �ɼ�����
	if (strLen == -1)
		cout << "read Error" << endl;

	cout << "Message from server : " << message << endl;
	closesocket(hSocket);
	WSACleanup(); //���� ���̺귯�� �Ҵ� ����
	return 0;
}