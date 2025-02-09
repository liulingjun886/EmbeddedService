#pragma once

#include <core/UserTypeDefines.h>

#pragma pack(1)

//功率信息
typedef struct stRealPowerInfo
{
	UINT32 m_pe;				// 额定功率 w
	UINT32 m_p;					// 有功功率 w
	INT16  m_pq;				// 无功功率 var
	UINT32 m_pa;				// 视在功率 VA
	INT16  m_pf;				// 功率因数 0.0001
}RealPowerInfo;

//线电压
typedef struct stLineVolt
{
	UINT16 m_u_ab;			//ab线电压 0.1V
	UINT16 m_u_bc;			//bc线电压 0.1V
	UINT16 m_u_ac;			//ac线电压 0.1V
}LineVolt;

//相电压
typedef struct stPhaseVolt
{
	UINT16 m_u_a;			//a相电压 0.1V
	UINT16 m_u_b;			//b相电压 0.1V
	UINT16 m_u_c;			//c相电压 0.1V
}PhaseVolt;

//相电流
typedef struct stPhaseCurr
{
	UINT16 m_i_a;			//a相电流 0.1
	UINT16 m_i_b;			//b相电流 0.1
	UINT16 m_i_c;			//c相电流 0.1
}PhaseCurr;

//相功率
typedef struct stPhasePower
{
	UINT32 m_p_a;			//a相功率 w
	UINT32 m_p_b;			//b相功率 w
	UINT32 m_p_c;			//c相功率 w
}PhasePower;

//交流侧数据
typedef struct stAcInfo
{
	LineVolt m_lineVolt;		//线电压
	PhaseCurr m_phaseCurr;		//相电流
	PhasePower m_active_power;	//相有功功率
	PhasePower m_reactive_power;//相无功功率
	PhasePower m_apparent_power;//相视在功率
	RealPowerInfo m_power_info;	//总功率数据
	UINT16 	m_grid_frequency;	// 电网频率 0.1HZ
}AcInfo;

#pragma pack()