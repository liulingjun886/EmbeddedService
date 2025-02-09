#include "syscfg.h"
#include "ui_syscfg.h"
#include <QDebug>
#include <QJsonDocument>
#include "StatusInfo.h"
#include <QDebug>
#include "common.h"
#include <QJsonArray>
#include <QMessageBox>

SysCfg::SysCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysCfg)
{
    ui->setupUi(this);
    //ui->cb_type->findData()

    //GetCfgJson();

    LoadModelInfo();
}

SysCfg::~SysCfg()
{
    delete ui;
}

int SysCfg::HandNetMsg(int nType, int nLen, const char* pData)
{
    QByteArray jsonArr;
    jsonArr.append(pData,nLen);
    QJsonDocument doc = QJsonDocument::fromJson(jsonArr);
    qDebug() << doc.object();
    QJsonObject obj = doc.object();
    if(SYS_CONF_GET == nType)
    {
        QString strModel = obj["model"].toString();
        for(int i = 0; i < m_vec_model_type_infos.size(); ++i)
        {
            const QVector<TypeSelect>& modelInfos = m_vec_model_type_infos.at(i);
            for(int j = 0; j < modelInfos.size(); ++j)
            {
                const TypeSelect& modelinfo = modelInfos.at(j);
                if(modelinfo.value == strModel)
                {
                    ui->cb_type->setCurrentIndex(i);
                    ui->cb_mode->setCurrentIndex(j);
                    ui->le_sn->setText(obj["sn"].toString());
                    ui->le_name->setText(obj["name"].toString());
                    if(g_status_info.subDeviceType != modelinfo.nSubDevType)
                    {
                        g_status_info.subDeviceType = modelinfo.nSubDevType;
                        g_status_info.pMainDialog->UpdateSubDeviceWidget();
                    }
                    return 0;
                }
            }
        }
    }
    else if(SYS_CONF_SET == nType)
    {
        int ret = obj["ret"].toInt();
        if(0 == ret)
        {
            QMessageBox::information(g_status_info.pMainDialog,"成功","设置成功");
            GetSrvData();
        }
        else
        {
            QMessageBox::warning(g_status_info.pMainDialog,"失败","设置失败");
        }
    }
    return 0;
}

void SysCfg::GetSrvData()
{
    on_cb_type_currentIndexChanged(ui->cb_type->currentIndex());
    QByteArray arr(sizeof(NetMsg),0);
    NetMsg* pMsg = (NetMsg*)arr.data();
    pMsg->nType = SYS_CONF_GET;
    SendData(arr);
}

void SysCfg::on_pushButton_clicked()
{
    QJsonObject obj;
    obj["model"] = ui->cb_mode->currentData().toString();
    obj["sn"] = ui->le_sn->text();
    obj["name"] = ui->le_name->text();

    QByteArray data(sizeof(NetMsg),0);
    QJsonDocument doc(obj);
    data.append(doc.toJson(QJsonDocument::Compact));
    NetMsg* pMsg = (NetMsg*)data.data();
    pMsg->nType = SYS_CONF_SET;
    pMsg->nLen = data.length() - sizeof(NetMsg);
    SendData(data);
}


void SysCfg::on_cb_type_currentIndexChanged(int index)
{
    ui->cb_mode->clear();
    if(m_vec_model_type_infos.size() <= 0)
        return;
    const QVector<TypeSelect>& vec_model_type_info = m_vec_model_type_infos.at(index);
    int i = 0;
    for(QVector<TypeSelect>::const_iterator it = vec_model_type_info.begin(); it != vec_model_type_info.end(); ++it,++i)
    {
        ui->cb_mode->insertItem(i,it->text,it->value);
    }
}

int SysCfg::LoadModelInfo()
{
    QString cfgFile = g_status_info.strAppDir;
    cfgFile.append("/conf/model.json");
    qDebug()<< cfgFile;
    QByteArray cfgContent = Common::GetFileContent(cfgFile);
    QJsonDocument doc = QJsonDocument::fromJson(cfgContent);
    if(!doc.isArray())
        return -1;

    QJsonArray arr = doc.array();
    for(int i = 0; i < arr.size(); ++i)
    {
        const QJsonObject obj = arr[i].toObject();
        ui->cb_type->insertItem(i,obj["text"].toString());
        const QJsonArray subitems = obj["subitem"].toArray();
        QVector<TypeSelect> subiteminfos;
        for(int j = 0; j < subitems.size(); ++j)
        {
            TypeSelect info;
            const QJsonObject subitem = subitems[j].toObject();
            info.text = subitem["text"].toString();
            info.value = subitem["value"].toString();
            info.nSubDevType = i*100 + j;
            subiteminfos.push_back(info);
        }
        m_vec_model_type_infos.push_back(subiteminfos);
    }
    return 0;
}

