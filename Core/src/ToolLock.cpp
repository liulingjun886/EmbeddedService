#include "ToolLock.h"
CMutexLock::CMutexLock()
{
	pthread_mutex_init(&m_metux,NULL);
}


CMutexLock::~CMutexLock()
{
	pthread_mutex_destroy(&m_metux);
}

pthread_mutex_t* CMutexLock::GetMutex()
{
	return &m_metux;
}

void CMutexLock::Lock(int)
{
	pthread_mutex_lock(&m_metux);
}

void CMutexLock::UnLock()
{
	pthread_mutex_unlock(&m_metux);
}

CRWLock::CRWLock()
{
	pthread_rwlock_init(&m_rwlock, NULL);
}

CRWLock::~CRWLock()
{
	pthread_rwlock_destroy(&m_rwlock);
}

void CRWLock::Lock(int nType)
{
	if (0 == nType)
	{
		pthread_rwlock_wrlock(&m_rwlock);
	}
	else
	{
		pthread_rwlock_rdlock(&m_rwlock);
	}
}

void CRWLock::UnLock()
{
	pthread_rwlock_unlock(&m_rwlock);
}

CSpinLock::CSpinLock()
{
	pthread_spin_init(&m_spin_lock, 0);
}

CSpinLock::~CSpinLock()
{
	pthread_spin_destroy(&m_spin_lock);
}

void CSpinLock::Lock(int nType)
{
	(void)nType;
	pthread_spin_lock(&m_spin_lock);
}

void CSpinLock::UnLock()
{
	pthread_spin_unlock(&m_spin_lock);
}

void CToolLock::Lock(int nLock_Type)
{
	m_pLock->Lock(nLock_Type);
}
void CToolLock::UnLock()
{
	m_pLock->UnLock();
}

CWaitCond::CWaitCond()
{
	pthread_cond_init(&m_cond, NULL);
}

CWaitCond::~CWaitCond()
{
	pthread_cond_destroy(&m_cond);
}

void CWaitCond::Wait(CMutexLock& mutex)
{
	pthread_cond_wait(&m_cond,mutex.GetMutex());
}

void CWaitCond::WakeUp()
{
	pthread_cond_signal(&m_cond);
}
