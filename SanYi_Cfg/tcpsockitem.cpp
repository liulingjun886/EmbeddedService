#include "tcpsockitem.h"
#include <QHostAddress>

TcpSockItem::TcpSockItem() : m_pSock(nullptr), m_b_reconnect(false), m_b_connected(false)
{
    m_recv_buff.clear();
    m_timer_connect.setSingleShot(false);
    connect(&m_timer_connect, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

TcpSockItem::~TcpSockItem()
{
    if (m_pSock)
    {
        m_pSock->close();
        m_pSock->deleteLater();
    }
}

int TcpSockItem::Init(const QString &addr, unsigned short port)
{
    m_b_reconnect = true;
    m_remote_addr = addr;
    m_remote_port = port;
    if (Initialize())
        return -1;
    m_timer_connect.start(5000);
    onTimerOut();
    return 0;
}

void TcpSockItem::Attach(QTcpSocket *pSock)
{
    m_pSock = pSock;
    QHostAddress addr = m_pSock->peerAddress();
    m_remote_addr = addr.toString();
    m_remote_port = m_pSock->peerPort();
    m_b_connected = true;
    ConnectSignalSlot();
    Initialize();
}

int TcpSockItem::SendData(const char *buff, int len)
{
    QByteArray data;
    data.append(buff, len);
    return SendData(data);
}

int TcpSockItem::SendData(const QByteArray &data)
{
    if (!m_b_connected)
        return -1;
    return m_pSock->write(data);
}

void TcpSockItem::ConnectSignalSlot()
{
    connect(m_pSock, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_pSock, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_pSock, SIGNAL(connected()), this, SLOT(onConnected()));
}

void TcpSockItem::DisConnectSignalSlot()
{
    disconnect(m_pSock, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    disconnect(m_pSock, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    disconnect(m_pSock, SIGNAL(connected()), this, SLOT(onConnected()));
}

void TcpSockItem::onTimerOut()
{
    if (m_pSock)
    {
        m_pSock->close();
        delete m_pSock;
        m_pSock = nullptr;
    }

    m_pSock = new QTcpSocket;
    ConnectSignalSlot();

    m_pSock->connectToHost(m_remote_addr, m_remote_port);
}

void TcpSockItem::onReadyRead()
{
    //qDebug() << "TcpSockItem::onReadyRead";
    QByteArray data = m_pSock->readAll();
    m_recv_buff.append(data);
    while (m_recv_buff.length() > 0)
    {
        int len = CompleteRecv(m_recv_buff);
        qDebug() << "Hand data len = " << len;
        switch (len)
        {
        case -1://当出现错误时需要关闭套接字
        {
            m_pSock->close();
            return;
        }
        case 0:
        {
            return;
        }
        default:
            m_recv_buff.remove(0, len);
        }
    }
}

void TcpSockItem::onDisconnected()
{
    qDebug() << "TcpSockItem::onDisconnected";
    m_b_connected = false;
    DisConnectSignalSlot();
    if (m_b_reconnect)
        m_timer_connect.start(5000);
    else
        delete this;
}

void TcpSockItem::onConnected()
{
    m_b_connected = true;
    m_timer_connect.stop();
    qDebug() << "TcpSockItem::onConnected";
    return;
}
