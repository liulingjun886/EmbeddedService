#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QWidget>
#include "DeviceCfgData.h"

namespace Ui {
class SerialPort;
}

class SerialPort : public QWidget,public DeviceCfgData
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = nullptr);
    ~SerialPort();
public:
    virtual QJsonObject GetCfgJson();
    virtual void SetCfgJson(const QJsonObject& obj);
private:
    Ui::SerialPort *ui;
};

#endif // SERIALPORT_H
