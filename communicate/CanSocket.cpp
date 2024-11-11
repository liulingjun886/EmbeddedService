#include "CanSocket.h"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/socket.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Log.h"

enum CANINTERFACE
{
    CAN0 = 0,
    CAN1 = 1,
};

CanSocket::CanSocket():m_n_canid(-1),m_n_can_bitrate(-1)
{
	
}

void CanSocket::SetCanInfo(int id, int bitrate)
{
	m_n_canid = id;
	m_n_can_bitrate = bitrate;
}

int CanSocket::SendCanData(can_frame* pData)
{
	int nbytes = CPollerObject::SendData((char*)pData, sizeof(struct can_frame));
    if (nbytes < 0)
    {
        log_error("can send error: ");
        return -1;
    }
    return 0;
}

int CanSocket::Init()
{
	char cmd[256] = {0};
    int can_fd = 0;

    if (m_n_canid != CAN0 && m_n_canid != CAN1)
    {
        log_error("can%d is invalid",m_n_canid);
        return -1;
    }

    // can down
    sprintf(cmd, "ip link set can%d down", m_n_canid);
    system(cmd);

    // set can bitrate
    sprintf(cmd, "ip link set can%d type can bitrate %d", m_n_canid, m_n_can_bitrate);
    system(cmd);

    // can up
    sprintf(cmd, "ip link set can%d up", m_n_canid);
    system(cmd);

    if ((can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        log_error("init can%d fault!",m_n_canid);
        return -1;
    }

    // 关闭回环
    int loopback = 0;
    setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));

    can_err_mask_t err_mask;
    err_mask = (CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF);
    setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

    char name[8] = {0};
    struct ifreq can_ifr;
    struct sockaddr_can can_addr;

    sprintf(name, "can%d", m_n_canid);
    strcpy(can_ifr.ifr_name, name);
    if (ioctl(can_fd, SIOCGIFINDEX, &can_ifr) < 0)
    {
        log_error("match socket can%d fail:",m_n_canid);
        return -1;
    }

    can_addr.can_family = AF_CAN;
    can_addr.can_ifindex = can_ifr.ifr_ifindex;

    if (bind(can_fd, (struct sockaddr *)&can_addr, sizeof(can_addr)) < 0)
    {
        log_error("bind sock can%d fail:",m_n_canid);
        return -1;
    }

    log_info("init can%d OK\n", m_n_canid);
    m_n_poll_id = can_fd;
   	EnableInput();
   	m_can_recv_data.fill(0, sizeof(struct can_frame));
    return 0;
}

int CanSocket::InPutNotify()
{
	struct can_frame *pframe = (struct can_frame *)m_can_recv_data.data();
    int nbytes = read(m_n_poll_id, pframe, sizeof(struct can_frame));
    if (nbytes < 0)
    {
        log_error("can receive error: ");
        return -1;
    }
    return OnCanRecvComplete(pframe);
}


