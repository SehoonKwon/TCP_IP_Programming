#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <WinSock2.h>
using namespace std;

#define endl "\n"

int main(int argc, char* argv[]) //argc는 옵션의 개수, argv는 옵션 배열
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello wolrd";
	
	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>" << endl;
		exit(1);
	}

	//winsock 초기화, 프로그램에서 요구하는 윈도우 소켓 버전을 알리고 해당 버전을 지원하는 라이브러리 초기화 작업
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(주버전, 부버전), WSADATA 변수의 주소 값
		cout << "WSAStartup Error" << endl;

	hServSock = socket(PF_INET, SOCK_STREAM, 0); //INET = IPv4, INET6 = IPv6 등, SOCK_STREAM = TCP, SOCK_DGRAM = UDP
	if (hServSock == INVALID_SOCKET) // -1 상수값이지만 상수로 쓰는게 아니라 이렇게 사용자정의 변수로 사용하면 가독성도 좋고 후에 변화가 있어도 대처가능
		cout << "socket Error" << endl;

	memset(&servAddr, 0, sizeof(servAddr)); //소켓의 마지막 부분을 0으로 채워야되는데 처음부터 0으로 채우고 필요한 앞부분에 새로 값 할당
	servAddr.sin_family = AF_INET; //주소 체계
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //여긴 책이랑 형태가 조금 다름. S_un 이후에 addr이 나옴, INADDR_ANY = 컴퓨터의 IP주소 자동할당
	servAddr.sin_port = htons(atoi(argv[1])); //htons = host byte to network byte short(타입형태)

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //2번째인자를 SOCKADDR 형변환을 이용해 IP와 Port번호 할당 한번에 해주기
		cout << "bind Error" << endl;

	if (listen(hServSock, 5) == SOCKET_ERROR) //listen 함수이후부터 클라이언트에서 connect가능, 2번째인자는 대기 큐의 개수 설정
		cout << "listen Error" << endl;

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //
	if (hClntSock == INVALID_SOCKET)
		cout << "accept Error" << endl;

	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup(); //윈속 라이브러리 할당 해제
	return 0;
}