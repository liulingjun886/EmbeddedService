#pragma once

#include <unistd.h>
#include <list>
#include <stdarg.h>
#include <core/ASyncCallSupport.h>
#include <core/TimeNode.h>
#include <core/TimeEventHander.h>

enum LOG_LEVEL
{
	LOG_NONE = 0,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

class Log : public CAsyncCallSupport,public CTimeEventHander
{
public:
	Log();
	~Log();
	static Log *GetInstance();
public:
	void InitLog(const char * const szFileName,LOG_LEVEL level = LOG_WARNING);
	void WriteLog(LOG_LEVEL level, const char *pFileName, const char* pFunName,unsigned long cLine,const char *fmt, ...);
	int ProcessTimeOutEvent(UINT32 nTimeId);
	
protected:
	void DoAsyncAction(INT32 cmd,  INT32 param, ASyncCallDataInst& pReq);
	
private:
	void CheckNewFile();
	
private:
	char m_szFileName[256];
	LOG_LEVEL m_nLogLevel;
	INT32 m_nMon;
	INT32 m_nDay;
	UINT32 m_nIndex;
	FILE* m_pFile;
	CTimer m_timerCheck;
	std::list<ASyncCallDataInst> m_log_pool;
};

#define filename(x) (strrchr(x,'/') != NULL ? strrchr(x,'/')+1:x)
#define log_debug(fmt,...) Log::GetInstance()->WriteLog(LOG_DEBUG,filename(__FILE__),__func__,__LINE__,fmt,##__VA_ARGS__);
#define log_info(fmt,...) Log::GetInstance()->WriteLog(LOG_INFO,filename(__FILE__),__func__,__LINE__,fmt,##__VA_ARGS__);
#define log_warn(fmt,...) Log::GetInstance()->WriteLog(LOG_WARNING,filename(__FILE__),__func__,__LINE__,fmt,##__VA_ARGS__);
#define log_error(fmt,...) Log::GetInstance()->WriteLog(LOG_ERROR,filename(__FILE__),__func__,__LINE__,fmt,##__VA_ARGS__);