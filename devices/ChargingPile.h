#pragma once

struct ChargingData
{
	
	float ActPower;		//当前功率
	float ReqPower;		//需求功率
	float LimitPower;	//限制功率
};

class ChargingPile : public Device
{
	
};