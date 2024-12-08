#include "devicecfg.h"
#include "ui_devicecfg.h"
#include "serialport.h"
#include "net.h"
#include "can.h"
#include "di.h"
#include "do.h"
#include "devicescfg.h"
#include <QDebug>

DeviceCfg::DeviceCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceCfg)
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(new SerialPort(this));
    ui->stackedWidget->addWidget(new Net(this));
    ui->stackedWidget->addWidget(new Can(this));
    ui->stackedWidget->addWidget(new Di(this));
    ui->stackedWidget->addWidget(new Do(this));
    ui->stackedWidget->setCurrentIndex(-1);
    //ui->stackedWidget->currentWidget()->setVisible(true);
}

DeviceCfg::~DeviceCfg()
{
    delete ui;
}

void DeviceCfg::setManagerWidget(DevicesCfg* pDevs)
{
    m_pDevsManager = pDevs;
    switch(pDevs->GetCurrentType())
    {
        case 0:
        {
            ui->combo_model->insertItem(0,"阳光电源","sunny");
            ui->combo_model->insertItem(1,"古瑞瓦特","gurui");
            break;
        }
        case 1:
        {
            ui->combo_model->insertItem(0,"三一锂能","sany");
            ui->combo_model->insertItem(1,"永泰数能","yotai");
            break;
        }
        case 2:
        {
            ui->combo_model->insertItem(0,"柴发1","caifa1");
            ui->combo_model->insertItem(1,"柴发1","caifa2");
            break;
        }
        case 3:
        {
            ui->combo_model->insertItem(0,"华为","HuaWei");
            ui->combo_model->insertItem(1,"永泰","YongTaiCharging");
            break;
        }
    }
}

QJsonObject DeviceCfg::GetCfgJson()
{
    QJsonObject obj;

    obj.insert("uuid",ui->le_sn->text());
    obj.insert("model",ui->combo_model->currentText());
    obj.insert("name",ui->le_name->text());
    int nIndex = ui->stackedWidget->currentIndex();
    obj.insert("commtype",nIndex);
    qDebug() << ui->stackedWidget->currentWidget();
    DeviceCfgData* pJsonData = dynamic_cast<DeviceCfgData*>(ui->stackedWidget->currentWidget());
    if(pJsonData)
        obj.insert("commparam",pJsonData->GetCfgJson());

    return obj;
}

void DeviceCfg::SetCfgJson(const QJsonObject& obj)
{
    ui->le_sn->setText(obj["uuid"].toString());
    ui->combo_model->setCurrentText(obj["model"].toString());
    ui->le_name->setText(obj["name"].toString());

    switch(obj["commtype"].toInt())
    {
        case 0:
        {
            ui->radioButton->setChecked(true);
            break;
        }
        case 1:
        {
            ui->radioButton_2->setChecked(true);
            break;
        }
        case 2:
        {
            ui->radioButton_3->setChecked(true);
            break;
        }
        case 3:
        {
            ui->radioButton_4->setChecked(true);
            break;
        }
        case 4:
        {
            ui->radioButton_5->setChecked(true);
            break;
        }
        default:
        {
            break;
        }
    }

    DeviceCfgData* pJsonData = dynamic_cast<DeviceCfgData*>(ui->stackedWidget->currentWidget());
    pJsonData->SetCfgJson(obj["commparam"].toObject());
}

void DeviceCfg::on_radioButton_toggled(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}


void DeviceCfg::on_radioButton_2_toggled(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}


void DeviceCfg::on_radioButton_3_toggled(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
}


void DeviceCfg::on_radioButton_4_toggled(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
}


void DeviceCfg::on_radioButton_5_toggled(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(4);
    }
}


void DeviceCfg::on_btn_del_clicked()
{
    //DevicesCfg* pLayout = dynamic_cast<DevicesCfg*>(parent());
    //qDebug() << "pLayout = " << parentWidget()->parentWidget()->parentWidget()->parentWidget();
    m_pDevsManager->on_del_device(this);
}


void DeviceCfg::on_radioButton_None_clicked(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(-1);
    }
}

