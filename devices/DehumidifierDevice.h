#pragma once

#include <Device.h>

#pragma pack(1)
struct DehumidifierData
{
	UINT16 m_work_mode; //工作模式
	UINT16 m_dehumi_start_value; //除湿启控值
	UINT16 m_dehumi_stop_value;	//除湿停止值
	UINT16 m_temp_start_value;	//温度启控值
	UINT16 m_temp_end_value;	//温度控制回差值
	INT16  m_temp_coolingPlateDeicing;//冷凝片除冰值
	INT16  m_temp_environment;  //环境温度测量值
	UINT16 m_humidity_environment; //环境湿度测量值
	UINT16 m_alarm_data[2];			//告警数据
	DehumidifierData()
	{
		memset(this,0,sizeof(DehumidifierData));
	}
};
#pragma pack()

class DehumidifierDevice : public Device
{
public:
	DehumidifierDevice();
	~DehumidifierDevice();
public:
	virtual int SetDataPtr(char* ptr);
	const DehumidifierData* GetData();
	
protected:
	DehumidifierData* m_pHumidifyData;
};