#include "WaterColdEms.h"
#include "LiquidCoolerDevice.h"
#include "Log.h"
#include "DeviceManager.h"
#include "ESSDevice.h"
#include "DehumidifierDevice.h"

WaterColdEMS::WaterColdEMS()
{
	
}

WaterColdEMS::~WaterColdEMS()
{

}

int WaterColdEMS::CreateDevice(Json::Value& value)
{
	if(IACESEMS::CreateDevice(value))
		return -1;

	if(!value["LiquidCooler"].isObject() || InitLiquidDevice(value["LiquidCooler"]))
		return -1;
	log_info("Create LiquidCooler Devices");
	return 0;
}

int WaterColdEMS::StartEMS()
{
	m_pLiquidCooler->SetEnableState(ENABLE);
	//m_pDehumidifier->SetEnableState(ENABLE);
	return IACESEMS::StartEMS();
}


void WaterColdEMS::HandOtherThings()
{
	//todo:温控排风扇啥的
}

int WaterColdEMS::InitLiquidDevice(Json::Value& value)
{
	m_pLiquidCooler = dynamic_cast<LiquidCoolerDevice *>(DeviceManager::GetInstance()->CreateDevice(value["model"].asString()));
	if(!m_pLiquidCooler)
	{
		log_error("create LiquidDevice failer");
		return -1;
	}

	CAsyncCallSupport* pAsync = dynamic_cast<CAsyncCallSupport*>(m_pLiquidCooler);
	if(pAsync)
	{
		pAsync->SetThreadIndex(METER_THREAD);
	}

	m_n_malloc += m_pLiquidCooler->SetDataPtr(m_sz_data_pool+m_n_malloc);

	int connType = 0;
	ASyncCallDataInst data;
	GetDeviceConnParam(connType,data,value);

	if(m_pLiquidCooler->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), connType, data))
	{
		log_error("Liquid init failer");
		return -1;
	}
	m_pLiquidCooler->SetParent(m_pEss);
	m_pLiquidCooler->SetStandardlyInterval(1000);
	return 0;
}

int WaterColdEMS::InitDehumiDevice(Json::Value& value)
{
	return 0;
}

