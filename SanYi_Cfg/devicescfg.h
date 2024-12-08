#ifndef DEVICESCFG_H
#define DEVICESCFG_H

#include <QWidget>
#include <QScrollArea>
#include <QVector>
#include "devicecfg.h"
#include <QVBoxLayout>
#include <QJsonObject>
#include "DeviceCfgData.h"

namespace Ui {
class DevicesCfg;
}
class DevicesCfg;

class DevicesCfg : public QWidget, public DeviceCfgData
{
    Q_OBJECT

public:
    explicit DevicesCfg(QWidget *parent = nullptr);
    ~DevicesCfg();

private slots:
    void on_btn_add_clicked();

    void on_btn_save_clicked();

    void on_btn_update_clicked();

    void on_combo_type_currentIndexChanged(int index);

public:
    virtual void GetSrvData();
    virtual int HandNetMsg(int nType, int nLen, const char* pData);
public:
    void on_del_device(DeviceCfg* pDev);
    int  GetCurrentType();

private:
    void ShowAllDevice();
    void ShowDevicesByJson(const QJsonArray& arr);
private:
    Ui::DevicesCfg *ui;
    QScrollArea* m_scrollarea;
    QVBoxLayout* m_p_verlayout;
    QVector<DeviceCfg*> m_vec_device;   //当前配置下的设备列表
    QJsonObject m_json_dev_cfg;
};


#endif // DEVICESCFG_H
