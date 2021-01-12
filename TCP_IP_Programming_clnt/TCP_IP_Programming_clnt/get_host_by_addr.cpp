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

	cout << "IP 주소 입력 : ";
	cin >> IP_addr;
	const char* nIPaddr = IP_addr.c_str(); //gethostbyaddr 함수의 인자가 const char* 형태라 타입변환 필요

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = inet_addr(nIPaddr);
	host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	if (!host)
		cout << "gethostbyaddr error" << endl;

	cout << "Official name : " << host->h_name << endl << endl;
	for (int i = 0; host->h_aliases[i]; i++) //도메인 이름들
		cout << "Aliases " << i + 1 << ": " << host->h_aliases[i] << endl;
	if(host->h_aliases[0] != NULL) cout << endl;

	cout << "Adress type : ";
	if (host->h_addrtype == AF_INET) cout << "AF_INET" << endl << endl;
	else cout << "AF_INET6" << endl << endl;

	for (int i = 0; host->h_addr_list[i]; i++) //IP 주소들
		cout << "IP addr " << i + 1 << ": " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << endl;

	return 0;
}