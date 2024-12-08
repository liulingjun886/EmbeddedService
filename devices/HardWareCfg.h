#pragma once

#include <vector>
#include <string>
#include <json/json.h>


class HardWareCfg
{
public:
	static HardWareCfg* GetInstance();
	
public:
	int Init();
	const std::string& GetRs485Name(int i);
	const std::string& GetCanName(int i);
	const int GetDiPin(int i);
	const int GetDoPin(int i);
	
private:
	HardWareCfg();
	~HardWareCfg();

	int InitRs485Info(const Json::Value& value);
	int InitCanInfo(const Json::Value& value);
	int InitDiInfo(const Json::Value& value);
	int InitDoInfo(const Json::Value& value);
private:
	std::vector<std::string> m_vec_rs485;
	std::vector<std::string> m_vec_can;
	std::vector<int> m_vec_di;
	std::vector<int> m_vec_do;
};
