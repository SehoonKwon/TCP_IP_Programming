#define _WINSOCK_DEPRECATED_NO_WARNINGS //없으면 inet_addr 에러
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <WinSock2.h>
using namespace std;

#define endl "\n"

int main(int argc, char* argv[]) //argc는 옵션의 개수, argv는 옵션 배열
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

	//winsock 초기화, 프로그램에서 요구하는 윈도우 소켓 버전을 알리고 해당 버전을 지원하는 라이브러리 초기화 작업
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(주버전, 부버전), WSADATA 변수의 주소 값
		cout << "WSAStartup Error" << endl;

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 등, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hSocket == INVALID_SOCKET) // -1 상수값이지만 상수로 쓰는게 아니라 이렇게 사용자정의 변수로 사용하면 가독성도 좋고 후에 변화가 있어도 대처가능
		cout << "socket Error" << endl;

	memset(&servAddr, 0, sizeof(servAddr)); //소켓의 sin_zero 부분을 0으로 채워야되는데 처음부터 0으로 채우고 필요한 앞부분에 새로 값 할당
	servAddr.sin_family = AF_INET; //주소 체계
	servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //IP주소를 네트워크 바이트 형태의 값으로 변환
	servAddr.sin_port = htons(atoi(argv[2])); //htons = host byte to network byte short(타입형태)

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //2번째인자를 SOCKADDR 형변환을 이용해 IP와 Port번호 할당 한번에 해주기
		cout << "connect Error" << endl;

	strLen = recv(hSocket, message, sizeof(message) - 1, 0); //인자 -> 대상과 연결된 소켓 핸들 값, 수신된 데이터를 저장할 주소 값, 수신할 수 있는 바이트 수, 옵션정보
	if (strLen == -1)
		cout << "read Error" << endl;

	cout << "Message from server : " << message << endl;
	closesocket(hSocket);
	WSACleanup(); //윈속 라이브러리 할당 해제
	return 0;
}