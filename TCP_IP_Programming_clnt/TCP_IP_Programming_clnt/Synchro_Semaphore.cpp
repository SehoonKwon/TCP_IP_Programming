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
		ReleaseSemaphore(semOne, 1, NULL); //�ݳ��� ��������, ���������� ������(0�� �Ǹ� �� �� ����), ���� ������ �������� �� ������ ���� ���� �ּ�
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

	//�������� : ��������, ���������� �ʱ� ��, �ִ� �������� ��, �����Ǵ� �������� ������Ʈ �̸�
	//�ʱ� ���� 3��° ���ں��� Ŀ�� �ȵǰ� 0 �̻��̾�� �Ѵ�
	//3��° ������ IMaximumCount�� 1�̸� �������� ���� 0, 1���� ���̳ʸ� ������� �ȴ�
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