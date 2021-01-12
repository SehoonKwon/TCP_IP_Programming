#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
#include <string>
using namespace std;

#define endl "\n"

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	hostent *host;
	string domain;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "WSAStartup error" << endl;

	cout << "������ �Է� : ";
	cin >> domain;
	const char* ndomain = domain.c_str(); //gethostbyname �Լ��� ���ڰ� const char* ���¶� Ÿ�Ժ�ȯ �ʿ�
	host = gethostbyname(ndomain);
	if (!host)
		cout << "gethostbyname error" << endl;

	cout << "Official name : " << host->h_name << endl << endl;
	for (int i = 0; host->h_aliases[i]; i++) //������ �̸���
		cout << "Aliases " << i + 1 << ": " << host->h_aliases[i] << endl;
	cout << endl;
	
	cout << "Adress type : ";
	if (host->h_addrtype == AF_INET) cout << "AF_INET" << endl << endl;
	else cout << "AF_INET6" << endl << endl;

	for (int i = 0; host->h_addr_list[i]; i++) //IP�ּҵ�
		cout << "IP addr " << i + 1 << ": " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << endl;

	return 0;
}