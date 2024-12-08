#ifndef TCPSOCKITEM_H
#define TCPSOCKITEM_H
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <QTimer>

class Q_DECL_EXPORT TcpSockItem : public QObject
{
    Q_OBJECT
public:
    TcpSockItem();
    ~TcpSockItem();
	//当此对象为客户端时需要初使化对端的IP、端口信息
    int Init(const QString &addr, unsigned short port);

	//当此对象为服务端时，需要绑定accept生成的对象
    void Attach(QTcpSocket *pSock);
    int SendData(const char *data, int len);
    int SendData(const QByteArray &data);
    virtual int Initialize() { return 0; }
	//数据接收完成调用此接口，拆包工作交由底下逻辑处理  -1错误 0包未接收完成 其他正数 此包数据大小
    virtual int CompleteRecv(const QByteArray &data) = 0;

public slots:
    void onTimerOut();
	//接收数据函数 接收完成后会调用CompleteRecv
    virtual void onReadyRead();
	//连接断开函数调用
    virtual void onDisconnected();
	//连接建立后调用
    virtual void onConnected();

private:
    void ConnectSignalSlot();
    void DisConnectSignalSlot();

protected:
    QTcpSocket *m_pSock;			//封装的套接字通信对象

private:
    QByteArray m_recv_buff;			//接收缓冲区
    QString m_remote_addr;			//对端IP地址
    unsigned short m_remote_port;	//对端端口
    bool m_b_reconnect;				//是否重连
    bool m_b_connected;				//连接状态
    QTimer m_timer_connect;			//重连定时器
};

#endif // TCPSOCKITEM_H
