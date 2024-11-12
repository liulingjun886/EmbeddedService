#include "PollerUnit.h"
#include "PollerObject.h"
#include <iostream>

CPollerUnit::CPollerUnit()
{
}
CPollerUnit::~CPollerUnit()
{
}

void CPollerUnit::AddPoller(CPollerObject* pObj)
{
	m_set_poller_unit.insert(pObj);	
}
void CPollerUnit::RemovePoller(CPollerObject* pObj)
{
	m_set_poller_unit.erase(pObj);	
}
void CPollerUnit::Wait(UINT64 timeOut)
{
	int maxfd = 0;
	FD_ZERO(&rd);
	FD_ZERO(&wd);
	FD_ZERO(&ed);

	for(std::set<CPollerObject*>::iterator it = m_set_poller_unit.begin(); it != m_set_poller_unit.end(); ++it)
	{
		int fd = (*it)->getPoller();
		//std::cout << fd << std::endl;
		if(fd < 0 )
			continue;
		
		if(fd > maxfd)
			maxfd = fd;

		int nEvent = (*it)->GetEvent();
		if(nEvent & READ)
			FD_SET(fd,&rd);
		
		if(nEvent & WRITE)
			FD_SET(fd,&rd);
		
		if(nEvent & ERROR)
			FD_SET(fd,&rd);

	}

	m_timeout.tv_sec = timeOut/1000;
	m_timeout.tv_usec = (timeOut%1000) * 1000;
	m_n_ret = select(maxfd+1, &rd, &wd, &ed, &m_timeout);
	return;
}

void CPollerUnit::HandPollerEvents()
{
	if(m_n_ret <= 0)
		return;

	CPollerObject* poller;
	int fd = -1;
	for(std::set<CPollerObject*>::iterator it = m_set_poller_unit.begin(); it != m_set_poller_unit.end(); )
	{
		poller = (*it++);
		int fd = poller->getPoller();
		if(fd < 0 )
			continue;
		
		if(FD_ISSET(fd,&ed))
		{
			poller->HunpUpNotiry();
			continue;
		}
		
		if(FD_ISSET(fd,&rd) && 0 != poller->InPutNotify())
		{
			poller->HunpUpNotiry();
		}

		if(FD_ISSET(fd,&wd) && 0 != poller->OutPutNotify())
		{
			poller->HunpUpNotiry();
		}
	}
}



