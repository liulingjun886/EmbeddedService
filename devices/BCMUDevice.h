#pragma once

#include "Device.h"
#include "DataPool.h"

enum BCMUOPER
{
	BMSPOWEROFF,
	BMSPOWERON,
};

enum RackPrechargePhase
{
	Disconnected = 0,
	Start,
	Connecting,
	Connected,
	Connectionfail,
};

#pragma pack(1)
//BCMU数据
struct BCMUData
{
	UINT16 m_u16_state_contactor;	//接触器状态（0x0: 断开, 0x1: 闭合）
	UINT16 m_u16_fault_other_device;//对外故障（0x0: 正常, 0x1: 故障） 
	UINT16 m_u16_alarm_frist_level;	//一级报警（0x0: 正常, 0x1: 一级报警）
	UINT16 m_u16_alarm_second_level;//二级报警（0x0: 正常, 0x1: 二级报警）
	UINT16 m_u16_alarm_third_level;	//三级报警（0x0: 正常, 0x1: 三级报警）
	UINT16 m_u16_volt_precharge;	//预充总压
	UINT16 m_u16_volt_total;		//电池总电压
	UINT16 m_u16_state_working;		//充放电指示（0x0: 静置; 0x1: 放电; 0x2: 充电）
	UINT16 m_u16_soc;				//SOC
	UINT16 m_u16_soh;				//SOH
	UINT16 m_u16_insulation;		//绝缘值
	UINT16 m_u16_insulation_positive;//正极绝缘值
	UINT16 m_u16_insulation_negative;//负极绝缘值
	UINT16 m_u16_curr_max_charge;	//最大充电电流
	UINT16 m_u16_curr_max_discharge;//最大放电电流
	UINT16 m_u16_volt_max_index;	//单体电压最高号
	UINT16 m_u16_volt_max_value;	//单体最高电压值
	UINT16 m_u16_volt_min_index;	//单体电压最低号
	UINT16 m_u16_volt_min_value;	//单体最低电压值
	UINT16 m_u16_max_index;			//单体温度最高号
	UINT16 m_u16_temp_max_value;	//单体温度最高值
	UINT16 m_u16_temp_min_index;	//单体温度最低号
	UINT16 m_u16_temp_min_value;	//单体温度最低值
	UINT16 m_u16_volt_average;		//平均电压
	UINT16 m_u16_temp_average;		//平均温度
	UINT16 m_u16_temp_hvbox1;		//高压箱温度1
	UINT16 m_u16_temp_hvbox2;		//高压箱温度2
	UINT16 m_u16_temp_hvbox3;		//高压箱温度3
	UINT16 m_u16_temp_hvbox4;		//高压箱温度4
	UINT16 m_u16_kwh_can_charge;	//可充电量
	UINT16 m_u16_kwh_can_discharge;				//可放电量
	UINT16 m_u16_kwh_last_charge;				//最近一次充电电量
	UINT16 m_u16_kwh_last_discharge;			//最近一次放电电量
	UINT16 m_u16_concentration_h2;				//氢气浓度
	UINT16 m_u16_signal_aerosol;				//气溶胶信号
	//UINT16 m_u16_signal_electrical_emergencystop;//电气急停信号
	UINT16 m_u16_signal_bms_emergencystop;	//BMS急停信号
	UINT16 m_u16_state_maincontactor;		//主接触器状态
	UINT16 m_u16_state_precontactor;		//预充接触器状态
	UINT16 m_u16_state_dc_circuit_breaker;	//直流断路器状态
	UINT16 m_u16_state_ac_circuit_breaker;	//交流断路器状态
	UINT16 m_u16_state_door;				//门禁状态
	UINT16 m_u16_state_pack_fan;			//PACK风扇状态（0:关, 1:开）
	UINT16 m_u16_state_pack_fan_read;		//PACK风扇状态反馈（0x0: 正常, 0x1: 故障;）
	UINT16 m_u16_cnt_dooropen;				//电池柜开门次数
	float  m_curr_battery_total;			//电池总电流
	float  m_eng_change_total;				//累计充电电量
	float  m_eng_dischange_total;			//累计放电电量
	UINT16 m_u16_state_bmu_connect[4];		//通信状态
	UINT16 m_state_pack_fan[4];				//BMU1风扇
	UINT16 m_u16_pack_pwd[4];				//PACK风扇PWM占空比
	UINT16 m_u16_alarm[8];					//BCMU故障 按位算
	BCMUData()
	{
		Reset();
	}

	void Reset()
	{
		memset(this,0,sizeof(BCMUData));
	}
};
#pragma pack()

class BCMUDevice : public Device
{
public:
	BCMUDevice();
	~BCMUDevice();
	
public:
	virtual int SetDataPtr(char* pData);
	virtual int GetDataLen();				//获取数据长度
	virtual void PowerOnOrOff(int nState)=0;
	virtual float GetMaxChargePower() = 0;				//获取当前最大充电功率
	virtual float GetMaxDisChargePower() = 0;			//获取当前最大放电功率
	
public:
	const BCMUData* GetData();
	void SetBcmuNo(int nIndex); //设置BCMU序号
	int  GetBcmuNo() const;
	
protected:
	BCMUData *m_pBcmu;
	int m_n_bcmu_no;
};