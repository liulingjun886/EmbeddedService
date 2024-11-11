#pragma once

#define NONE 0
#define RS485 1
#define MODBUS_RTU 2
#define MODBUS_TCP_CLI 3
#define MODBUS_TCP_SRV 4
#define CAN 5

//串口通信
struct SerialPort
{
	char PortDev[32];				//设备节点
	int  nBaud;						//波特率
	int  nParity;					//校验位
	int  nDataBit;					//数据位
	int  nStopBit;					//停止位
};

//Modbus-RTU
struct ModbusRtuComm : public SerialPort
{
	int nSlaveId;					//设备Modbus地址
};

//TCP客户端
struct TcpCliComm
{
	char addr[46];					//设备地址
	unsigned short nPort;			//设备端口
	
};
//modbus-tcp
struct ModbusTcpCliComm : public TcpCliComm
{
	int nSlaveId;					//设备地址
};

struct ModbusTcpSrvComm
{
	int nPort;						//监听端口
	int nSlaveId;					//设备地址
};

struct CanComm
{
	int nCanId;						//can接口ID
	int nBand;						//can速度
};