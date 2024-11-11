#include "monitor.h"
#include "monitortcpitem.h"
#include "PublicTool.h"

#define MONITOR_PORT 32767
#define CONF_PORT 32768


enum TIMER_ID
{
	TIMER_CHECK_SLAVE_ONLINE=1,
};

static CTcpSocketItem* CreateMonitorSockItem()
{
	return new MonitorTcpSockItem;
}


EMSMonitor::EMSMonitor():m_p_srv_monitor(nullptr),m_p_srv_conf(nullptr)
{
	m_set_slave_info.clear();
	m_timer_check_online.InitTimerNode(this, TIMER_CHECK_SLAVE_ONLINE);
}

EMSMonitor::~EMSMonitor()
{

}

int EMSMonitor::Init()
{
	int ret = 0;

	m_p_srv_conf = new CTcpV4Server;
	if(!m_p_srv_conf)
		return -1;
	m_p_srv_monitor->setAddrInfo("0.0.0.0", CONF_PORT);
	m_p_srv_monitor->SetCreateFun(CreateMonitorSockItem);
	ret = m_p_srv_monitor->Init();
	if(ret)
	{
		log_error("m_p_srv_monitor init failer %d", ret);
		return ret;
	}
	
	m_p_srv_monitor = new CTcpV4Server;
	if(!m_p_srv_monitor)
		return -1;
	m_p_srv_monitor->setAddrInfo("127.0.0.1", MONITOR_PORT);
	m_p_srv_monitor->SetCreateFun(CreateMonitorSockItem);
	ret = m_p_srv_monitor->Init();
	if(ret)
	{
		log_error("m_p_srv_monitor init failer %d", ret);
		return ret;
	}
	
	m_timer_check_online.StartTimerSec(30, -1);
	return 0;
}

int EMSMonitor::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case TIMER_CHECK_SLAVE_ONLINE:
		{
			CheckSlavesOnline();
			break;
		}
		default:
		{
			break;
		}
	}
	return 0;
}

int EMSMonitor::CheckSlavesOnline()
{
	for(std::set<salveInfo>::intertor it = m_set_slave_info.begin(); it != m_set_slave_info.end();++it)
	{
		int cnt = ++(it->nTimeOutCnt);
		if(cnt > 3)
		{
			char command[32] = {0};
			sprintf(command,"kill -9 %d",it->pid);
			system(command);
			it->pid = StartSlaveNode(it->strIP,it->nPort);
			it->nTimeOutCnt = 0;
		}
		
	}
	return 0;
}

int EMSMonitor::LoadSlaveNode()
{
	std::string dir = PublicTool::GetProgramLocation();
	
}

pid_t EMSMonitor::StartSlaveNode(std::string& ip, UINT16 port)
{
	pid_t pid = fork();
	
	if(pid < 0)
		return -1;
	else if(0 == pid)
	{
		
	}
	else(pid > 0)
		return pid;
}

