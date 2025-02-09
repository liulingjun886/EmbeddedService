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
	int nEvent = 0;
	int fd = -1;
	FD_ZERO(&rd);
	FD_ZERO(&wd);
	FD_ZERO(&ed);

	for(std::set<CPollerObject*>::iterator it = m_set_poller_unit.begin(); it != m_set_poller_unit.end(); ++it)
	{
		fd = (*it)->getPoller();
		//std::cout << fd << std::endl;
		if(fd < 0 )
			continue;
		
		if(fd > maxfd)
			maxfd = fd;

		nEvent = (*it)->GetEvent();
		if(nEvent & READ)
			FD_SET(fd,&rd);
		
		if(nEvent & WRITE)
			FD_SET(fd,&wd);
		
		if(nEvent & ERROR)
			FD_SET(fd,&ed);

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

	int fd = -1;
	int nEvent = 0;
	CPollerObject* poller;
	
	for(std::set<CPollerObject*>::iterator it = m_set_poller_unit.begin(); it != m_set_poller_unit.end(); )
	{
		poller = (*it++);
		fd = poller->getPoller();
		if(fd < 0 )
			continue;
		
		if((nEvent & ERROR) && FD_ISSET(fd,&ed))
		{
			poller->HunpUpNotiry();
			continue;
		}
		
		if(FD_ISSET(fd,&rd) && 0 != poller->InPutNotify())
		{
			poller->HunpUpNotiry();
		}

		if((nEvent & WRITE) && FD_ISSET(fd,&wd) && 0 != poller->OutPutNotify())
		{
			poller->HunpUpNotiry();
		}
	}
}



