#define _CRT_SECURE_NO_WARNINGS // for use fopen
#define _WINSOCK_DEPRECATED_NO_WARNINGS // for use inet_addr
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>
#include <WS2tcpip.h> // for IP_MULTICAST_TTL option
#include <string>
#include <Windows.h> // for use Sleep
using namespace std;

#define TTL 64
#define endl "\n"
#define BUF_SIZE 30

int main(int argc, char* argv[]) //argc�� �ɼ��� ����, argv�� �ɼ� �迭
{
	WSADATA wsaData;
	SOCKET hSendSock;
	SOCKADDR_IN mulAdr;
	
	int timeLive = TTL;
	FILE *fp;
	char buf[BUF_SIZE];

	//winsock �ʱ�ȭ, ���α׷����� �䱸�ϴ� ������ ���� ������ �˸��� �ش� ������ �����ϴ� ���̺귯�� �ʱ�ȭ �۾�
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(�ֹ���, �ι���), WSADATA ������ �ּ� ��
		cout << "WSAStartup Error" << endl;

	hSendSock = socket(PF_INET, SOCK_DGRAM, 0); // ��Ƽĳ��Ʈ�� UDP ����
	if (hSendSock == INVALID_SOCKET) // -1 ����������� ����� ���°� �ƴ϶� �̷��� ��������� ������ ����ϸ� �������� ���� �Ŀ� ��ȭ�� �־ ��ó����
		cout << "socket Error" << endl;

	string sGroupIP, sPORT;
	cout << "��Ƽ ĳ��Ʈ �� GroupIP �Է� : ";
	cin >> sGroupIP;
	cout << "PORT ��ȣ �Է� : ";
	cin >> sPORT;

	//�Ʒ� �Լ��� �̿��ϱ� ���� C Ÿ������ ����ȯ
	const char* GroupIP = sGroupIP.c_str();
	const char* PORT = sPORT.c_str();

	Sleep(10000);

	memset(&mulAdr, 0, sizeof(mulAdr)); //������ sin_zero �κ��� 0���� ä���ߵǴµ� ó������ 0���� ä��� �ʿ��� �պκп� ���� �� �Ҵ�
	mulAdr.sin_family = AF_INET; //�ּ� ü��
	mulAdr.sin_addr.S_un.S_addr = inet_addr(GroupIP); // ��Ƽ ĳ��Ʈ �� �׷� ������ �Ҵ�
	mulAdr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(Ÿ������)

	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&timeLive, sizeof(timeLive)); // ���� �ɼ� ��Ƽ ĳ��Ʈ�� ����, 4��° ���ڰ� å���� void* ���� ���� const char* ����
	
	if ((fp = fopen("news.txt", "r")) == NULL)
		cout << "fopen Error" << endl;

	while (!feof(fp)) //txt ���� ���� ����
	{
		fgets(buf, BUF_SIZE, fp); // ���� �����ŭ�� �а�
		sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&mulAdr, sizeof(mulAdr)); // ����
	}

	fclose(fp);
	closesocket(hSendSock);
	WSACleanup(); //���� ���̺귯�� �Ҵ� ����
	return 0;
}