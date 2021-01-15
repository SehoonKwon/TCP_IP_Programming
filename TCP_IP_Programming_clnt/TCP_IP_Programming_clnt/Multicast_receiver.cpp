#define _WINSOCK_DEPRECATED_NO_WARNINGS // for use inet_addr
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>
#include <WS2tcpip.h> // for struct ip_mreq
#include <string>
using namespace std;

#define endl "\n"
#define BUF_SIZE 30

int main()
{
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKADDR_IN Adr;
	ip_mreq joinAdr;

	char buf[BUF_SIZE];
	int strLen;

	//winsock �ʱ�ȭ, ���α׷����� �䱸�ϴ� ������ ���� ������ �˸��� �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(�ֹ���, �ι���), WSADATA ������ �ּ� ��
		cout << "WSAStartup error" << endl;

	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0); // ��Ƽĳ��Ʈ�� UDP ����
	if (hRecvSock == INVALID_SOCKET) // -1 ����������� ����� ���°� �ƴ϶� �̷��� ��������� ������ ����ϸ� �������� ���� �Ŀ� ��ȭ�� �־ ��ó����
		cout << "socket error" << endl;

	string sGroupIP, sPORT;
	cout << "Group IP �Է� : "; //�׷� IP�� 127.0.0.1 ������ IP�� ������ �ؿ� setsockopt ���� �����߻�
	cin >> sGroupIP;
	cout << "PORT ��ȣ �Է� : ";
	cin >> sPORT;

	//�Ʒ� �Լ��� �̿��ϱ� ���� C Ÿ������ ����ȯ
	const char* PORT = sPORT.c_str();
	const char* GroupIP = sGroupIP.c_str();

	memset(&Adr, 0, sizeof(Adr)); //������ sin_zero �κ��� 0���� ä���ߵǴµ� ó������ 0���� ä��� �ʿ��� �պκп� ���� �� �Ҵ�
	Adr.sin_family = AF_INET; //�ּ� ü��
	Adr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // �����Ǵ� �ڱ� �ڽ�, �Ŀ� �������� ��Ƽĳ��Ʈ �׷쿡 ����
	Adr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(Ÿ������)

	if (bind(hRecvSock, (SOCKADDR*)&Adr, sizeof(Adr)) == SOCKET_ERROR)
		cout << "bind error" << endl;

	joinAdr.imr_multiaddr.S_un.S_addr = inet_addr(GroupIP);
	joinAdr.imr_interface.S_un.S_addr = htonl(INADDR_ANY);

	if (setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR) // ���� �ɼ� ��Ƽ ĳ��Ʈ�� ����, 4��° ���ڰ� å���� void* ���� ���� const char* ����
		cout << "setsock error" << endl;

	while (1) // ��Ƽ ĳ��Ʈ ���� ����
	{
		strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (strLen < 0) break;
		buf[strLen] = 0;
		fputs(buf, stdout);
	}

	closesocket(hRecvSock);
	WSACleanup(); //���� ���̺귯�� �Ҵ� ����
	return 0;
}