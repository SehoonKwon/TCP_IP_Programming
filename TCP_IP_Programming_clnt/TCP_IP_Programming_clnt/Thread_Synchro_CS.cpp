#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

//������� ����ȭ ����� ũ��Ƽ�� ���� ������Ʈ�� �̿��� ����ȭ

#define NUM_THREAD 50

long long num = 0;
CRITICAL_SECTION cs; //ũ��Ƽ�� ���� ���� ����

//WINAPI ��ȯ�� = _beginthreadex�� ����ϴ� ȣ�� �Ծ��� ��Ű�� ���� ����Ѵ�.
unsigned WINAPI threadInc(void* arg)
{
	//for�� �ȿ� �־ ������ �׷��� �ſ� ���� ���� CS ������Ʈ ȹ�� �� �ݳ��� �̷���� �ſ� �����ɸ�
	EnterCriticalSection(&cs); //ũ��Ƽ�� ���� ������Ʈ ȹ��
	for (int i = 0; i < 5000000; i++)
		num++;
	LeaveCriticalSection(&cs); //ũ��Ƽ�� ���� ������Ʈ �ݳ�
	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	EnterCriticalSection(&cs); //ũ��Ƽ�� ���� ������Ʈ ȹ��
	for (int i = 0; i < 5000000; i++)
		num--;
	LeaveCriticalSection(&cs); //ũ��Ƽ�� ���� ������Ʈ �ݳ�
	return 0;
}

int main()
{
	HANDLE tHandles[NUM_THREAD]; //Ŀ�� ������Ʈ�� ������ �������ϴ� '�ڵ�'

	cout << "sizoef long long : " << sizeof(long long) << " \n";

	InitializeCriticalSection(&cs); //ũ��Ƽ�� ���� ������Ʈ �ʱ�ȭ
	for (int i = 0; i < NUM_THREAD; i++)
	{
		//3��°, 4��° ���ڸ� �Ű澲�� ������ NULL�� 0���� ����Ѵ�(default ����)
		//3��° = ����Ϸ��� �������� �����Լ�, 4��° = �� ������ �����Լ��� ��������
		//������ ������ �ڵ�, ���н� NULL��ȯ
		//_beginthread �Լ��� �ְ� �������� �� �Լ��� ������ ������ ��ȯ�Ǵ� �ڵ��� ��ȿȭ���� Ŀ�� ������Ʈ�� ������ �� ���� ������ �ִ�
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	//WaitForSingleObject�� �ټ�������
	//�Ű������� �˻��� Ŀ�� ������Ʈ�� ��, �ڵ������� ��� �ִ� �迭�ּ� ��, �˻系��, Ÿ�Ӿƿ� ����
	//3��° ������ �˻系�뿡�� TRUE = ��� �˻����� signaled�� �Ǿ�� ��ȯ(=signaled�� �ƴٴ°� �ش� �����尡 ����Ǿ��ٴ� �ǹ�)
	//FALSE = �˻��� �� �ϳ��� signaled ���°� �Ǹ� ��ȯ
	//INFINITE�� �����ϸ� Ŀ�� ������Ʈ�� signaled�� �Ǳ� ������ ��ȯ���� �ʴ´�
	//������ �̺�Ʈ ���� = WAIT_OBJECT_0 �Ǵ� Ÿ�Ӿƿ����� ���� ���� WAIT_TIMEOUT�� ��ȯ�ϸ� ���н� WAIT_FAILED ��ȯ
	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);

	DeleteCriticalSection(&cs); //ũ��Ƽ�� ���� ������Ʈ �Ҹ�
	cout << "result : " << num << endl;
	return 0;
}
