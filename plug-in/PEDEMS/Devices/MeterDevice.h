#pragma once
#include "Device.h"
#include "DevStateDef.h"

#pragma pack(1)
//电表数据
struct MeterData
{
	float m_volt_a; //A相电压
	float m_volt_b; //B相电压
	float m_volt_c; //C相电压

	float m_curr_a; //A相电流
	float m_curr_b; //B相电流
	float m_curr_c; //C相电流

	float m_p_a; //A相有功功率
	float m_p_b; //B相有功功率
	float m_p_c; //C相有功功率
	float m_p_total; //总有功功率

	float m_preac_a; //A相无功功率
	float m_preac_b; //B相无功功率
	float m_preac_c; //C相无功功率
	float m_preac_total; //总无功功率

	float m_pappa_a; //A相视在功率
	float m_pappa_b; //B相视在功率
	float m_pappa_c; //C相视在功率
	float m_pappa_total; //总视在功率

	float m_factor_a; //A相功率因数
	float m_factor_b; //B相功率因数
	float m_factor_c; //C相功率因数
	float m_factor_total; //总功率因数

	float m_freq_grid;//电网频率

	float m_act_eng_comb_total; //当前组合有功总电能
	float m_act_eng_comb_sharp;	//当前组合有功尖电能
	float m_act_eng_comb_peak;	//当前组合有功峰电能
	float m_act_eng_comb_flat;	//当前组合有功平电能
	float m_act_eng_comb_valley;//当前组合有功谷电能

	float m_act_eng_forward_total; //当前正向有功总电能
	float m_act_eng_forward_sharp;	//当前正向有功尖电能
	float m_act_eng_forward_peak;	//当前正向有功峰电能
	float m_act_eng_forward_flat;	//当前正向有功平电能
	float m_act_eng_forward_valley;//当前正向有功谷电能

	float m_act_eng_reverse_total; //当前反向有功总电能
	float m_act_eng_reverse_sharp;	//当前反向有功尖电能
	float m_act_eng_reverse_peak;	//当前反向有功峰电能
	float m_act_eng_reverse_flat;	//当前反向有功平电能
	float m_act_eng_reverse_valley;//当前反向有功谷电能

	MeterData()
	{
		Reset();
	}
	
	void Reset()
	{
		m_volt_a = 0.f; //A相电压
		m_volt_b = 0.f; //B相电压
		m_volt_c = 0.f; //C相电压

		m_curr_a = 0.f; //A相电流
		m_curr_b = 0.f; //B相电流
		m_curr_c = 0.f; //C相电流

		m_p_a = 0.f; //A相有功功率
		m_p_b = 0.f; //B相有功功率
		m_p_c = 0.f; //C相有功功率
		m_p_total = 0.f; //总有功功率

		m_preac_a = 0.f; //A相无功功率
		m_preac_b = 0.f; //B相无功功率
		m_preac_c = 0.f; //C相无功功率
		m_preac_total = 0.f; //总无功功率

		m_pappa_a = 0.f; //A相视在功率
		m_pappa_b = 0.f; //B相视在功率
		m_pappa_c = 0.f; //C相视在功率
		m_pappa_total = 0.f; //总视在功率

		m_factor_a = 0.f; //A相功率因数
		m_factor_b = 0.f; //B相功率因数
		m_factor_c = 0.f; //C相功率因数
		m_factor_total = 0.f; //总功率因数

		m_freq_grid = 0.f;//电网频率

		m_act_eng_comb_total = 0.f; //当前组合有功总电能
		m_act_eng_comb_sharp = 0.f;	//当前组合有功尖电能
		m_act_eng_comb_peak = 0.f;	//当前组合有功峰电能
		m_act_eng_comb_flat = 0.f;	//当前组合有功平电能
		m_act_eng_comb_valley = 0.f;//当前组合有功谷电能

		m_act_eng_forward_total = 0.f; //当前正向有功总电能
		m_act_eng_forward_sharp = 0.f;	//当前正向有功尖电能
		m_act_eng_forward_peak = 0.f;	//当前正向有功峰电能
		m_act_eng_forward_flat = 0.f;	//当前正向有功平电能
		m_act_eng_forward_valley = 0.f;//当前正向有功谷电能

		m_act_eng_reverse_total = 0.f; //当前反向有功总电能
		m_act_eng_reverse_sharp = 0.f;	//当前反向有功尖电能
		m_act_eng_reverse_peak = 0.f;	//当前反向有功峰电能
		m_act_eng_reverse_flat = 0.f;	//当前反向有功平电能
		m_act_eng_reverse_valley = 0.f;//当前反向有功谷电能
	}
};
#pragma pack()

class MeterDevice : public Device
{
public:
	MeterDevice();
	~MeterDevice();

public:
	const MeterData* GetData();
	
public:
	virtual int  GetDataLen();
	virtual char* GetLocalData(int& nlen);	//获取本地显示屏数据
	
protected:
	virtual int SetDataPtr(char * ptr);

protected:
	MeterData* m_pMeterData;
};