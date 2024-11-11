#pragma once

#include "Device.h"
#include "DataPool.h"

#pragma pack(1)

struct TOUTime
{
	UINT16 m_u16_starthour;
	UINT16 m_u16_startmin;
	UINT16 m_u16_endhour;
	UINT16 m_u16_endmin;
	 INT16 m_i16_Power;
};

struct EMSData
{
	UINT16 m_u16_year;					//年
	UINT16 m_u16_mon;					//月
	UINT16 m_u16_day;					//日
	UINT16 m_u16_cpuuse;				//EMS CPU使用率
	UINT16 m_u16_memuse;				//EMS 内存使用率
	UINT16 m_u16_diskuse;				//EMS 磁盘使用率
	UINT16 m_u16_workstate;				//工作状态 0 停止 1 启动
	UINT16 m_u16_workmode;				//工作模式 0 手动 1 TOU
	char   m_sz_imei[20];				//SIM卡IMEI
	char   m_sz_iccid[20];				//SIM卡iccid
	UINT16 m_u16_maxsoc;				//TOU最大soc
	UINT16 m_u16_minsoc;				//TOU最小soc
	TOUTime m_sz_tou[10];				//TOU时间段
	UINT16 m_u16_power_demand; 			//变压器需量
	UINT16 m_u16_power_antibackflow; 	//防逆流值
	UINT16 m_u16_master;				//主从标识
	UINT16 m_u16_pv_consume;			//光伏消纳
	UINT16 m_u16_demand_dynamic;		//动态增容
};

#pragma pack()


class EMSDevice : public Device
{
public:
	virtual int GetThreadNum() {return 8;}
	virtual int InitEMS() = 0;
	virtual int StartEMS() = 0;
	virtual Device* GetTopDevice() = 0;
	virtual float GetGridMeterActivePower() = 0;			  //获取电网侧电网有功功率
	virtual void SetPower(float p) = 0;
	virtual UINT16 GetSoc() = 0;
	virtual float GetMaxChargePower() = 0;
	virtual float GetMaxDisChargePower() = 0;
public:
	virtual int SetDataPtr(char *ptr)
	{
		int nBasicDataLen = Device::SetDataPtr(ptr);
		m_pEMSData = (EMSData *)(ptr + nBasicDataLen);
		return sizeof(EMSData)+nBasicDataLen;
	}
	const EMSData* GetEmsData()	{return m_pEMSData;}
protected:
	EMSData *m_pEMSData;	
};
