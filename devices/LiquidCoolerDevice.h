#pragma once
#include "Device.h"
#include "DataPool.h"


enum LiquidCoolerWorkState
{
	COOLER_POWERON=1,
	COOLER_POWEROFF,
};

enum LiquidCoolerWorkMode
{
	COOLER_STOP=1,
	INTERNALCIRCULATION,
	COOLER_COOL,
	COOLER_HEAT,
	COOLER_AUTO,
};


#pragma pack(1)

//液冷机数据
struct LiquidCoolerData
{
	UINT16 m_work_mode;				//工作模式
	UINT16 m_work_state;			//工作状态
	UINT16 m_warn_level;			//当前告警最高等级
	UINT16 m_temp_cool;				//水温制冷点
	UINT16 m_temp_heat;				//水温加热点
	UINT16 m_temp_water_out;		//出水温度
	UINT16 m_temp_water_back;		//回水温度
	UINT16 m_pressure_back;			//进水压力
	UINT16 m_pressure_out;			//出水压力
	UINT16 m_tmep_enev;				//环境温度
	UINT16 m_temp_exhaust;			//排气温度
	UINT16 m_water_pump_state;		//水泵状态
	UINT16 m_water_pump_speed;		//水泵转速
	UINT16 m_compressor_state;		//压缩机状态
	UINT16 m_sz_alarm[4];			//告警数据

	LiquidCoolerData()
	{
		Reset();
	}

	void Reset()
	{
		memset(this,0,sizeof(LiquidCoolerData));
	}
};

#pragma pack()

class LiquidCoolerDevice : public Device
{
public:
	LiquidCoolerDevice();
	~LiquidCoolerDevice();

public:
	virtual int SetDataPtr(char* ptr);
	virtual void PowerOnOrOff(LiquidCoolerWorkState state);
	virtual void SetWorkMode(LiquidCoolerWorkMode mode);
	
public:
	const LiquidCoolerData* GetData();

protected:
	LiquidCoolerData* m_LiquidCoolerData;
};
