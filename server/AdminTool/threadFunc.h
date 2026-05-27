#pragma once 

//	thread로 사용되는 몇몇 함수들의 공유를 위해 묶어둔 클래스. 다른 의미는 없다.


class cThreadFunc
{
public:
	static void WorldCtlThread(void *tmp);
};