#pragma once
#include <string>
#include <vector>
#include <map>
class Device;

class DeviceManager
{
public:
	static DeviceManager* GetInstance();
public:
	int Init();
	Device* CreateDevice(const std::string& model);
	//const std::vector<Device*>& GetDevices();
	const std::map<std::string, Device*>& GetDevices();
	Device* GetDeviceByUuid(std::string& uuid);

	bool RegisterDevice(Device* const pDev);
private:
	//std::vector<Device*> m_vec_device;
	std::map<std::string, Device*> m_map_devices;
	std::map<std::string, std::string> m_map_model_relative;
};
