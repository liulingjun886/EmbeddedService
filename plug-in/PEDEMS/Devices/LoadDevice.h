#pragma once
#include "Device.h"
#include "DevStateDef.h"

//负荷
#pragma pack(1)

typedef struct stLoadData
{
	float needp;
	float p;
}LoadData;

#pragma pack()

class LoadDevice : public Device
{
public:
	LoadDevice();
	~LoadDevice();

public:
	virtual int  GetDataLen();
	virtual void TurnOnOrOff(RUNSTATE s)=0;
	
public:
	const LoadData* GetData();	
	
protected:
	virtual int SetDataPtr(char * ptr);
	
private:
	LoadData* m_ld_data;
};