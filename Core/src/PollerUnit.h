#pragma once
#include <set>
#include "UserTypeDefines.h"
#include <sys/select.h>
#include <unistd.h>

class CPollerObject;

class CPollerUnit
{
public:
	CPollerUnit();
	~CPollerUnit();
public:
	void AddPoller(CPollerObject* pObj);
	void RemovePoller(CPollerObject* pObj);
	void Wait(UINT64 timeOut);
	void HandPollerEvents();
private:
	int m_n_ret;
	fd_set rd;
	fd_set wd;
	fd_set ed;
	timeval m_timeout;
	std::set<CPollerObject*> m_set_poller_unit;
};
