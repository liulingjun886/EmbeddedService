#include "ModbusOperater.h"
#include "modbus/modbus.h"
#include "Log.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>



ModbusOperator::ModbusOperator():m_pCtx(nullptr),m_b_reconnect(false),m_modbus_type(UNDEFINE)
{
	
}

ModbusOperator::~ModbusOperator()
{
	if(m_pCtx)
	{
		modbus_close(m_pCtx);
		modbus_free(m_pCtx);
	}
}

int ModbusOperator::CreateCtx(const std::string& devPath,int nBaud, int nDataBit,int nParity, int nStopBit)
{
	m_pCtx = modbus_new_rtu(devPath.c_str(),nBaud,nParity,nDataBit,nStopBit);
	if(!m_pCtx)
	{
		log_error("modbus_new_rtu failer %s %d %d %d %d",devPath.c_str(),nBaud,nParity,nDataBit,nStopBit);
		return -1;
	}
	
	/*if(RS485 && modbus_rtu_set_serial_mode(m_pCtx,MODBUS_RTU_RS485))
	{
		log_error("enable rs485 failer");
		return -1;
	}*/
	
	/*if(RS485 && EnableRs485())
	{
		log_error("enable rs485 failer");
		return -1;
	}*/
		
	m_modbus_type = MODBUS_RTU;
	return 0;
}

int ModbusOperator::EnableRs485()
{
	struct serial_rs485 rs485conf;
	if (ioctl(modbus_get_socket(m_pCtx), TIOCGRS485, &rs485conf) < 0) 
	{
		
		log_error("get TIOCGRS485 failer %d",modbus_get_socket(m_pCtx));
    	return -1;
    }

    rs485conf.flags |= SER_RS485_ENABLED;
    rs485conf.delay_rts_before_send = 0x00000004;

    if (ioctl(modbus_get_socket(m_pCtx), TIOCSRS485, &rs485conf) < 0)
    {
        return -1;
    }
	
	return 0;
}


int ModbusOperator::CreateCtx(const std::string& ipAddr,UINT16 nPort)
{
	m_pCtx = modbus_new_tcp(ipAddr.c_str(),nPort);
	if(!m_pCtx)
		return -1;
	
	m_modbus_type = MODBUS_TCP;
	return 0;
}

int ModbusOperator::InitCtx(int nSalveId, int nTimeOut)
{
	if(!m_pCtx)
		return -1;
	if(modbus_set_slave(m_pCtx, nSalveId))
		return -2;
	if(modbus_set_response_timeout(m_pCtx,nTimeOut/1000,(nTimeOut%1000)*1000))
		return -3;
	return 0;
}

int ModbusOperator::Connect()
{
	if(!m_pCtx)
		return -1;

	if(MODBUS_TCP == m_modbus_type)
		m_b_reconnect = true;
	
	return modbus_connect(m_pCtx);
}

int ModbusOperator::ReConnect()
{
	if(!m_pCtx)
		return -1;
		
	if(!m_b_reconnect)
		return -2;
	
	modbus_close(m_pCtx);
	return Connect();
}

void ModbusOperator::AttachCtx(int fd, bool flag)
{
	if(!m_pCtx)
		return;
		
	if(flag)
		modbus_close(m_pCtx);
	
	modbus_set_socket(m_pCtx, fd);
}

int ModbusOperator::ReadInputRegisters(int regAddr, int regNum, UINT16* pData)
{
	if(!m_pCtx)
		return -1;
	return modbus_read_input_registers(m_pCtx, regAddr, regNum, pData);
}

int ModbusOperator::ReadRegisters(int regAddr, int regNum, UINT16* pData)
{
	if(!m_pCtx)
		return -1;
	return modbus_read_registers(m_pCtx, regAddr, regNum, pData);
}

int ModbusOperator::WriteRegister(int regAddr, UINT16 data)
{
	if(!m_pCtx)
		return -1;
	return modbus_write_register(m_pCtx, regAddr, data);
}

int ModbusOperator::WriteRegisters(int regAddr, int regNum, UINT16* pData)
{
	if(!m_pCtx)
		return -1;
	return modbus_write_registers(m_pCtx, regAddr, regNum, pData);
}

