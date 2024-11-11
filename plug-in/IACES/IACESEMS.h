#pragma once

#include "core/TimeEventHander.h"
#include "core/ASyncCallSupport.h"
#include "EMSDevice.h"
#include "core/TimeNode.h"
#include <json/json.h>
#include "rapidjson/document.h"

class ESSDevice;
class PCSDevice;
class MeterDevice;
class BCMUDevice;
class BMUDevice;
class DiDevice;
class EnergySchedule;

struct PCSData;
struct BCMUData;
struct MeterData;

enum DEVICETHREAD
{
	EMS_THREAD = 1,
	PCS_THREAD,
	BCMU_THREAD,
	METER_THREAD,
	DEHU_THREAD,
	DI_THREAD,
	DATABASE_THREAD,
};

#pragma pack(1)
struct IACESEMSData
{
	float HandSetPower;		//当前设置的功率
};
#pragma pack()

class IACESEMS : public EMSDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
	IACESEMS();
	~IACESEMS();
	
public:
	virtual int 	InitEMS();
	virtual int 	StartEMS();
	Device* 		GetTopDevice();
	virtual int 	ProcessTimeOutEvent(UINT32 nTimeId);
	virtual int 	Initialize(int commtype, ASyncCallDataInst& initdata);
	virtual int 	SetDataPtr(char *ptr);
	virtual void 	DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst & pReq);
	virtual void 	DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst & pResp);

	virtual float 	GetGridActPower(); 			//获取市电电表有功功率
	virtual float 	GetLocalActPower();			//获取计量电表有功功率

	virtual void 	SetPower(float p);			//设置功率
	virtual UINT16  GetSoc();
	virtual float   GetMaxChargePower();
	virtual float   GetMaxDisChargePower();

	float GetGridMeterActivePower(){return 0.f;}

	virtual int 	CreateDevice(Json::Value& value);
	virtual void 	HandOtherThings();

public:
	const PCSData* GetPcsData();
	const BCMUData* GetBcmuData();
	const MeterData* GetGridMeterData();
	const MeterData* GetInvtMeterData();

protected:
	void GetDeviceConnParam(int& connType,ASyncCallDataInst& pConnParam,const Json::Value& ConnParam = Json::Value::nullRef);
	
//初始化相应的设备
private:
	int InitESSDevice(Json::Value& value);
	int InitEmsDevice(Json::Value& value);
	int InitPcsDevice(Json::Value& value);
	int InitBmsDevice(Json::Value& value);
	int InitBmuDevices(Json::Value& value);
	int InitGridMeter(Json::Value& value);
	int InitInvtMeter(Json::Value& value);
	int InitEmergencyDi();

	/*int InitESSDevice(rapidjson::Document& value);
	int InitEmsDevice(rapidjson::Document& value);
	int InitPcsDevice(rapidjson::Document& value);
	int InitBmsDevice(rapidjson::Document& value);
	int InitBmuDevices(rapidjson::Document& value);
	int InitGridMeter(rapidjson::Document& value);
	int InitInvtMeter(rapidjson::Document& value);*/

private:
	
	bool GetThreePhasePower(float& pA, float& pB, float& pC);
	
protected:
	char* m_sz_data_pool;				//数据池
	int   m_n_malloc;					//已分配的字节长度
	IACESEMSData* m_p_ems_other_data;
	
	ESSDevice *m_pEss;
	PCSDevice *m_pPcs;
	int m_n_invt_meter_num;				//计量电表个数
	MeterDevice *m_pInvtMeter;			//计量电表
	MeterDevice *m_pGridMeter;			//防逆流电表
	BCMUDevice *m_pBcmu;
	int m_n_bmu_num;					//bmu个数
	BMUDevice **m_pBmu;				
	DiDevice *m_pEmergencyStop;
	EnergySchedule* m_pTOU;
	CTimer m_timer_test;
};