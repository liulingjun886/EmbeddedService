#include "devicescfg.h"
#include "ui_devicescfg.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

DevicesCfg::DevicesCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevicesCfg)
{
    ui->setupUi(this);
    m_scrollarea = new QScrollArea(this);
    m_scrollarea->setGeometry(0,35,745,720);
    m_scrollarea->setWidget(ui->wid_devices);
    m_scrollarea->setVisible(true);
    m_p_verlayout = new QVBoxLayout(this);
    ui->wid_devices->setLayout(m_p_verlayout);

    ui->combo_type->insertItem(0,"光伏设备");
    ui->combo_type->insertItem(1,"储能设备");
    ui->combo_type->insertItem(2,"柴发设备");
    ui->combo_type->insertItem(4,"负载设备");

    connect(this,SIGNAL(del_device(void*)),this,SLOT(on_del_device(void*)));

    ShowAllDevice();
}

DevicesCfg::~DevicesCfg()
{
    delete ui;
}

void DevicesCfg::on_btn_add_clicked()
{
    DeviceCfg* pDev = new DeviceCfg(this);
    pDev->setManagerWidget(this);
    m_vec_device.append(pDev);
    ShowAllDevice();

}

void DevicesCfg::ShowAllDevice()
{
    for(int i = m_vec_device.size()-1; i >=0 ; --i)
    {
        DeviceCfg* pDev = m_vec_device[i];
        m_p_verlayout->addWidget(pDev);
        pDev->setVisible(true);
    }
    QRect rect = ui->wid_devices->geometry();
    ui->wid_devices->setGeometry(rect.x(),rect.y(),rect.width(),240*m_vec_device.size());
    qDebug() << "ShowAllDevice"<<m_vec_device.size() << "count = " << m_p_verlayout->count();
}

void DevicesCfg::ShowDevicesByJson(const QJsonArray& arr)
{
    for(int i = 0; i < arr.size(); i++)
    {
        const QJsonObject& obj = arr[i].toObject();
        DeviceCfg* pDev = new DeviceCfg(this);
        pDev->setManagerWidget(this);
        m_vec_device.append(pDev);
        pDev->SetCfgJson(obj);
        qDebug()<<"ShowDevicesByJson";
    }
    ShowAllDevice();
}

void DevicesCfg::GetSrvData()
{
    QByteArray data(sizeof(NetMsg),0);
    NetMsg* pMsg = (NetMsg*)data.data();
    pMsg->nType = DEV_CONF_GET;
    SendData(data);
}

int DevicesCfg::HandNetMsg(int nType, int nLen, const char* pData)
{
    QByteArray jsonArr;
    jsonArr.append(pData,nLen);
    QJsonDocument doc = QJsonDocument::fromJson(jsonArr);
    qDebug() << doc.object();
    switch(nType)
    {
        case DEV_CONF_GET:
        {
            m_json_dev_cfg = doc.object();
            on_combo_type_currentIndexChanged(ui->combo_type->currentIndex());
            qDebug() << m_json_dev_cfg << pData;
            return 0;
        }
        case DEV_CONF_SET:
        {
            QJsonObject obj = doc.object();
            if(0 == obj["ret"].toInt())
            {
                QMessageBox::information(this,"成功","上传服务器成功");
            }
            else
            {
                QMessageBox::warning(this,"失败","上传服务器失败，请检查相关配置");
            }
            return 0;
        }
        default:
            return 0;
    }
    return 0;
}

void DevicesCfg::on_del_device(DeviceCfg* pDev)
{
    qDebug() << m_vec_device.size();
    for(int i = 0; i < m_vec_device.size(); ++i)
    {
        if(m_vec_device[i] == pDev)
        {
            m_vec_device.remove(i);
            delete pDev;
            ShowAllDevice();
            return;
        }
    }

}

int  DevicesCfg::GetCurrentType()
{
    return ui->combo_type->currentIndex();
}

void DevicesCfg::on_btn_save_clicked()
{
    qDebug() << "on_btn_save_clicked" << m_vec_device.size();

    QJsonArray arr;
    for(int i = 0; i < m_vec_device.size(); ++i)
    {
        arr.append(m_vec_device[i]->GetCfgJson());
    }
    switch(ui->combo_type->currentIndex())
    {
        case 0:
        {
            m_json_dev_cfg["Pv"] = arr;
            break;
        }
        case 1:
        {
            m_json_dev_cfg["Ess"] = arr;
            break;
        }
        case 2:
        {
            m_json_dev_cfg["Diesel"] = arr;
            break;
        }
        case 3:
        {
            m_json_dev_cfg["Load"] = arr;
            break;
        }
        default:
            break;
    }

    QJsonDocument doc(arr);
    qDebug() << doc.toJson(QJsonDocument::Compact);
}


void DevicesCfg::on_btn_update_clicked()
{
    QByteArray data(sizeof(NetMsg),0);
    QJsonDocument doc(m_json_dev_cfg);
    data.append(doc.toJson(QJsonDocument::Compact));
    NetMsg* pMsg = (NetMsg*)data.data();
    pMsg->nType = DEV_CONF_SET;
    pMsg->nLen = data.length() - sizeof(NetMsg);
    qDebug() << data.length() << pMsg->nLen << data;
    SendData(data);
}


void DevicesCfg::on_combo_type_currentIndexChanged(int index)
{
    qDebug() << "on_combo_type_currentIndexChanged" << index;
    for(int i = 0; i < m_vec_device.size(); ++i)
    {
        delete m_vec_device[i];
    }
    m_vec_device.clear();

    switch(index)
    {
        case 0:
        {
            ShowDevicesByJson(m_json_dev_cfg["Pv"].toArray());
            return;
        }
        case 1:
        {
            ShowDevicesByJson(m_json_dev_cfg["Ess"].toArray());
            return;
        }
        case 2:
        {
            ShowDevicesByJson(m_json_dev_cfg["Diesel"].toArray());
            return;
        }
        case 3:
        {
            ShowDevicesByJson(m_json_dev_cfg["Load"].toArray());
            return;
        }
    }
}

