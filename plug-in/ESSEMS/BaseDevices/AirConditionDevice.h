#pragma once

#include "Device.h"
#include "DataPool.h"

#pragma pack(1)
//空调数据
struct AirConditionData
{
	UINT16 m_u16_state_working; 				//工作状态
	UINT16 m_u16_state_inside_fan_switch;		//内风机 
	UINT16 m_u16_state_outside_fan_switch;		//外风机
	UINT16 m_u16_state_compressor_switch;		//压缩机状态
	UINT16 m_u16_state_electric_heating_switch;	//电加热
	UINT16 m_u16_state_emergency_fan_switch;	//应急风机

	UINT16 m_temp_coil;						//盘管温度
	UINT16 m_temp_outside;					//室外温度
	UINT16 m_temp_condensation;				//冷凝温度
	UINT16 m_temp_inside;					//室内温度
	UINT16 m_humidity;						//湿度
	UINT16 m_temp_aerofluxus;				//排气温度
	UINT16 m_curr;							//电流
	UINT16 m_volt_ac;						//交流电压
	UINT16 m_volt_dc;						//直流电压
	UINT16 m_sz_alarm[4];					//告警变量按位算

	AirConditionData()
	{
		Reset();
	}

	void Reset()
	{
		memset(this,0,sizeof(AirConditionData));
	}
};
#pragma pack()

class AirConditionDevice : public Device
{
public:
	AirConditionDevice();
	~AirConditionDevice();
	
public:
	virtual int SetDataPtr(char* ptr);
	virtual int GetDataLen();				//获取数据长度
	virtual void TurnOnOrOff(int nState){}

protected:
	AirConditionData* m_pAirConditionData;
};