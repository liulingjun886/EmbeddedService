/*-----------------------------
	液冷逻辑文件
-------------------------------*/

#include "IACESEMS.h"

class AirConditionDevice;


class WindColdEMS : public IACESEMS
{
public:
	WindColdEMS();
	~WindColdEMS();
public:
	virtual int CreateDevice(Json::Value& value);
	void 		HandOtherThings();
private:
	AirConditionDevice* m_pAirConditionDevice;
};


