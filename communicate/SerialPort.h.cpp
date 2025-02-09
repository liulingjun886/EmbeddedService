#include "SerialPort.h"
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>

SerialPort::SerialPort():m_fd(-1)
{

}

SerialPort::~SerialPort()
{
    if(m_fd > 0)
    {
        close(m_fd);
        m_fd = -1;
    }
}

int SerialPort::Init(const char *device, int baud, char parity, int data_bit, int stop_bit)
{
    if(0 != Open(device))
        return -1;
    ConfigSerialPort(baud,parity,data_bit,stop_bit);
    return 0;
}

int SerialPort::SendData(char* pData, int nDataLen)
{
    return write(m_fd,pData,nDataLen);
}

int SerialPort::RecvData(char* pData, int nDataLen)
{
    return read(m_fd,pData,nDataLen);
}

int SerialPort::DataExchange(char* pSendData, int nSendLen, char* pRecvData, int pRecvLen)
{
    int ret = -1;
    ret = SendData(pSendData,nSendLen);
    if(ret < 0)
        return -1;

    ret = CheckTimeOut();
    if(ret < 0)
        return -1;

    return RecvData(pRecvData,pRecvLen);
}

int SerialPort::Open(const char *device)
{
    m_fd = open(device, O_RDWR); 
 
    if(m_fd < 0)
    { 
        return -1; 
    }
    return 0;
}

int SerialPort::ConfigSerialPort(int baud, char parity, int data_bit, int stop_bit)
{
    struct termios oldtio,newtio; 
    //得到当前的配置 
    tcgetattr(m_fd, &oldtio); 
    //将新配置初始化为 0 
    memset(&newtio, 0, sizeof(struct termios));
    
    newtio.c_cflag |= (CLOCAL | CREAD); 
    
    //设置波特率为 115200bps 
    //newtio.c_cflag |= UartSpeed(baud); 
    
    //设置停止位 
    switch (stop_bit) { 
    case 2: /* 2 stopbits */ 
        newtio.c_cflag |= CSTOPB; 
        break; 
    case 1: /* 1 stopbits */ 
    default: 
        newtio.c_cflag &= ~CSTOPB; 
        break; 
    } 
    
    //设置寄偶校验 
    switch (parity) { 
    case 1: /* Odd parity */ 
        newtio.c_cflag |= (PARENB | PARODD); 
        break; 
    case 2: /* Even parity */ 
        newtio.c_cflag |= PARENB; 
        newtio.c_cflag &= ~(PARODD); 
        break; 
    case 0: /* None parity */ 
    default: 
        newtio.c_cflag &= ~(PARENB); 
        break; 
    } 
    //设置数据位 
    newtio.c_cflag&=~CSIZE; 
    switch (data_bit) { 
    case 5: 
        newtio.c_cflag |= CS5; 
        break; 
    case 6: 
        newtio.c_cflag |= CS6; 
        break; 
    case 7: 
        newtio.c_cflag |= CS7; 
        break; 
    case 8: 
    default: 
        newtio.c_cflag |= CS8; 
        break; 
    } 
    //设置无硬件流控 
    newtio.c_cflag&=~CRTSCTS; 

    newtio.c_lflag=0; 
    newtio.c_oflag=0; 
    newtio.c_cc[VMIN]=4; 
    newtio.c_cc[VTIME]=0; 
    newtio.c_iflag &= ~(IXON|IXOFF|IXANY); 
    cfsetispeed(&newtio, UartSpeed(baud)); 
    cfsetospeed(&newtio, UartSpeed(baud)); 
    tcsetattr(m_fd, TCSANOW, &newtio); 
    tcflush(m_fd, TCIFLUSH);
    return 0;
}

int SerialPort::UartSpeed(int s)
{
    switch (s) 
    {
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    case 460800:
        return B460800;
    case 500000:
        return B500000;
    case 576000:
        return B576000;
    case 921600:
        return B921600;
    case 1000000:
        return B1000000;
    case 1152000:
        return B1152000;
    case 1500000:
        return B1500000;
    case 2000000:
        return B2000000;
    case 2500000:
        return B2500000;
    case 3000000:
        return B3000000;
    case 3500000:
        return B3500000;
    case 4000000:
        return B4000000;
    default:
        return B0;
    }
}

int SerialPort::CheckTimeOut()
{
    fd_set rd;
    timeval timeout;
    int ret = -1;

    timeout.tv_sec = m_time_out/1000;
	timeout.tv_usec = (m_time_out%1000) * 1000;
LOOP:
    FD_ZERO(&rd);
    FD_SET(m_fd,&rd);
	ret = select(m_fd+1, &rd, NULL, NULL, &timeout);
    if(-1 == ret)
    {
        if(EINTR == errno)
            goto LOOP;
        return -1;
    }
    return ret;
}