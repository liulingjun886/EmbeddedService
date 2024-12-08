#ifndef DEVICECFG_H
#define DEVICECFG_H

#include <QWidget>
#include "DeviceCfgData.h"

class DevicesCfg;

namespace Ui {
class DeviceCfg;
}

class DeviceCfg : public QWidget, public DeviceCfgData
{
    Q_OBJECT

public:
    explicit DeviceCfg(QWidget *parent = nullptr);
    ~DeviceCfg();

    void setManagerWidget(DevicesCfg* pDevs);
    virtual QJsonObject GetCfgJson();
    virtual void SetCfgJson(const QJsonObject& obj);
private slots:
    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_radioButton_3_toggled(bool checked);

    void on_radioButton_4_toggled(bool checked);

    void on_radioButton_5_toggled(bool checked);

    void on_btn_del_clicked();

    void on_radioButton_None_clicked(bool checked);

private:
    Ui::DeviceCfg *ui;
    DevicesCfg* m_pDevsManager;
};

#endif // DEVICECFG_H
