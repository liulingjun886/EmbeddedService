#pragma once

//程序运行的主模块入口

class EMSServer
{
public:
	virtual int GetThreadNum()=0;
	virtual int InitEMS()=0;
	virtual int StartEMS()=0;
};