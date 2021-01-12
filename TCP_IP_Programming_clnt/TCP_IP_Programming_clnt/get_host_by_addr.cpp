#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <string>
using namespace std;

#define endl "\n"

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	hostent *host;
	SOCKADDR_IN addr;
	string IP_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "wsastartup error" << endl;

	cout << "IP �ּ� �Է� : ";
	cin >> IP_addr;
	const char* nIPaddr = IP_addr.c_str(); //gethostbyaddr �Լ��� ���ڰ� const char* ���¶� Ÿ�Ժ�ȯ �ʿ�

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = inet_addr(nIPaddr);
	host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	if (!host)
		cout << "gethostbyaddr error" << endl;

	cout << "Official name : " << host->h_name << endl << endl;
	for (int i = 0; host->h_aliases[i]; i++) //������ �̸���
		cout << "Aliases " << i + 1 << ": " << host->h_aliases[i] << endl;
	if(host->h_aliases[0] != NULL) cout << endl;

	cout << "Adress type : ";
	if (host->h_addrtype == AF_INET) cout << "AF_INET" << endl << endl;
	else cout << "AF_INET6" << endl << endl;

	for (int i = 0; host->h_addr_list[i]; i++) //IP �ּҵ�
		cout << "IP addr " << i + 1 << ": " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << endl;

	return 0;
}