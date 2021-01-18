#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

static HANDLE semOne;
static HANDLE semTwo;
static int num;

unsigned WINAPI Read(void* arg)
{
	for (int i = 0; i < 5; i++)
	{
		fputs("Input num : ", stdout);
		WaitForSingleObject(semTwo, INFINITE);
		scanf("%d", &num);
		ReleaseSemaphore(semOne, 1, NULL); //반납할 세마포어, 세마포어의 증가량(0이 되면 꽉 찬 상태), 변경 이전의 세마포어 값 지정을 위한 변수 주소
	}
	return 0;
}

unsigned WINAPI Accu(void* arg)
{
	int sum = 0;
	for (int i = 0; i < 5; i++)
	{
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		ReleaseSemaphore(semTwo, 1, NULL);
	}
	cout << "Result : " << sum << endl;
	return 0;
}

int main()
{
	HANDLE hThread1, hThread2;

	//인자정보 : 보안정보, 세마포어의 초기 값, 최대 세모포어 값, 생성되는 세마포어 오브젝트 이름
	//초기 값은 3번째 인자보다 커선 안되고 0 이상이어야 한다
	//3번째 인자인 IMaximumCount가 1이면 세마포어 값은 0, 1뿐인 바이너리 세마포어가 된다
	semOne = CreateSemaphore(NULL, 0, 1, NULL);
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	CloseHandle(semOne); 
	CloseHandle(semTwo);
	return 0;
}