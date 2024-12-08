#pragma once

#include <Device.h>
#include <DataPool.h>
#include "DevStateDef.h"


#pragma pack(1)

//PCS数据
struct PCSData
{
	UINT16 m_u16_dsp_v; 	// DSP-V版本
	UINT16 m_u16_dsp_b; 	// DSP-B版本
	UINT16 m_u16_dsp_d; 	// DSP-D版本
	UINT16 m_u16_cpld_v; 	// CPLD-V版本
	UINT16 m_u16_cpld_b; 	// CPLD-B版本
	UINT16 m_u16_cpld_d; 	// CPLD-D版本
	
	UINT16 m_u16_work_mode; // 当前实际充放电工作模式（0 电池恒压，1 并网恒流，2 电池恒流）
	UINT16 m_u16_work_state; // 工作状态 (0 停机 1 启动中 3 运行 4 故障 5 关闭中)
	
	float m_volt_ab; 		// 输出 AB 线电压
	float m_volt_bc; 		// 输出 BC 线电压
	float m_volt_ca; 		// 输出 CA 线电压
	float m_volt_out_a; 	// 输出 A 相电压
	float m_volt_out_b; 	// 输出 B 相电压
	float m_volt_out_c; 	// 输出 C 相电压
	float m_curr_out_a; 	// 输出 A 相电流
	float m_curr_out_b; 	// 输出 B 相电流
	float m_curr_out_c; 	// 输出 C 相电流
	float m_curr_indu_a; 	// 电感 A 相电流
	float m_curr_indu_b; 	// 电感 B 相电流
	float m_curr_indu_c; 	// 电感 C 相电流
	float m_freq_grid; 		// 电网频率
	float m_phase_seq_grid; // 当前电网相序
	float m_p_ac_a; 		// 交流 A 相有功功率
	float m_p_ac_b; 		// 交流 B 相有功功率
	float m_p_ac_c; 		// 交流 C 相有功功率
	float m_appap_ac_a; 	// 交流 A 相视在功率
	float m_appap_ac_b; 	// 交流 B 相视在功率
	float m_appap_ac_c; 	// 交流 C 相视在功率
	float m_reacp_ac_a; 	// 交流 A 相无功功率
	float m_reacp_ac_b; 	// 交流 B 相无功功率
	float m_reacp_ac_c; 	// 交流 C 相无功功率
	float m_p_ac_total; 	// 交流输出总有功功率
	float m_appap_ac_total; // 交流输出总无功功率
	float m_reacp_ac_total; // 交流输出总视在功率
	float m_power_factor_ac;// 交流功率因数
	float m_volt_bus_total; // 总母线电压
	float m_volt_bus_positive; // 正母线电压
	float m_volt_bus_negative; // 负母线电压
	float m_volt_battery; 	// 电池电压
	float m_curr_battery; 	// 电池电流
	float m_p_dc; 			// 直流功率
	float m_p_dc_total; 	// 直流总电流
	float m_temp_igbt; 		// IGBT 温度
	float m_temp_env; 		// 环境温度
	float m_temp_indu; 		// 电感温度
	UINT16 m_sz_alarm[8];
	
	PCSData()
	{
		Reset();
	}
	
	void Reset()
	{
		m_u16_dsp_v = 0;
		m_u16_dsp_b = 0;
		m_u16_dsp_d = 0;
		m_u16_cpld_v = 0;
		m_u16_cpld_b = 0;
		m_u16_cpld_d = 0;
		
		m_u16_work_mode = 0;
		m_u16_work_state = 0;
		
		m_volt_ab = 0;
		m_volt_bc = 0;
		m_volt_ca = 0;
		m_volt_out_a = 0;
		m_volt_out_b = 0;
		m_volt_out_c = 0;
		m_curr_out_a = 0;
		m_curr_out_b = 0;
		m_curr_out_c = 0;
		m_curr_indu_a = 0;
		m_curr_indu_b = 0;
		m_curr_indu_c = 0;
		m_freq_grid = 0;
		m_phase_seq_grid = 0;
		m_p_ac_a = 0;
		m_p_ac_b = 0;
		m_p_ac_c = 0;
		m_appap_ac_a = 0;
		m_appap_ac_b = 0;
		m_appap_ac_c = 0;
		m_reacp_ac_a = 0;
		m_reacp_ac_b = 0;
		m_reacp_ac_c = 0;
		m_p_ac_total = 0;
		m_appap_ac_total = 0;
		m_reacp_ac_total = 0;
		m_power_factor_ac = 0;
		m_volt_bus_total = 0;
		m_volt_bus_positive = 0;
		m_volt_bus_negative = 0;
		m_volt_battery = 0;
		m_curr_battery = 0;
		m_p_dc = 0;
		m_p_dc_total = 0;
		m_temp_igbt = 0;
		m_temp_env = 0;
		m_temp_indu = 0;
		memset(m_sz_alarm,0,sizeof(m_sz_alarm));
	}
};
#pragma pack()


class PCSDevice : public Device
{
public:
	PCSDevice();
	~PCSDevice();
	
public:
	const PCSData* GetData();
	
public:
	virtual int SetDataPtr(char * ptr);
	virtual int PowerOnOrOff(RUNSTATE state)=0;		//1 开机 0 关机
	virtual int Reset()=0;								//复位
	virtual int SetP(float p)=0;						//设置有功功率 正放 负充 单位kw
	virtual int SetP(float pA, float pB, float pC)=0;	//设置分三相设置有功功率
	virtual int SetAppaP(float p)=0;					//设置无功功率
	virtual int SetPowerFactor(float factor)=0;			//设置功率因数
	virtual int GetRatePower()=0;						//获取PCS额定功率
	
protected:
	PCSData    *m_pcs_data; 							//这里用指针，方便后期做内存映射
};
