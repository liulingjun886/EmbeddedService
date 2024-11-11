#include "DeviceManager.h"
#include "PublicTool.h"
#include "Device.h"
#include <json/json.h>
#include "Log.h"
#include "DataPool.h"

DeviceManager* DeviceManager::GetInstance()
{
	static DeviceManager mp_instance;
	return &mp_instance;
}

int DeviceManager::Init()
{
	Json::Value cfgroot;
	Json::Reader reader;
	std::string content = PublicTool::ReadFile(PublicTool::GetProgramLocation()+"conf/modules.json");
	if(!reader.parse(content, cfgroot))
	{
		log_error("reader.parse");
		return -1;
	}
	
	if(!cfgroot.isArray())
	{
		log_error("cfgroot.isArray failer");
		return -1;
	}

	for(int i = 0; i < cfgroot.size(); ++i)
	{
		Json::Value& value = cfgroot[i]; 
		m_map_model_relative[value["model"].asString()] = value["soname"].asString();
	}
	return 0;
}


Device* DeviceManager::CreateDevice(const std::string& model)
{
	Device* pDev = nullptr;
	std::map<std::string, std::string>::iterator it = m_map_model_relative.find(model);
	if(it != m_map_model_relative.end())
	{
		pDev = PublicTool::CreateDevice(PublicTool::GetProgramLocation()+"plug_in/"+it->second, it->first);
	}
	
	return pDev;
}

/*const std::vector<Device*>& DeviceManager::GetDevices()
{
	return m_vec_device;
}*/

const std::map<std::string, Device*>& DeviceManager::GetDevices()
{
	return m_map_devices;
}

Device* DeviceManager::GetDeviceByUuid(std::string& uuid)
{
	/*for(std::vector<Device*>::iterator it = m_vec_device.begin(); it != m_vec_device.end(); ++it)
	{
		if((*it)->getDevSn() == uuid)
			return (*it);
	}*/
	std::map<std::string, Device*>::iterator it = m_map_devices.find(uuid);
	if(it == m_map_devices.end())
		return nullptr;
	return it->second;
}

bool DeviceManager::RegisterDevice(Device* const pDev)
{
	/*for(std::vector<Device*>::iterator it = m_vec_device.begin(); it != m_vec_device.end(); ++it)
	{
		if((*it) == pDev)
			return false;
	}*/
	std::map<std::string, Device*>::iterator it = m_map_devices.find(pDev->GetDevBasicInfo()->m_str_device_sn);
	if(it != m_map_devices.end())
		return false;
	
	m_map_devices[pDev->GetDevBasicInfo()->m_str_device_sn] = pDev;
	return true;
}


