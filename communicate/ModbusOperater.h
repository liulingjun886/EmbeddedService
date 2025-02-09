#pragma once
#include <string>
#include "core/UserTypeDefines.h"
#include "core/TimeEventHander.h"
#include "core/TimeNode.h"

typedef struct _modbus modbus_t;

enum Modbus_Type
{
	UNDEFINE = 0,
	MODBUS_RTU,
	MODBUS_TCP,
};

class ModbusOperator
{
public:
	static int InitModbusOperaterHander(ModbusOperator* p, int type, const char * initdata, UINT32 nTimeOut);
	ModbusOperator();
	~ModbusOperator();
public:
	int CreateCtx(const std::string& devPath,int nBaud, int nDataBit,int nParity, int nStopBit);
	int CreateCtx(const std::string& ipAddr,UINT16 nPort);
	int InitCtx(int nSalveId, int nTimeOut);
	int Connect();
	int ReConnect();
	void AttachCtx(int fd,bool flag = true);

	int ReadInputRegisters(int regAddr, int regNum, UINT16* pData);
	int ReadRegisters(int regAddr, int regNum, UINT16* pData);
	int WriteRegister(int regAddr, UINT16 data);
	int WriteRegisters(int regAddr, int regNum, UINT16* pData);
	int EnableRs485();

	int EnableDebug();
	
private:
	modbus_t* m_pCtx;
	bool m_b_reconnect;
	Modbus_Type m_modbus_type;
};