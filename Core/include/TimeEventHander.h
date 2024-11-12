#pragma once

/*
定时器处理基类，如果要使用定时器必须继承自此类
*/

class CTimeEventHander
{
public:
	virtual ~CTimeEventHander(){}
	virtual int ProcessTimeOutEvent(UINT32 nTimeId)=0;
};

