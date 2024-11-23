/*-----------------------------
	液冷逻辑文件
-------------------------------*/

#include "IACESEMS.h"

class LiquidCoolerDevice;
class DehumidifierDevice;

class WaterColdEMS : public IACESEMS
{
public:
	WaterColdEMS();
	~WaterColdEMS();
public:
	virtual int CreateDevice(Json::Value& value);
	virtual int StartEMS();
	void 		HandOtherThings();
private:
	int 		InitLiquidDevice(Json::Value& value);
	int 		InitDehumiDevice(Json::Value& value);
private:
	LiquidCoolerDevice *m_pLiquidCooler;//液冷机
	DehumidifierDevice *m_pDehumidifier;//除湿机
};