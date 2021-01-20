#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

//동기화가 이루어지지 않아 전역변수인 num에 쓰레드들이 동시에 접근하여 올바른 결과가 나오지 않는다

#define NUM_THREAD 50

long long num = 0;

//WINAPI 반환형 = _beginthreadex가 명시하는 호출 규약을 지키기 위해 사용한다
unsigned WINAPI threadInc(void* arg)
{
	for (int i = 0; i < 5000000; i++)
		num++;
	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	for (int i = 0; i < 5000000; i++)
		num--;
	return 0;
}

int main()
{
	HANDLE tHandles[NUM_THREAD]; //커널 오브젝트의 구분자 역할을하는 '핸들'
	
	cout << "sizoef long long : " << sizeof(long long) << " \n";
	
	for (int i = 0; i < NUM_THREAD; i++)
	{
		//3번째, 4번째 인자만 신경쓰고 나머진 NULL과 0으로 사용한다(default 설정)
		//3번째 = 사용하려는 쓰레드의 메인함수, 4번째 = 그 쓰레드 메인함수의 인자정보
		//성공시 쓰레드 핸들, 실패시 NULL반환
		//_beginthread 함수도 있고 편하지만 이 함수는 쓰레드 생성시 반환되는 핸들을 무효화시켜 커널 오브젝트에 접근할 수 없는 문제가 있다
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	//WaitForSingleObject의 다수버전용
	//매개변수는 검사할 커널 오브젝트의 수, 핸들정보를 담고 있는 배열주소 값, 검사내용, 타임아웃 지정
	//3번째 인자인 검사내용에서 TRUE = 모든 검사대상이 signaled가 되어야 반환(=signaled가 됐다는건 해당 쓰레드가 종료되었다는 의미)
	//FALSE = 검사대상 중 하나라도 signaled 상태가 되면 반환
	//INFINITE로 설정하면 커널 오브젝트가 signaled가 되기 전까지 반환하지 않는다
	//성공시 이벤트 정보 = WAIT_OBJECT_0 또는 타임아웃으로 인한 정보 WAIT_TIMEOUT을 반환하며 실패시 WAIT_FAILED 반환
	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	cout << "result : " << num << endl;
	return 0;
}
