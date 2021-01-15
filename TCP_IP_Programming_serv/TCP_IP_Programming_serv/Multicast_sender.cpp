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

int main(int argc, char* argv[]) //argc는 옵션의 개수, argv는 옵션 배열
{
	WSADATA wsaData;
	SOCKET hSendSock;
	SOCKADDR_IN mulAdr;
	
	int timeLive = TTL;
	FILE *fp;
	char buf[BUF_SIZE];

	//winsock 초기화, 프로그램에서 요구하는 윈도우 소켓 버전을 알리고 해당 버전을 지원하는 라이브러리 초기화 작업
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //MAKEWOR(주버전, 부버전), WSADATA 변수의 주소 값
		cout << "WSAStartup Error" << endl;

	hSendSock = socket(PF_INET, SOCK_DGRAM, 0); // 멀티캐스트는 UDP 형식
	if (hSendSock == INVALID_SOCKET) // -1 상수값이지만 상수로 쓰는게 아니라 이렇게 사용자정의 변수로 사용하면 가독성도 좋고 후에 변화가 있어도 대처가능
		cout << "socket Error" << endl;

	string sGroupIP, sPORT;
	cout << "멀티 캐스트 할 GroupIP 입력 : ";
	cin >> sGroupIP;
	cout << "PORT 번호 입력 : ";
	cin >> sPORT;

	//아래 함수를 이용하기 위해 C 타입으로 형변환
	const char* GroupIP = sGroupIP.c_str();
	const char* PORT = sPORT.c_str();

	Sleep(10000);

	memset(&mulAdr, 0, sizeof(mulAdr)); //소켓의 sin_zero 부분을 0으로 채워야되는데 처음부터 0으로 채우고 필요한 앞부분에 새로 값 할당
	mulAdr.sin_family = AF_INET; //주소 체계
	mulAdr.sin_addr.S_un.S_addr = inet_addr(GroupIP); // 멀티 캐스트 할 그룹 아이피 할당
	mulAdr.sin_port = htons(atoi(PORT)); //htons = host byte to network byte short(타입형태)

	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&timeLive, sizeof(timeLive)); // 소켓 옵션 멀티 캐스트로 설정, 4번째 인자가 책에는 void* 지만 현재 const char* 형태
	
	if ((fp = fopen("news.txt", "r")) == NULL)
		cout << "fopen Error" << endl;

	while (!feof(fp)) //txt 내용 전부 전달
	{
		fgets(buf, BUF_SIZE, fp); // 버프 사이즈만큼씩 읽고
		sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&mulAdr, sizeof(mulAdr)); // 전달
	}

	fclose(fp);
	closesocket(hSendSock);
	WSACleanup(); //윈속 라이브러리 할당 해제
	return 0;
}