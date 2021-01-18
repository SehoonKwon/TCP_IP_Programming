#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

//Ŀ�θ�� ����ȭ ����� ���ؽ��� �̿��� ����ȭ

#define NUM_THREAD 50

long long num = 0;
HANDLE hMutex; //���ؽ�

//WINAPI ��ȯ�� = _beginthreadex�� ����ϴ� ȣ�� �Ծ��� ��Ű�� ���� ����Ѵ�.
unsigned WINAPI threadInc(void* arg)
{
	WaitForSingleObject(hMutex, INFINITE); //�ش� �Լ��� ��ȯ�� �� ���ؽ��� �ڵ����� non-signaled ���°� �ǹǷ� �� �Ʒ��� �Ӱ迵�� ���� 
	for (int i = 0; i < 5000000; i++)
		num++;
	ReleaseMutex(hMutex); //�� ���� �Ӱ迵�� ��
	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < 5000000; i++)
		num--;
	ReleaseMutex(hMutex);
	return 0;
}

int main()
{
	HANDLE tHandles[NUM_THREAD]; //Ŀ�� ������Ʈ�� ������ �������ϴ� '�ڵ�'

	cout << "sizoef long long : " << sizeof(long long) << " \n";

	//1��° ���ڴ� �⺻ ���ȼ����� ���� NULL, 2��° 3��° ���ڴ� ���ؽ� ������Ʈ �������� ���¿� ���ؽ� ������Ʈ�� �̸��ο�
	//2��° ���� TRUE = ȣ���� �������� ������ �Ǹ� non-signaled / FALSE = ������Ʈ �����ڰ� �������� ������ signaled ���� 
	hMutex = CreateMutex(NULL, FALSE, NULL);
	
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
	CloseHandle(hMutex); //�ش� ���ؽ� ������Ʈ �Ҹ�
	cout << "result : " << num << endl;
	return 0;
}
