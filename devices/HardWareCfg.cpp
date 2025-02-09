#include "HardWareCfg.h"
#include "Log.h"
#include "PublicTool.h"



HardWareCfg* HardWareCfg::GetInstance()
{
	static HardWareCfg mp_instance;
	return &mp_instance;
}

HardWareCfg::HardWareCfg()
{
	
}

HardWareCfg::~HardWareCfg()
{
	
}

int HardWareCfg::Init()
{
	Json::Value cfgroot;

	if(PublicTool::LoadJsonCfg(cfgroot, "HardWareCfg.json"))
		return -1;
	
	if(!cfgroot.isObject())
	{
		log_error("HardWareCfg is Not Object");
		return -1;
	}

	if(cfgroot.isMember("RS485") && InitRs485Info(cfgroot["RS485"]))
	{
		return -1;
	}

	if(cfgroot.isMember("CAN") && InitCanInfo(cfgroot["CAN"]))
	{
		return -1;
	}

	if(cfgroot.isMember("DI") && InitDiInfo(cfgroot["DI"]))
	{
		return -1;
	}

	if(cfgroot.isMember("DO") && InitDoInfo(cfgroot["DO"]))
	{
		return -1;
	}
	return 0;
}

const std::string& HardWareCfg::GetRs485Name(int i)
{
	if(i >= m_vec_rs485.size())
		return PublicTool::GetNullString();
	return m_vec_rs485[i];
}

const std::string& HardWareCfg::GetCanName(int i)
{
	if(i >= m_vec_can.size())
		return PublicTool::GetNullString();
	return m_vec_can[i];
}

const int HardWareCfg::GetDiPin(int i)
{
	if(i >= m_vec_di.size())
		return -1;
	return m_vec_di[i];
}

const int HardWareCfg::GetDoPin(int i)
{
	if(i >= m_vec_do.size())
		return -1;
	return m_vec_do[i];
}

int HardWareCfg::InitRs485Info(const Json::Value& value)
{
	if(value.isArray())
	{
		const Json::Value& v = value;
		for(int i = 0; i < value.size(); ++i)
		{
			m_vec_rs485.push_back(v[i].asString());
		}
	}
	return 0;
}

int HardWareCfg::InitCanInfo(const Json::Value& value)
{
	if(value.isArray())
	{
		const Json::Value& v = value;
		for(int i = 0; i < value.size(); ++i)
		{
			m_vec_can.push_back(v[i].asString());
		}
	}
	return 0;
}

int HardWareCfg::InitDiInfo(const Json::Value& value)
{
	if(value.isArray())
	{
		const Json::Value& v = value;
		for(int i = 0; i < value.size(); ++i)
		{
			m_vec_di.push_back(v[i].asInt());
		}
	}
	return 0;
}
	
int HardWareCfg::InitDoInfo(const Json::Value& value)
{
	if(!value.isArray())
	{
		const Json::Value& v = value;
		for(int i = 0; i < value.size(); ++i)
		{
			m_vec_di.push_back(v[i].asInt());
		}
	}
	return 0;
}
