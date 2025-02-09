#include "dialog.h"
#include "ui_dialog.h"
#include "serialport.h"
#include "filetransmit.h"
#include "syscfg.h"
#include "devicescfg.h"
#include <QDebug>
#include <QMap>
#include "tcpsockitem.h"
#include "devicesrvcfg.h"
#include "StatusInfo.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QMap>

typedef void (Dialog::*MainFun)(int subCode);
typedef void (Dialog::*SubFun)();

typedef QMap<int,MainFun> MapFun;

StatusInfo g_status_info;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    g_status_info.pMainDialog = this;
    g_status_info.strAppDir = QCoreApplication::applicationDirPath();

    ui->setupUi(this);
    //ui->splitter->setStretchFactor(1,1);
//    ui->stackedWidget->addWidget(new DeviceSrvCfg(this));
//    ui->stackedWidget->addWidget(new FileTransmit(this));
//    ui->stackedWidget->addWidget(new SysCfg(this));
//    ui->stackedWidget->addWidget(new DevicesCfg(this));
    ui->stackedWidget->insertWidget(0,new DeviceSrvCfg(this));
    ui->stackedWidget->insertWidget(1,new FileTransmit(this));
    ui->stackedWidget->insertWidget(2,new SysCfg(this));
    ui->stackedWidget->insertWidget(3,ui->stackedWidget->widget(2));
//    ui->stackedWidget->insertWidget(4,new FileTransmit(this));
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->currentWidget()->setVisible(true);
    //ui->stackedWidget->setGeometry(0,0,1000,750);
    InitTreeItem();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::InitTreeItem()
{
    ui->treeWidget->setHeaderHidden(true);
    AddItem("连接参数",0);
    AddItem("文件传输",1);
    AddItem("系统参数",2);
    AddItem("设备管理",3);
    //AddItem("电表管理",4);
}

QTreeWidgetItem * Dialog::AddItem(QString text, int data)
{
    QTreeWidgetItem *itemTop = new QTreeWidgetItem(ui->treeWidget);
    itemTop->setText(0,text);
    itemTop->setData(0,Qt::ToolTipRole,data);
    return itemTop;
}

void Dialog::AddSubItem(QTreeWidgetItem *itemTop, QString text, int data)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(itemTop);
    item->setText(0,text);
    qDebug()<< itemTop->data(0,Qt::ToolTipRole).toInt();
    item->setData(0,Qt::ToolTipRole,itemTop->data(0,Qt::ToolTipRole).toInt() | data);
}

int Dialog::HandNetMsg(int nType, int nLen, const char* pData)
{
    DeviceCfgData* pWidget = dynamic_cast<DeviceCfgData*>(ui->stackedWidget->currentWidget());
    if(pWidget)
        return pWidget->HandNetMsg(nType,nLen,pData);
    return 0;
}

int Dialog::UpdateSubDeviceWidget()
{
    static QMap<int,QWidget*> map_subdevwidget = {
        {0,new DevicesCfg},
        {101,new FileTransmit}
    };

    QMap<int,QWidget*>::iterator it = map_subdevwidget.find(g_status_info.subDeviceType);
    if(it == map_subdevwidget.end())
    {
        g_status_info.subDeviceType = -1;
        return -1;
    }

    ui->stackedWidget->removeWidget(ui->stackedWidget->widget(3));
    ui->stackedWidget->insertWidget(3,it.value());

    return 0;
}

void Dialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    (void)column;
//    static MapFun map_fun = {
//        {2,&Dialog::OnDeviceManager}
//    };
    int nMainCode = item->data(0,Qt::ToolTipRole).toInt();
    //int nSubCode = item->data(0,Qt::ToolTipRole).toInt() & 0xFF;

//    auto it = map_fun.find(nMainCode);
//    if(it != map_fun.end())
//    {
//        MainFun fun = it.value();
//        (this->*fun)(nSubCode);
//    }

//    if(0 == item->childCount())
//    {
//        ui->stackedWidget->setCurrentIndex(nMainCode);
//    }

      if(nMainCode == 3)
      {
          //ui->stackedWidget->removeWidget()
          if(-1 == g_status_info.subDeviceType)
          {
              QMessageBox::warning(g_status_info.pMainDialog,"失败","请查看产品类型");
              return;
          }
      }

      ui->stackedWidget->setCurrentIndex(nMainCode);
}

void Dialog::OnDeviceManager(int nSubCode)
{
    switch(nSubCode)
    {
        case 1:
        {

        }
    }
}

void Dialog::on_stackedWidget_currentChanged(int arg1)
{
    DeviceCfgData* pCfgWidget = dynamic_cast<DeviceCfgData*>(ui->stackedWidget->currentWidget());
    if(pCfgWidget)
        pCfgWidget->GetSrvData();
    qDebug() << "on_stackedWidget_currentChanged" << arg1;
}

