#pragma once

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();

public:
    int Init(const char *device, int baud, char parity, int data_bit, int stop_bit);
    int SendData(char* pData, int nDataLen);
    int RecvData(char* pData, int nDataLen);
    int DataExchange(char* pSendData, int nSendLen, char* pRecvData, int pRecvLen);
private:
    int Open(const char *device);
    int ConfigSerialPort(int baud, char parity, int data_bit, int stop_bit);
    int UartSpeed(int s);
    int CheckTimeOut();
private:
    int m_fd;       // 串口fd
    int m_time_out; // 通信超时时间
};