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

	//winsock 초기화, 프로그램에서 요구하는 윈도우 소켓 버전을 알리고 해당 버전을 지원하는 라이브러리 초기화 작업
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(주버전, 부버전), WSADATA 변수의 주소 값
		cout << "WSAStartup error" << endl;

	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0); // 멀티캐스트는 UDP 형식
	if (hRecvSock == INVALID_SOCKET) // -1 상수값이지만 상수로 쓰는게 아니라 이렇게 사용자정의 변수로 사용하면 가독성도 좋고 후에 변화가 있어도 대처가능
		cout << "socket error" << endl;

	string sGroupIP, sPORT;
	cout << "Group IP 입력 : "; //그룹 IP를 127.0.0.1 루프백 IP를 넣으면 밑에 setsockopt 에서 에러발생
	cin >> sGroupIP;
	cout << "PORT 번호 입력 : ";
	cin >> sPORT;

	//아래 함수를 이용하기 위해 C 타입으로 형변환
	const char* PORT = sPORT.c_str();
	const char* GroupIP = sGroupIP.c_str();

	memset(&Adr, 0, sizeof(Adr)); //소켓의 sin_zero 부분을 0으로 채워야되는데 처음부터 0으로 채우고 필요한 앞부분에 새로 값 할당
	Adr.sin_family = AF_INET; //주소 체계
	Adr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 아이피는 자기 자신, 후에 소켓으로 멀티캐스트 그룹에 가입
	Adr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(타입형태)

	if (bind(hRecvSock, (SOCKADDR*)&Adr, sizeof(Adr)) == SOCKET_ERROR)
		cout << "bind error" << endl;

	joinAdr.imr_multiaddr.S_un.S_addr = inet_addr(GroupIP);
	joinAdr.imr_interface.S_un.S_addr = htonl(INADDR_ANY);

	if (setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR) // 소켓 옵션 멀티 캐스트로 설정, 4번째 인자가 책에는 void* 지만 현재 const char* 형태
		cout << "setsock error" << endl;

	while (1) // 멀티 캐스트 내용 수신
	{
		strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (strLen < 0) break;
		buf[strLen] = 0;
		fputs(buf, stdout);
	}

	closesocket(hRecvSock);
	WSACleanup(); //윈속 라이브러리 할당 해제
	return 0;
}