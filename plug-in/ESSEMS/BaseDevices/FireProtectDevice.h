#pragma once

#pragma pack(1)

//消防数据
struct FireProtectData
{
	UINT16 m_state_fire_sprinkler;	//消防喷射状态
	UINT16 m_state_running;		   	//运行状态
	INT16  m_i16_temp;				//温度数值
	UINT16 m_concentration_CO; 		//一氧化碳数值
	UINT16 m_gas_sensitive_smoke;	//气敏烟雾数值
	UINT16 m_photoelectric_smoke;	//光电烟雾数值
	UINT16 m_sz_alarm[2];			//告警数据

	FireProtectData()
	{
		Reset();
	}

	void Reset()
	{
		memset(this,0,sizeof(FireProtectData));
	}
};

#pragma pack()


class FireProtectDevice : public Device
{
protected:
	FireProtectData *m_pFireProtectData;

private:
	bool m_b_free;
};
