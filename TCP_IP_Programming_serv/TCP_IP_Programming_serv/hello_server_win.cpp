#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <WinSock2.h>
#include <string>
using namespace std;

#define endl "\n"

int main(int argc, char* argv[]) //argc�� �ɼ��� ����, argv�� �ɼ� �迭
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello wolrd";

	//winsock �ʱ�ȭ, ���α׷����� �䱸�ϴ� ������ ���� ������ �˸��� �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(�ֹ���, �ι���), WSADATA ������ �ּ� ��
		cout << "WSAStartup Error" << endl;

	hServSock = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 ��, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hServSock == INVALID_SOCKET) // -1 ����������� ����� ���°� �ƴ϶� �̷��� ��������� ������ ����ϸ� �������� ���� �Ŀ� ��ȭ�� �־ ��ó����
		cout << "socket Error" << endl;

	string sPORT;
	cout << "PORT ��ȣ �Է� : ";
	cin >> sPORT;

	//�Ʒ� �Լ��� �̿��ϱ� ���� C Ÿ������ ����ȯ
	const char* PORT = sPORT.c_str();

	memset(&servAddr, 0, sizeof(servAddr)); //������ sin_zero �κ��� 0���� ä���ߵǴµ� ó������ 0���� ä��� �ʿ��� �պκп� ���� �� �Ҵ�
	servAddr.sin_family = AF_INET; //�ּ� ü��
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //���� å�̶� ���°� ���� �ٸ�. S_un ���Ŀ� addr�� ����, INADDR_ANY = ��ǻ���� IP�ּ� �ڵ��Ҵ�
	servAddr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(Ÿ������)

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //2��°���ڸ� SOCKADDR ����ȯ�� �̿��� IP�� Port��ȣ �Ҵ� �ѹ��� ���ֱ�
		cout << "bind Error" << endl;

	if (listen(hServSock, 5) == SOCKET_ERROR) //listen �Լ����ĺ��� Ŭ���̾�Ʈ���� connect����, 2��°���ڴ� ��� ť�� ���� ����
		cout << "listen Error" << endl;

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //
	if (hClntSock == INVALID_SOCKET)
		cout << "accept Error" << endl;

	send(hClntSock, message, sizeof(message), 0); //���� -> ���� ����� ���� �ڵ� ��, ������ �������� �ּ� ��, ������ ����Ʈ ��, �ɼ�����
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup(); //���� ���̺귯�� �Ҵ� ����
	return 0;
}