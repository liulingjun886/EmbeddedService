#include "Device.h"
#include "DataPool.h"


Device::Device():m_pDevBasicInfo(nullptr),m_bFreeDevdData(false),
m_p_parent(nullptr),m_n_state_enable(DISABLE),m_quickly_interval(200),
m_standardly_interval(1000),m_slowly_interval(60000)
{
	m_vec_children.clear();
}

Device::~Device()
{
	if(m_bFreeDevdData)
		delete[] (char*)m_pDevBasicInfo;
}

int Device::InitData(char* ptr)
{
	if(!ptr)
	{
		ptr = new char[GetDataLen()];
		if(!ptr)
			return 0;
		m_bFreeDevdData = true;
	}

	return SetDataPtr(ptr);
}


int Device::Init(const std::string& sn, const std::string& model, const std::string& name, int cmd, ASyncCallDataInst& initdata)
{
	if(!m_pDevBasicInfo)
		return -1;
	snprintf(m_pDevBasicInfo->m_str_device_sn,sizeof(m_pDevBasicInfo->m_str_device_sn),"%s",sn.c_str());
	snprintf(m_pDevBasicInfo->m_str_model,sizeof(m_pDevBasicInfo->m_str_model),"%s",model.c_str());
	snprintf(m_pDevBasicInfo->m_str_name,sizeof(m_pDevBasicInfo->m_str_name),"%s",name.c_str());
	return Initialize(cmd,initdata);
}

const DevBasicInfo* Device::GetDevBasicInfo()
{
	return m_pDevBasicInfo;
}

void Device::SetParent(Device* pDev)
{
	m_p_parent = pDev;
}

Device* Device::GetParent()
{
	return m_p_parent;
}

Device* Device::GetTopDevice()
{
	if(!m_p_parent)
		return this;
	return m_p_parent->GetParent();
}

void Device::AddChild(Device* pDev)
{
	m_vec_children.push_back(pDev);
}

const std::vector<Device*>& Device::GetChildren()
{
	return m_vec_children;
}

bool Device::SetRunState(ONLINESTATE state)
{
	if(m_pDevBasicInfo->m_n_run_state == state)
		return false;
	m_pDevBasicInfo->m_n_run_state = state;
	return true;
}

void Device::SetEnableState(DEVICE_WORKING_STATE state)
{
	if(m_n_state_enable == state)
		return;
	m_n_state_enable = state;
	if(ENABLE == m_n_state_enable)
		StartDevice();
	else
		StopDevice();
}

DEVICE_WORKING_STATE Device::GetEnableState() const
{
	return m_n_state_enable;
}

int Device::SetDataPtr(char* ptr) 
{
	if(ptr)
	{
		m_pDevBasicInfo = new (ptr) DevBasicInfo;
		return sizeof(DevBasicInfo);
	}

	m_pDevBasicInfo = new DevBasicInfo;
	m_bFreeDevdData = true;
	return 0;
}

int Device::GetDataLen()
{
	return sizeof(DevBasicInfo);
}


int Device::GetSysFault()
{
	return 0;
}

void Device::SetQuicklyInterval(UINT32 msec)
{
	m_quickly_interval = msec;
}
void Device::SetStandardlyInterval(UINT32 msec)
{
	m_standardly_interval = msec;
}
void Device::SetSlowlyInterval(UINT32 msec)
{
	m_slowly_interval = msec;
}


