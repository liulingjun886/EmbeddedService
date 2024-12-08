#include "Log.h"
#include "PublicTool.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_LOG_SIZE 1024
#define MAX_LOG_FILE_SIZE 1024*1024*30



enum COMMOND
{
	WRITELOG,
	CHECKNEWFILE,
};

static const char* g_str_level[5] = {"NONE","DEBUG","INFO","WARN","ERROR"};

Log *Log::GetInstance()
{
	static Log mp_instance;
	return &mp_instance;
}


Log::Log():m_nLogLevel(LOG_NONE),m_nMon(0),m_nDay(0),m_pFile(NULL),m_nIndex(1)
{
	//SetThreadIndex(0);
	memset(m_szFileName,0,sizeof(m_szFileName));
	m_timerCheck.InitTimerNode(this, 1);
	m_log_pool.clear();
}

Log::~Log()
{
	
}


void Log::InitLog(const char * const szFileName,LOG_LEVEL level)
{
	if(strlen(m_szFileName) > 0)
		return;
		
	if(strlen(szFileName) > sizeof(m_szFileName))
		_exit(0);

	std::string logPath(PublicTool::GetProgramLocation());
	logPath.append("log");
	if(!PublicTool::FileIsExists(logPath))
	{
		char mkdirCmd[256] = {0};
		sprintf(mkdirCmd,"mkdir -p %s",logPath.c_str());
		if(system(mkdirCmd))
			printf("create log file failer\n");
	}
	
	int ret = snprintf(m_szFileName,256,"%s/log/%s",PublicTool::GetProgramLocation().c_str(),szFileName);
	m_nLogLevel = level;
	ProcessTimeOutEvent(1);
	m_timerCheck.StartTimerSec(10,-1);
}


void Log::WriteLog(LOG_LEVEL level, const char *pFileName, const char* pFunName,unsigned long cLine,const char *fmt, ...)
{	
	if(level < m_nLogLevel)
		return;
	
	ASyncCallDataInst log;
	log.fill(0, MAX_LOG_SIZE);

	int len = snprintf(log.data(), MAX_LOG_SIZE,"%s|[%s,%ld]|%s|:",g_str_level[level], pFileName, cLine, pFunName);
	va_list ap;
	va_start(ap,fmt);
	vsnprintf(log.data()+len,MAX_LOG_SIZE-len, fmt, ap);
	va_end(ap);

	SendAsyncAction(WRITELOG, log);
}

int Log::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case 1:
		{
			SendAsyncAction(CHECKNEWFILE);
			break;
		}
	}
}

void Log::SetLogLevel(LOG_LEVEL level)
{
	m_nLogLevel = level;
}

void Log::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq)
{
	switch(cmd)
	{
		case WRITELOG:
		{
			static struct timeval tv;
  			static struct tm t;
  			static char buffer [] = "00:00:00";
  			static int milli = 0;
  			gettimeofday(&tv, NULL);
  			milli = tv.tv_usec / 1000;
  			localtime_r(&tv.tv_sec, &t);
  			strftime(buffer, sizeof(buffer), "%H:%M:%S", &t);
			
			if(m_pFile)
			{
				struct stat log_stat;
				fstat(fileno(m_pFile),&log_stat);
				if(log_stat.st_size >= MAX_LOG_FILE_SIZE)
				{
					do
					{
						m_nIndex += 1;
						char szFileName[512] = {0};
						sprintf(szFileName,"%s_%02d_%02d_%u.txt",m_szFileName,m_nMon+1,m_nDay,m_nIndex);
						fclose(m_pFile);
						m_pFile = fopen(szFileName,"a");
						if(!m_pFile)
						{
							printf("log file open failer %s",szFileName);
							_exit(0);
						}
						fstat(fileno(m_pFile),&log_stat);
					}while(log_stat.st_size >= MAX_LOG_FILE_SIZE);
				}
				
				//fprintf(m_pFile,"%02d:%02d:%02d|%s\n",p->tm_hour,p->tm_min,p->tm_sec,pReq.data());
				fprintf(m_pFile,"%s.%03d|%s\n",buffer,milli,pReq.data());
				fflush(m_pFile);
				//printf("%02d:%02d:%02d|%s\n",p->tm_hour,p->tm_min,p->tm_sec,pReq.data());
				printf("%s.%03d|%s\n",buffer,milli,pReq.data());
				
			}
			else
			{
				//printf("%02d:%02d:%02d|%s\n",p->tm_hour,p->tm_min,p->tm_sec,(char*)pReq.data());
				printf("%s.%03d|%s\n",buffer,milli,pReq.data());
				fflush(stdout);
			}
			break;
		}
		case CHECKNEWFILE:
		{
			CheckNewFile();
			break;
		}
		default:
			break;
	}
}


void Log::CheckNewFile()
{
	time_t timep; 
	struct tm *p;  
	time(&timep); 
	p=localtime(&timep);

	if(p->tm_mon == m_nMon && p->tm_mday == m_nDay)
		return;
	
	m_nMon = p->tm_mon;
	m_nDay = p->tm_mday;
	m_nIndex = 1;

	char szFileName[512] = {0};
	sprintf(szFileName,"%s_%02d_%02d_%u.txt",m_szFileName,m_nMon+1,m_nDay,m_nIndex);
	//sDumpFileDir = szFileName;
	if(m_pFile)
		fclose(m_pFile);
	
	m_pFile = fopen(szFileName,"a");
	
	if(!m_pFile)
	{
		printf("log file open failer\n");
		_exit(0);
	}
}

